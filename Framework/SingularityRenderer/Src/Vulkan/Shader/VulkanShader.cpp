#include "Precompiled.h"
#include "Vulkan/Shader/VulkanShader.h"
#include "RendererAPI/Renderer.h"
#include "vulkan/Device/VulkanDevice.h"
#include "vulkan/Util/VulkanFunctions.h"
#include "External/shaderc/libshaderc/include/shaderc/shaderc.hpp"
#include <fstream>
#include <string>
namespace SingularityEngine::SERenderer
{
	VulkanShader::VulkanShader(std::string filePath) : mAssetPath(filePath)
	{
		//Fragment Shader
		std::filesystem::path p = mAssetPath;
		mName = p.filename().stem().string();
	}

	VulkanShader::~VulkanShader()
	{
		cleanup();

	}

	void VulkanShader::reload()
	{
		cleanup();
		mShaderStages.resize(2);
		//Load Vertex Shader
		std::array<std::vector<uint32_t>, 2> compiledSPIRVShaders;
		compileOrGetVulkanBinary(compiledSPIRVShaders);
		createVertexShader(mShaderStages[0], compiledSPIRVShaders[0]);
		createFragShader(mShaderStages[1], compiledSPIRVShaders[1]);
	}



	const std::string& VulkanShader::getName() const
	{
		return mName;
	}

	void VulkanShader::setMat4(const std::string& name, const Math::Matrix4x4& value)
	{
		UNREFERENCED_PARAMETER(name);
		UNREFERENCED_PARAMETER(value);

	}

	void VulkanShader::setFloat4(const std::string& name, const Math::Vector2& value)
	{
		UNREFERENCED_PARAMETER(name);
		UNREFERENCED_PARAMETER(value);
	}

	void VulkanShader::setFloat3(const std::string& name, const Math::Vector2& value)
	{
		UNREFERENCED_PARAMETER(name);
		UNREFERENCED_PARAMETER(value);
	}

	void VulkanShader::setFloat2(const std::string& name, const Math::Vector2& value)
	{
		UNREFERENCED_PARAMETER(name);
		UNREFERENCED_PARAMETER(value);
	}

	void VulkanShader::setFloat(const std::string& name, float value)
	{
		UNREFERENCED_PARAMETER(name);
		UNREFERENCED_PARAMETER(value);
	}

	void VulkanShader::setIntArray(const std::string& name, int* values, uint32_t count)
	{
		UNREFERENCED_PARAMETER(name);
		UNREFERENCED_PARAMETER(values);
		UNREFERENCED_PARAMETER(count);
	}

	void VulkanShader::setInt(const std::string& name, int value)
	{
		UNREFERENCED_PARAMETER(name);
		UNREFERENCED_PARAMETER(value);
	}

	void VulkanShader::setUniform(const std::string& fullname, Math::Matrix4x4 value)
	{
		UNREFERENCED_PARAMETER(fullname);
		UNREFERENCED_PARAMETER(value);
	}

	void VulkanShader::setUniform(const std::string& fullname, Math::Matrix3x3 value)
	{
		UNREFERENCED_PARAMETER(fullname);
		UNREFERENCED_PARAMETER(value);
	}

	void VulkanShader::setUniform(const std::string& fullname, Math::Vector4 value)
	{
		UNREFERENCED_PARAMETER(fullname);
		UNREFERENCED_PARAMETER(value);
	}

	void VulkanShader::setUniform(const std::string& fullname, Math::Vector3 value)
	{
		UNREFERENCED_PARAMETER(fullname);
		UNREFERENCED_PARAMETER(value);
	}

	void VulkanShader::setUniform(const std::string& fullname, Math::Vector2 value)
	{
		UNREFERENCED_PARAMETER(fullname);
		UNREFERENCED_PARAMETER(value);
	}

	void VulkanShader::setUniform(const std::string& fullname, int value)
	{
		UNREFERENCED_PARAMETER(fullname);
		UNREFERENCED_PARAMETER(value);
	}

