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
		createDescriptors();
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
		size_t pos = name.find('.', 0);
		ASSERT(pos != std::string::npos, "[SERenderer::VulkanShader] improper buffer name passed in.");
		
		std::string bufferName = name.substr(0, pos);
		std::string memberName = name.substr(pos);
		ASSERT(mBuffers.find(bufferName) != mBuffers.end(), "[SERenderer::VulkanShader] could not find buffer specified.");
		ASSERT(mBuffers[bufferName].Uniforms.find(memberName) != mBuffers[bufferName].Uniforms.end(), "[SERenderer::VulkanShader] could not find buffer specified.");

		ShaderUniform& uniform = mBuffers[bufferName].Uniforms[memberName];
		void* destPtr = mapUniformBuffer(uniform.GetBindingPoint(), uniform.GetSet());
		memcpy(destPtr, data, size);
		unmapUniformbuffer(uniform.GetBindingPoint(), uniform.GetSet());
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

	void VulkanShader::reflect(VkShaderStageFlagBits stage, std::vector<uint32_t>& shaderData)
	{
		std::shared_ptr<VulkanDevice> device = std::dynamic_pointer_cast<VulkanDevice>(Renderer::Get()->getGraphicsDevice());

		SENGINE_TRACE("==========================");
		SENGINE_TRACE(" Vulkan Shader Reflection ");
		SENGINE_TRACE(" %s", mAssetPath.c_str());
		SENGINE_TRACE("==========================");

		spirv_cross::Compiler compiler(shaderData);
		auto resources = compiler.get_shader_resources();

		SENGINE_TRACE("Uniform Buffers:");
		for (const auto& resource : resources.uniform_buffers)
		{
			const auto& name = resource.name;
			auto& bufferType = compiler.get_type(resource.base_type_id);
			size_t memberCount = bufferType.member_types.size();
			uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
			uint32_t descriptorSet = compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
			size_t size = compiler.get_declared_struct_size(bufferType);

			ShaderDescriptorSet& shaderDescriptorSet = mShaderDescriptorSets[descriptorSet];
			ASSERT(shaderDescriptorSet.UniformBuffers.find(binding) == shaderDescriptorSet.UniformBuffers.end(), "[SERenderer::VulkanShader] Reflection failed!");
			VulkanUniformBuffer& buffer = shaderDescriptorSet.UniformBuffers[binding];
			buffer.setBindingPoint(binding);
			buffer.setSize(size);
			buffer.setName(name);
			buffer.setStage(stage);

			ShaderBuffer& shaderBuffer = mBuffers[name];
			shaderBuffer.Name = name;
			shaderBuffer.Size = (uint32_t)size;

			for (size_t i = 0; i < memberCount; i++)
			{
				auto type = compiler.get_type(bufferType.member_types[i]);
				const auto& memberName = compiler.get_member_name(bufferType.self, (uint32_t)i);
				auto memberSize = compiler.get_declared_struct_member_size(bufferType, (uint32_t)i);
				auto offset = compiler.type_struct_member_offset(bufferType, (uint32_t)i);

				std::string uniformName = name + "." + memberName;
				shaderBuffer.Uniforms[uniformName] = ShaderUniform(uniformName, SPIRTypeToShaderUniformType(type), (uint32_t)memberSize, offset, binding, descriptorSet);
			}

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
				bufferOffset = mPushConstantRanges.back().Offset + mPushConstantRanges.back().Size;

			auto& pushConstantRange = mPushConstantRanges.emplace_back();
			pushConstantRange.ShaderStage = stage;
			pushConstantRange.Size = (uint32_t)bufferSize;
			pushConstantRange.Offset = bufferOffset;

			// Skip empty push constant buffers - these are for the renderer only
			if (bufferName.empty() || bufferName == "u_Renderer")
				continue;

			ShaderBuffer& buffer = mBuffers[bufferName];
			buffer.Name = bufferName;
			buffer.Size = ((uint32_t)bufferSize) - bufferOffset;

			SENGINE_TRACE("  Name: {0}", bufferName);
			SENGINE_TRACE("  Member Count: {0}", memberCount);
			SENGINE_TRACE("  Size: {0}", bufferSize);

			for (size_t i = 0; i < memberCount; i++)
			{
				auto type = compiler.get_type(bufferType.member_types[i]);
				const auto& memberName = compiler.get_member_name(bufferType.self, (uint32_t)i);
				auto size = compiler.get_declared_struct_member_size(bufferType, (uint32_t)i);
				auto offset = compiler.type_struct_member_offset(bufferType, (uint32_t)i) - bufferOffset;

				std::string uniformName = bufferName + "." + memberName;
				buffer.Uniforms[uniformName] = ShaderUniform(uniformName, SPIRTypeToShaderUniformType(type), (uint32_t)size, offset, 0, 0);
			}
		}

		SENGINE_TRACE("Sampled Images:");
		for (const auto& resource : resources.sampled_images)
		{
			const auto& name = resource.name;
			//auto& type = compiler.get_type(resource.base_type_id);
			uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
			uint32_t descriptorSet = compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
			//uint32_t dimension = (uint32_t)type.image.dim;

			ShaderDescriptorSet& shaderDescriptorSet = mShaderDescriptorSets[descriptorSet];
			auto& imageSampler = shaderDescriptorSet.ImageSamplers[binding];
			imageSampler.BindingPoint = binding;
			imageSampler.DescriptorSet = descriptorSet;
			imageSampler.Name = name;
			imageSampler.ShaderStage = stage;

			mResources[name] = ShaderResourceDeclaration(name, binding, 1);

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

			ShaderDescriptorSet& shaderDescriptorSet = mShaderDescriptorSets[descriptorSet];
			auto& imageSampler = shaderDescriptorSet.StorageImages[binding];
			imageSampler.BindingPoint = binding;
			imageSampler.DescriptorSet = descriptorSet;
			imageSampler.Name = name;
			imageSampler.ShaderStage = stage;

			mResources[name] = ShaderResourceDeclaration(name, binding, 1);

			SENGINE_TRACE("  {0} ({1}, {2})", name, descriptorSet, binding);
		}
		SENGINE_TRACE("===========================");

		//LOG("Storage Buffers:");
		//for (const auto& resource : resources.storage_buffers)
		//{

		//}

		//LOG("Separate Images:");
		//for (const auto& resource : resources.separate_images)
		//{

		//}

		//LOG("Separate Samplers:");
		//for (const auto& resource : resources.separate_samplers)
		//{

		//}
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
		mResources.clear();

		for (auto [stage, data] : shaderData)
		{
			reflect(stage, data);
		}
	}

	void VulkanShader::createDescriptors()
	{
		std::shared_ptr<VulkanDevice> device = std::dynamic_pointer_cast<VulkanDevice>(Renderer::Get()->getGraphicsDevice());
		mTypeCounts.clear();
		for (auto&& [set, shaderDescriptorSet] : mShaderDescriptorSets)
		{
			if (!shaderDescriptorSet.UniformBuffers.empty())
			{
				VkDescriptorPoolSize& typeCount = mTypeCounts[set].emplace_back();
				typeCount.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				typeCount.descriptorCount = (uint32_t)shaderDescriptorSet.UniformBuffers.size();
			}
			if (!shaderDescriptorSet.ImageSamplers.empty())
			{
				VkDescriptorPoolSize& typeCount = mTypeCounts[set].emplace_back();
				typeCount.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				typeCount.descriptorCount = (uint32_t)shaderDescriptorSet.ImageSamplers.size();
			}
			if (!shaderDescriptorSet.StorageImages.empty())
			{
				VkDescriptorPoolSize& typeCount = mTypeCounts[set].emplace_back();
				typeCount.type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
				typeCount.descriptorCount = (uint32_t)shaderDescriptorSet.StorageImages.size();
			}


			std::vector<VkDescriptorSetLayoutBinding> layoutBindings;
			for (auto& [binding, uniformBuffer] : shaderDescriptorSet.UniformBuffers)
			{
				VkDescriptorSetLayoutBinding& layoutBinding = layoutBindings.emplace_back();
				layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				layoutBinding.descriptorCount = 1;
				layoutBinding.stageFlags = uniformBuffer.getStage();
				layoutBinding.pImmutableSamplers = nullptr;
				layoutBinding.binding = binding;

				VkWriteDescriptorSet& descSet = shaderDescriptorSet.WriteDescriptorSets[uniformBuffer.getName()];
				descSet = {};
				descSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				descSet.descriptorType = layoutBinding.descriptorType;
				descSet.descriptorCount = 1;
				descSet.dstBinding = layoutBinding.binding;

				uniformBuffer.allocateBuffer();
			}

			for (auto& [binding, imageSampler] : shaderDescriptorSet.ImageSamplers)
			{
				auto& layoutBinding = layoutBindings.emplace_back();
				layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				layoutBinding.descriptorCount = 1;
				layoutBinding.stageFlags = imageSampler.ShaderStage;
				layoutBinding.pImmutableSamplers = nullptr;
				layoutBinding.binding = binding;

				ASSERT(shaderDescriptorSet.UniformBuffers.find(binding) == shaderDescriptorSet.UniformBuffers.end(), "Binding is already present!");

				VkWriteDescriptorSet& descSet = shaderDescriptorSet.WriteDescriptorSets[imageSampler.Name];
				descSet = {};
				descSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				descSet.descriptorType = layoutBinding.descriptorType;
				descSet.descriptorCount = 1;
				descSet.dstBinding = layoutBinding.binding;
			}

			for (auto& [bindingAndSet, imageSampler] : shaderDescriptorSet.StorageImages)
			{
				auto& layoutBinding = layoutBindings.emplace_back();
				layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
				layoutBinding.descriptorCount = 1;
				layoutBinding.stageFlags = imageSampler.ShaderStage;
				layoutBinding.pImmutableSamplers = nullptr;

				uint32_t binding = bindingAndSet & 0xffffffff;
				layoutBinding.binding = binding;

				ASSERT(shaderDescriptorSet.UniformBuffers.find(binding) == shaderDescriptorSet.UniformBuffers.end(), "Binding is already present!");
				ASSERT(shaderDescriptorSet.ImageSamplers.find(binding) == shaderDescriptorSet.ImageSamplers.end(), "Binding is already present!");

				VkWriteDescriptorSet& descSet = shaderDescriptorSet.WriteDescriptorSets[imageSampler.Name];
				descSet = {};
				descSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				descSet.descriptorType = layoutBinding.descriptorType;
				descSet.descriptorCount = 1;
				descSet.dstBinding = layoutBinding.binding;
			}

			VkDescriptorSetLayoutCreateInfo descriptorLayout = {};
			descriptorLayout.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			descriptorLayout.pNext = nullptr;
			descriptorLayout.bindingCount = (uint32_t)layoutBindings.size();
			descriptorLayout.pBindings = layoutBindings.data();

			SENGINE_TRACE("Creating descriptor set {0} with {1} ubos, {2} samplers and {3} storage images",
				set,
				shaderDescriptorSet.UniformBuffers.size(),
				shaderDescriptorSet.ImageSamplers.size(),
				shaderDescriptorSet.StorageImages.size());
			ASSERT(vkCreateDescriptorSetLayout(device->getLogicalDevice(), &descriptorLayout, nullptr, &mDescriptorSetLayouts[set]) == VK_SUCCESS, "[SERenderer::VulkanShader] failed creation of descriptor set layouts.");
		}

	}

	void* VulkanShader::mapUniformBuffer(uint32_t bindingPoint, uint32_t set)
	{
		ASSERT(mShaderDescriptorSets.find(set) != mShaderDescriptorSets.end(), "[SERenderer::VulkanShader] Could not find uniform buffer!");
		std::shared_ptr<VulkanDevice> device = std::dynamic_pointer_cast<VulkanDevice>(Renderer::Get()->getGraphicsDevice());
		uint8_t* pData;
		ASSERT(vkMapMemory(device->getLogicalDevice(), mShaderDescriptorSets.at(set).UniformBuffers[bindingPoint].getMemory(), 0, mShaderDescriptorSets.at(set).UniformBuffers[bindingPoint].getSize(), 0, (void**)&pData), "");
		return mShaderDescriptorSets.at(set).UniformBuffers[bindingPoint].map();
	}

	void VulkanShader::unmapUniformbuffer(uint32_t bindingPoint, uint32_t set)
	{
		ASSERT(mShaderDescriptorSets.find(set) != mShaderDescriptorSets.end(), "[SERenderer::VulkanShader] Could not find uniform buffer!");
		mShaderDescriptorSets.at(set).UniformBuffers[bindingPoint].unmap();
	}

	std::vector<VkDescriptorSetLayout> VulkanShader::getAllDescriptorSetLayouts()
	{
		std::vector<VkDescriptorSetLayout> result;
		result.reserve(mDescriptorSetLayouts.size());
		for (auto [set, layout] : mDescriptorSetLayouts)
			result.emplace_back(layout);

		return result;
	}

	VulkanShader::ShaderMaterialDescriptorSet VulkanShader::CreateDescriptorSets(uint32_t set /*= 0*/)
	{
		ShaderMaterialDescriptorSet result;
		std::shared_ptr<VulkanDevice> device = std::dynamic_pointer_cast<VulkanDevice>(Renderer::Get()->getGraphicsDevice());

		ASSERT(mTypeCounts.find(set) != mTypeCounts.end(), "");

		// TODO: Move this to the centralized renderer
		VkDescriptorPoolCreateInfo descriptorPoolInfo = {};
		descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptorPoolInfo.pNext = nullptr;
		descriptorPoolInfo.poolSizeCount = (uint32_t)mTypeCounts.at(set).size();
		descriptorPoolInfo.pPoolSizes = mTypeCounts.at(set).data();
		descriptorPoolInfo.maxSets = 1;

		ASSERT(vkCreateDescriptorPool(device->getLogicalDevice(), &descriptorPoolInfo, nullptr, &result.Pool) == VK_SUCCESS, "");

		// Allocate a new descriptor set from the global descriptor pool
		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = result.Pool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &mDescriptorSetLayouts[set];

		result.DescriptorSets.emplace_back();
		ASSERT(vkAllocateDescriptorSets(device->getLogicalDevice(), &allocInfo, result.DescriptorSets.data()) == VK_SUCCESS, "");
		return result;
	}

	VulkanShader::ShaderMaterialDescriptorSet VulkanShader::CreateDescriptorSets(uint32_t set, uint32_t numberOfSets)
	{
		ShaderMaterialDescriptorSet result;
		std::shared_ptr<VulkanDevice> device = std::dynamic_pointer_cast<VulkanDevice>(Renderer::Get()->getGraphicsDevice());

		std::unordered_map<uint32_t, std::vector<VkDescriptorPoolSize>> poolSizes;
		for (auto&& [descSet, shaderDescriptorSet] : mShaderDescriptorSets)
		{
			if (!shaderDescriptorSet.UniformBuffers.empty())
			{
				VkDescriptorPoolSize& typeCount = poolSizes[descSet].emplace_back();
				typeCount.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				typeCount.descriptorCount = (uint32_t)shaderDescriptorSet.UniformBuffers.size() * numberOfSets;
			}
			if (!shaderDescriptorSet.ImageSamplers.empty())
			{
				VkDescriptorPoolSize& typeCount = poolSizes[descSet].emplace_back();
				typeCount.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				typeCount.descriptorCount = (uint32_t)shaderDescriptorSet.ImageSamplers.size() * numberOfSets;
			}
			if (!shaderDescriptorSet.StorageImages.empty())
			{
				VkDescriptorPoolSize& typeCount = poolSizes[descSet].emplace_back();
				typeCount.type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
				typeCount.descriptorCount = (uint32_t)shaderDescriptorSet.StorageImages.size() * numberOfSets;
			}

		}

		ASSERT(poolSizes.find(set) != poolSizes.end(), "");

		// TODO: Move this to the centralized renderer
		VkDescriptorPoolCreateInfo descriptorPoolInfo = {};
		descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptorPoolInfo.pNext = nullptr;
		descriptorPoolInfo.poolSizeCount = (uint32_t)poolSizes.at(set).size();
		descriptorPoolInfo.pPoolSizes = poolSizes.at(set).data();
		descriptorPoolInfo.maxSets = numberOfSets;

		ASSERT(vkCreateDescriptorPool(device->getLogicalDevice(), &descriptorPoolInfo, nullptr, &result.Pool) == VK_SUCCESS, "");

		result.DescriptorSets.resize(numberOfSets);

		for (uint32_t i = 0; i < numberOfSets; i++)
		{
			VkDescriptorSetAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocInfo.descriptorPool = result.Pool;
			allocInfo.descriptorSetCount = 1;
			allocInfo.pSetLayouts = &mDescriptorSetLayouts[set];

			ASSERT(vkAllocateDescriptorSets(device->getLogicalDevice(), &allocInfo, &result.DescriptorSets[i]) == VK_SUCCESS, "");
		}
		return result;
	}

	const VkWriteDescriptorSet* VulkanShader::GetDescriptorSet(const std::string& name, uint32_t set /*= 0*/) const
	{
		ASSERT(mShaderDescriptorSets.find(set) != mShaderDescriptorSets.end(), "");
		if (mShaderDescriptorSets.at(set).WriteDescriptorSets.find(name) == mShaderDescriptorSets.at(set).WriteDescriptorSets.end())
		{
			SENGINE_WARN("Shader {0} does not contain requested descriptor set {1}", mName, name);
			return nullptr;
		}
		return &mShaderDescriptorSets.at(set).WriteDescriptorSets.at(name);
	}

	std::vector<VkPushConstantRange> VulkanShader::GetVKPushConstantRanges() const
	{
		std::vector<VkPushConstantRange> vulkanPushConstantRanges(mPushConstantRanges.size());
		for (uint32_t i = 0; i < mPushConstantRanges.size(); i++)
		{
			const auto& pushConstantRange = mPushConstantRanges[i];
			auto& vulkanPushConstantRange = vulkanPushConstantRanges[i];

			vulkanPushConstantRange.stageFlags = pushConstantRange.ShaderStage;
			vulkanPushConstantRange.offset = pushConstantRange.Offset;
			vulkanPushConstantRange.size = pushConstantRange.Size;
		}
		return vulkanPushConstantRanges;
	}

}
