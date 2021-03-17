#include "Precompiled.h"
#include "Vulkan/Shader/VulkanShader.h"
#include "RendererAPI/Renderer.h"
#include "vulkan/Device/VulkanDevice.h"
#include "vulkan/Util/VulkanFunctions.h"
#include "External/shaderc/libshaderc/include/shaderc/shaderc.hpp"
#include "External/SPIRV-Cross/Inc/spirv_cross.hpp"
#include <fstream>
#include <string>


namespace SingularityEngine::SERenderer
{	
	// FNV-1a 32bit hashing algorithm.
	constexpr uint32_t fnv1a_32(char const* s, std::size_t count)
	{
		return ((count ? fnv1a_32(s, count - 1) : 2166136261u) ^ s[count]) * 16777619u;
	}
	static uint32_t hash_descriptor_layout_info(VkDescriptorSetLayoutCreateInfo* info)
	{
		//we are going to put all the data into a string and then hash the string
		std::stringstream ss;

		ss << info->flags;
		ss << info->bindingCount;

		for (auto i = 0u; i < info->bindingCount; i++) {
			const VkDescriptorSetLayoutBinding& binding = info->pBindings[i];

			ss << binding.binding;
			ss << binding.descriptorCount;
			ss << binding.descriptorType;
			ss << binding.stageFlags;
		}

		auto str = ss.str();

		return fnv1a_32(str.c_str(), str.length());
	}

	static ShaderUniformType SPIRTypeToShaderUniformType(spirv_cross::SPIRType type)
	{
		switch (type.basetype)
		{
		case spirv_cross::SPIRType::Boolean:  return ShaderUniformType::Bool;
		case spirv_cross::SPIRType::Int:      return ShaderUniformType::Int;
		case spirv_cross::SPIRType::UInt:     return ShaderUniformType::UInt;
		case spirv_cross::SPIRType::Float:
			if (type.vecsize == 1)            return ShaderUniformType::Float;
			if (type.vecsize == 2)            return ShaderUniformType::Vec2;
			if (type.vecsize == 3)            return ShaderUniformType::Vec3;
			if (type.vecsize == 4)            return ShaderUniformType::Vec4;

			if (type.columns == 3)            return ShaderUniformType::Mat3;
			if (type.columns == 4)            return ShaderUniformType::Mat4;
			break;
		}
		ASSERT(false, "Unknown type!");
		return ShaderUniformType::None;
	}


	static shaderc_shader_kind VkShaderStageToShaderC(VkShaderStageFlagBits stage)
	{
		switch (stage)
		{
		case VK_SHADER_STAGE_VERTEX_BIT:    return shaderc_vertex_shader;
		case VK_SHADER_STAGE_FRAGMENT_BIT:  return shaderc_fragment_shader;
		case VK_SHADER_STAGE_COMPUTE_BIT:   return shaderc_compute_shader;
		}
		ASSERT(false, "Unknown type!");
		return (shaderc_shader_kind)0;
	}

	static VkShaderStageFlagBits shaderTypeFromString(std::string type)
	{
		if (type.compare("vertex") == 0)
		{
			return VK_SHADER_STAGE_VERTEX_BIT;
		}
		if (type.compare("fragment") == 0 || type.compare("pixel"))
		{
			return VK_SHADER_STAGE_FRAGMENT_BIT;
		}
		if (type.compare("compute") == 0)
		{
			return VK_SHADER_STAGE_COMPUTE_BIT;
		}
		if (type.compare("geometry") == 0)
		{
			return VK_SHADER_STAGE_GEOMETRY_BIT;
		}
		return VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
	}