	void VulkanShader::setUniform(const std::string& fullname, float value)
	{
		UNREFERENCED_PARAMETER(fullname);
		UNREFERENCED_PARAMETER(value);
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

	void VulkanShader::createVertexShader(VkPipelineShaderStageCreateInfo& shaderStageInfo, const std::vector<uint32_t>& shaderData)
	{
		VkShaderModuleCreateInfo moduleCreateInfo{};
		moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		moduleCreateInfo.codeSize = shaderData.size() * sizeof(uint32_t);
		moduleCreateInfo.pCode = shaderData.data();
		std::shared_ptr<VulkanDevice> device = std::dynamic_pointer_cast<VulkanDevice>(Renderer::Get()->getGraphicsDevice());
		VkShaderModule shaderModule;
		ASSERT(vkCreateShaderModule(device->getLogicalDevice(), &moduleCreateInfo, nullptr, &shaderModule) == VK_SUCCESS, "[SERenderer::VulkanShader] Failed to create shader module!");

		shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		shaderStageInfo.module = shaderModule;
		shaderStageInfo.pName = "main";

	}

	void VulkanShader::createFragShader(VkPipelineShaderStageCreateInfo& shaderStageInfo, const std::vector<uint32_t>& shaderData)
	{
		VkShaderModuleCreateInfo moduleCreateInfo{};
		moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		moduleCreateInfo.codeSize = shaderData.size() * sizeof(uint32_t);
		moduleCreateInfo.pCode = shaderData.data();
		std::shared_ptr<VulkanDevice> device = std::dynamic_pointer_cast<VulkanDevice>(Renderer::Get()->getGraphicsDevice());
		VkShaderModule shaderModule;
		ASSERT(vkCreateShaderModule(device->getLogicalDevice(), &moduleCreateInfo, nullptr, &shaderModule) == VK_SUCCESS,"[SERenderer::VulkanShader] Failed to create shader module!");

		shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shaderStageInfo.module = shaderModule;
		shaderStageInfo.pName = "main";
	}

	std::vector<uint32_t> VulkanShader::loadBinaryShader(std::string path)
	{
		std::vector<uint32_t> binaryShader;
		FILE* f;
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

			LOG("[SERenderer::VulkanShader] Failed to load shader cache %s at file location: %s", mName.c_str(), mAssetPath.c_str());
		}
		return binaryShader;
	}

	std::string VulkanShader::getCacheShaderFilePath(std::string shaderType)
	{
		//Fragment Shader
		std::filesystem::path p = mAssetPath;
		auto path = p.parent_path() / "cached" / (p.filename().string() + ".cached_vulkan." + shaderType);
		return path.string();
	}


	const std::vector<VkPipelineShaderStageCreateInfo>& VulkanShader::getShaderStages()
	{
		return mShaderStages;
	}

	void VulkanShader::compileOrGetVulkanBinary(std::array<std::vector<uint32_t>, 2>& outputBinary, bool forceCompile)
	{
		if (!forceCompile)
		{
			outputBinary[0] = loadBinaryShader(getCacheShaderFilePath("vertex"));
			outputBinary[1] = loadBinaryShader(getCacheShaderFilePath("fragment"));
		}
		if (forceCompile || (outputBinary[0].empty() || outputBinary[1].empty()))
		{
			if (outputBinary[0].empty() || forceCompile)
			{
				outputBinary[0] = compileVulkanBinaryFromGLSL("vertex");
			}
			if (outputBinary[1].empty() || forceCompile)
			{
				outputBinary[1] = compileVulkanBinaryFromGLSL("fragment");
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

	VkShaderStageFlagBits VulkanShader::shaderTypeFromString(std::string type)
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
	int VulkanShader::shadercTypeFromString(std::string type)
	{
		if (type.compare("vertex") == 0)
		{
			return shaderc_vertex_shader;
		}
		if (type.compare("fragment") == 0 || type.compare("pixel"))
		{
			return shaderc_fragment_shader;
		}
		if (type.compare("compute") == 0)
		{
			return shaderc_compute_shader;
		}
		if (type.compare("geometry") == 0)
		{
			return shaderc_geometry_shader;
		}
		return VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
	}
	//TODO: add a second version of this for trans (TRANS RIGHTS!) compilation of HLSL to SPIRV
	std::vector<uint32_t> VulkanShader::compileVulkanBinaryFromGLSL(std::string shaderType) 
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
			auto& shaderSource = mShaderSources.at(shaderTypeFromString(shaderType));
			shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(shaderSource, (shaderc_shader_kind)shadercTypeFromString(shaderType), mAssetPath.c_str(), options);
			if (module.GetCompilationStatus() != shaderc_compilation_status_success)
			{
				ASSERT(false, "%s", module.GetErrorMessage().c_str());
			}

			const uint8_t* begin = (const uint8_t*)module.begin();
			const uint8_t* end = (const uint8_t*)module.end();
			const ptrdiff_t size = end - begin;

			compiledShader = std::vector<uint32_t>(module.cbegin(), module.cend());
		}

		{

			std::string cachedFilePath = getCacheShaderFilePath(shaderType);
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

}