	static std::string readShaderFromFile(const std::string& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
		}
		else
		{
			ASSERT(false, "Could not load shader!");
		}
		in.close();
		return result;
	}


	VulkanShader::VulkanShader(std::string filePath) : mAssetPath(filePath)
	{
		std::filesystem::path p = mAssetPath;
		mName = p.filename().stem().string();
	}

	VulkanShader::~VulkanShader()
	{
		cleanup();
	}

	void VulkanShader::reload(bool forceCompile)
	{
		cleanup();
		std::string source = readShaderFromFile(mAssetPath);
		preProcess(source);
		std::unordered_map<VkShaderStageFlagBits, std::vector<uint32_t>> compiledShaderData;
		compileOrGetVulkanBinary(compiledShaderData, forceCompile);
		createAllShaders(compiledShaderData);
		reflectAllShaders(compiledShaderData);
	}



	const std::string& VulkanShader::getName() const
	{
		return mName;
	}

	void VulkanShader::setUniform(const std::string& fullname, Math::Matrix4x4 value)
	{
		setUniformBuffer(fullname, &value, sizeof(Math::Matrix4x4));
	}

	void VulkanShader::setUniform(const std::string& fullname, Math::Matrix3x3 value)
	{
		setUniformBuffer(fullname, &value, sizeof(Math::Matrix3x3));
	}

	void VulkanShader::setUniform(const std::string& fullname, Math::Vector4 value)
	{
		setUniformBuffer(fullname, &value, sizeof(Math::Vector4));
	}

	void VulkanShader::setUniform(const std::string& fullname, Math::Vector3 value)
	{
		setUniformBuffer(fullname, &value, sizeof(Math::Vector3));
	}

	void VulkanShader::setUniform(const std::string& fullname, Math::Vector2 value)
	{
		setUniformBuffer(fullname, &value, sizeof(Math::Vector2));
	}

	void VulkanShader::setUniform(const std::string& fullname, int value)
	{
		setUniformBuffer(fullname, &value, sizeof(int));
	}

	void VulkanShader::setUniform(const std::string& fullname, float value)
	{
		setUniformBuffer(fullname, &value, sizeof(float));
	}

	void VulkanShader::setUniformBuffer(const std::string& name, const void* data, uint32_t size)
	{
		UNREFERENCED_PARAMETER(name);
		UNREFERENCED_PARAMETER(data);
		UNREFERENCED_PARAMETER(size);
	}

	void VulkanShader::unbind() const
	{

	}

	void VulkanShader::bind()
	{

	}

	std::vector<uint32_t> VulkanShader::loadBinaryShader(std::string path)
	{
		std::vector<uint32_t> binaryShader;
		FILE* f; // find OS independant way of doing this, possibly down in core
		fopen_s(&f, path.c_str(), "rb");
		if (f)
		{
			fseek(f, 0, SEEK_END);
			uint64_t size = ftell(f);
			fseek(f, 0, SEEK_SET);
			binaryShader = std::vector<uint32_t>(size / sizeof(uint32_t));
			fread(binaryShader.data(), sizeof(uint32_t), binaryShader.size(), f);
			fclose(f);
		}
		else
		{

			LOG("[SERenderer::VulkanShader] Failed to load cached shader %s at file location: %s", mName.c_str(), mAssetPath.c_str());
		}
		return binaryShader;
	}

	std::string VulkanShader::getCacheShaderFilePath(VkShaderStageFlagBits stageFlag)
	{
		std::string shaderType;
		switch (stageFlag)
		{
		case VK_SHADER_STAGE_VERTEX_BIT:
			shaderType = "vert";
			break;
		case VK_SHADER_STAGE_FRAGMENT_BIT:
			shaderType = "frag";
			break;
		default:
			break;
		}

		//Fragment Shader
		std::filesystem::path p = mAssetPath;
		auto path = p.parent_path() / "cached" / (p.filename().string() + ".cached_vulkan." + shaderType);
		return path.string();
	}


	const std::vector<VkPipelineShaderStageCreateInfo>& VulkanShader::getShaderStages()
	{
		return mShaderStages;
	}

	void VulkanShader::compileOrGetVulkanBinary(std::unordered_map<VkShaderStageFlagBits, std::vector<uint32_t>>& outputBinary, bool forceCompile)
	{
		for (auto[stage, source] : mShaderSources)
		{
			if (!forceCompile)
			{
				outputBinary[stage] = loadBinaryShader(getCacheShaderFilePath(stage));
			}
			if ((outputBinary[stage]).empty())
			{
				outputBinary[stage] = compileVulkanBinaryFromGLSL(stage);
			}
		}
	}

	void VulkanShader::preProcess(const std::string& source)
	{
		const char* typeToken = "#type";
		size_t typeTokenLength = std::strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			ASSERT(eol != std::string::npos, "[SERenderer::VulkanShader] syntax error!");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			ASSERT(type.compare("fragment") == 0 || type.compare("vertex") || type.compare("pixel") || type.compare("compute") || type.compare("geometry"), "[SERenderer::Shader] Invalid Shader type specified.");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			VkShaderStageFlagBits shaderType = shaderTypeFromString(type);
			mShaderSources[shaderType] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}
	}


	//TODO: add a second version of this for trans (TRANS RIGHTS!) compilation of HLSL to SPIRV
	std::vector<uint32_t> VulkanShader::compileVulkanBinaryFromGLSL(VkShaderStageFlagBits stage) 
	{
		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		std::vector<uint32_t> compiledShader;
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
		const bool optimize = false;
		if (optimize)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

		{
			std::ifstream shaderFile(mAssetPath, std::ios::in);
			if (shaderFile.is_open())
			{
				std::string shaderStringData;
				shaderFile.seekg(0, std::ios::end);
				shaderStringData.reserve(shaderFile.tellg());
				shaderFile.seekg(0, std::ios::beg);
				shaderStringData.assign((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
				if (!shaderStringData.empty())
				{
					preProcess(shaderStringData);
				}
				shaderFile.close();
			}
		}

		{
			auto& shaderSource = mShaderSources.at(stage);
			shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(shaderSource, VkShaderStageToShaderC(stage), mAssetPath.c_str(), options);
			if (module.GetCompilationStatus() != shaderc_compilation_status_success)
			{
				ASSERT(false, "%s", module.GetErrorMessage().c_str());
			}

			const auto* begin = (const uint8_t*)module.begin();
			const auto* end = (const uint8_t*)module.end();
			const ptrdiff_t size = end - begin;

			compiledShader = std::vector<uint32_t>(module.cbegin(), module.cend());
		}

		{

			std::string cachedFilePath = getCacheShaderFilePath(stage);
			FILE* f;
			fopen_s(&f, cachedFilePath.c_str(), "wb");
			if (f)
			{
				fwrite(compiledShader.data(), sizeof(uint32_t), compiledShader.size(), f);
				fclose(f);
			}
		}
		return compiledShader;
	}

	void VulkanShader::cleanup()
	{
		std::shared_ptr<VulkanDevice> device = std::dynamic_pointer_cast<VulkanDevice>(Renderer::Get()->getGraphicsDevice());

		for (auto& shaderPipelineInfo : mShaderStages)
		{
			vkDestroyShaderModule(device->getLogicalDevice(), shaderPipelineInfo.module, nullptr);
		}
		mShaderStages.clear();
	}

	VulkanShader::DescriptorSetLayoutData VulkanShader::reflect(VkShaderStageFlagBits stage, std::vector<uint32_t>& shaderData)
	{
		std::shared_ptr<VulkanDevice> device = std::dynamic_pointer_cast<VulkanDevice>(Renderer::Get()->getGraphicsDevice());

		SENGINE_TRACE("==========================");
		SENGINE_TRACE(" Vulkan Shader Reflection ");
		SENGINE_TRACE(" %s", mAssetPath.c_str());
		SENGINE_TRACE("==========================");

		spirv_cross::Compiler compiler(shaderData);
		auto resources = compiler.get_shader_resources();
		DescriptorSetLayoutData layout = {};
		SENGINE_TRACE("Uniform Buffers:");
		for (const auto& resource : resources.uniform_buffers)
		{
			const auto& name = resource.name;
			auto& bufferType = compiler.get_type(resource.base_type_id);
			size_t memberCount = bufferType.member_types.size();
			uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
			uint32_t descriptorSet = compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
			size_t size = compiler.get_declared_struct_size(bufferType);
			ReflectedBinding& reflectedBinding = mBindings[name];
			reflectedBinding.binding = binding;
			reflectedBinding.set = descriptorSet;
			reflectedBinding.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			VkDescriptorSetLayoutBinding& layoutBinding = layout.bindings[binding];
			layoutBinding.binding = binding;
			layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			layoutBinding.descriptorCount = (uint32_t)memberCount;
			layoutBinding.stageFlags |= stage;

			SENGINE_TRACE("  {0} ({1}, {2})", name, descriptorSet, binding);
			SENGINE_TRACE("  Member Count: {1}", memberCount);
			SENGINE_TRACE("  Size: {1}", size);
			SENGINE_TRACE("-------------------");
		}

		SENGINE_TRACE("Push Constants:");
		for (const auto& resource : resources.push_constant_buffers)
		{
			const auto& bufferName = resource.name;
			auto& bufferType = compiler.get_type(resource.base_type_id);
			auto bufferSize = compiler.get_declared_struct_size(bufferType);
			size_t memberCount = bufferType.member_types.size();
			uint32_t bufferOffset = 0;
			if (!mPushConstantRanges.empty())
				bufferOffset = mPushConstantRanges.back().offset + mPushConstantRanges.back().size;

			auto& pushConstantRange = mPushConstantRanges.emplace_back();
			pushConstantRange.stageFlags = stage;
			pushConstantRange.size = (uint32_t)bufferSize;
			pushConstantRange.offset = bufferOffset;
			// Skip empty push constant buffers - these are for the renderer only
			SENGINE_TRACE("  Name: {0}", bufferName);
			SENGINE_TRACE("  Member Count: {0}", memberCount);
			SENGINE_TRACE("  Size: {0}", bufferSize);
		}

		SENGINE_TRACE("Sampled Images:");
		for (const auto& resource : resources.sampled_images)
		{
			const auto& name = resource.name;
			//auto& type = compiler.get_type(resource.base_type_id);
			uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
			uint32_t descriptorSet = compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
			//uint32_t dimension = (uint32_t)type.image.dim;
			ReflectedBinding& reflectedBinding = mBindings[name];
			reflectedBinding.binding = binding;
			reflectedBinding.set = descriptorSet;
			reflectedBinding.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			VkDescriptorSetLayoutBinding& layoutBinding = layout.bindings[binding];
			layoutBinding.binding = binding;
			layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			layoutBinding.descriptorCount = 1;
			layoutBinding.stageFlags |= stage;

			SENGINE_TRACE("  {0} ({1}, {2})", name, descriptorSet, binding);
		}

		SENGINE_TRACE("Storage Images:");
		for (const auto& resource : resources.storage_images)
		{
			const auto& name = resource.name;
			//auto& type = compiler.get_type(resource.base_type_id);
			uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
			uint32_t descriptorSet = compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
			//uint32_t dimension = type.image.dim;

			ReflectedBinding& reflectedBinding = mBindings[name];
			reflectedBinding.binding = binding;
			reflectedBinding.set = descriptorSet;
			reflectedBinding.type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
			VkDescriptorSetLayoutBinding& layoutBinding = layout.bindings[binding];
			layoutBinding.binding = binding;
			layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
			layoutBinding.descriptorCount = 1;
			layoutBinding.stageFlags |= stage;

			SENGINE_TRACE("  {0} ({1}, {2})", name, descriptorSet, binding);
		}
		SENGINE_TRACE("===========================");

		LOG("Storage Buffers:");
		for (const auto& resource : resources.storage_buffers)
		{
			const auto& name = resource.name;
			//auto& type = compiler.get_type(resource.base_type_id);
			uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
			uint32_t descriptorSet = compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
			//uint32_t dimension = type.image.dim;

			ReflectedBinding& reflectedBinding = mBindings[name];
			reflectedBinding.binding = binding;
			reflectedBinding.set = descriptorSet;
			reflectedBinding.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
			VkDescriptorSetLayoutBinding& layoutBinding = layout.bindings[binding];
			layoutBinding.binding = binding;
			layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
			layoutBinding.descriptorCount = 1;
			layoutBinding.stageFlags |= stage;

			SENGINE_TRACE("  {0} ({1}, {2})", name, descriptorSet, binding);
		}

		LOG("Separate Images:");
		for (const auto& resource : resources.separate_images)
		{
			const auto& name = resource.name;
			//auto& type = compiler.get_type(resource.base_type_id);
			uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
			uint32_t descriptorSet = compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
			//uint32_t dimension = type.image.dim;

			ReflectedBinding& reflectedBinding = mBindings[name];
			reflectedBinding.binding = binding;
			reflectedBinding.set = descriptorSet;
			reflectedBinding.type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
			VkDescriptorSetLayoutBinding& layoutBinding = layout.bindings[binding];
			layoutBinding.binding = binding;
			layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
			layoutBinding.descriptorCount = 1;
			layoutBinding.stageFlags |= stage;

			SENGINE_TRACE("  {0} ({1}, {2})", name, descriptorSet, binding);
		}

		LOG("Separate Samplers:");
		for (const auto& resource : resources.separate_samplers)
		{
			const auto& name = resource.name;
			//auto& type = compiler.get_type(resource.base_type_id);
			uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
			uint32_t descriptorSet = compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
			//uint32_t dimension = type.image.dim;

			ReflectedBinding& reflectedBinding = mBindings[name];
			reflectedBinding.binding = binding;
			reflectedBinding.set = descriptorSet;
			reflectedBinding.type = VK_DESCRIPTOR_TYPE_SAMPLER;
			VkDescriptorSetLayoutBinding& layoutBinding = layout.bindings[binding];
			layoutBinding.binding = binding;
			layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
			layoutBinding.descriptorCount = 1;
			layoutBinding.stageFlags |= stage;

			SENGINE_TRACE("  {0} ({1}, {2})", name, descriptorSet, binding);
		}
		return layout;
	}

	void VulkanShader::createAllShaders(const std::unordered_map<VkShaderStageFlagBits, std::vector<uint32_t>>& shaderData)
	{
		std::shared_ptr<VulkanDevice> device = std::dynamic_pointer_cast<VulkanDevice>(Renderer::Get()->getGraphicsDevice());
		mShaderStages.clear();
		for (auto [stage, data] : shaderData)
		{
			ASSERT((data.size()), "[SERenderer::VulkanShader] buffer size is invalid!");
			// Create a new shader module that will be used for pipeline creation
			VkShaderModuleCreateInfo moduleCreateInfo{};
			moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			moduleCreateInfo.codeSize = data.size() * sizeof(uint32_t);
			moduleCreateInfo.pCode = data.data();

			VkShaderModule shaderModule;
			ASSERT(vkCreateShaderModule(device->getLogicalDevice(), &moduleCreateInfo, NULL, &shaderModule) == VK_SUCCESS, "[SERenderer::VulkanShader] Failed to create shader module!");

			VkPipelineShaderStageCreateInfo& shaderStage = mShaderStages.emplace_back();
			shaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			shaderStage.stage = stage;
			shaderStage.module = shaderModule;
			shaderStage.pName = "main";
		}
	}

	void VulkanShader::reflectAllShaders(const std::unordered_map<VkShaderStageFlagBits, std::vector<uint32_t>>& shaderData)
	{
		mBindings.clear();
		std::vector<DescriptorSetLayoutData> setLayouts;
		for (auto [stage, data] : shaderData)
		{
			setLayouts.push_back(reflect(stage, data));
		}
		std::array<DescriptorSetLayoutData, 4> mergedLayouts;
		std::shared_ptr<VulkanDevice> device = std::dynamic_pointer_cast<VulkanDevice>(Renderer::Get()->getGraphicsDevice());

		for (uint32_t i = 0; i < 4; i++) {

			DescriptorSetLayoutData& ly = mergedLayouts[i];

			ly.setNumber = i;

			ly.createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;

			std::unordered_map<int, VkDescriptorSetLayoutBinding> binds;
			for (auto& s : setLayouts) {
				if (s.setNumber == i) {
					for (auto& b : s.bindings)
					{
						auto it = binds.find(b.binding);
						if (it == binds.end())
						{
							binds[b.binding] = b;
						}
						else {
							//merge flags
							binds[b.binding].stageFlags |= b.stageFlags;
						}

					}
				}
			}
			for (auto [k, v] : binds)
			{
				ly.bindings.push_back(v);
			}
			//sort the bindings, for hash purposes
			std::sort(ly.bindings.begin(), ly.bindings.end(), [](VkDescriptorSetLayoutBinding& a, VkDescriptorSetLayoutBinding& b) {
				return a.binding < b.binding;
				});


			ly.createInfo.bindingCount = (uint32_t)ly.bindings.size();
			ly.createInfo.pBindings = ly.bindings.data();
			ly.createInfo.flags = 0;
			ly.createInfo.pNext = 0;


			if (ly.createInfo.bindingCount > 0) {
				mSetHashes[i] = hash_descriptor_layout_info(&ly.createInfo);
				vkCreateDescriptorSetLayout(device->getLogicalDevice(), &ly.createInfo, nullptr, &mDescriptorSetLayouts[i]);
			}
			else {
				mSetHashes[i] = 0;
				mDescriptorSetLayouts[i] = VK_NULL_HANDLE;
			}
		}
	}

}
