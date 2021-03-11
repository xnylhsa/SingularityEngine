#include "Precompiled.h"
#include "Vulkan/Shader/VulkanShader.h"
#include "RendererAPI/Renderer.h"
#include "vulkan/Device/VulkanDevice.h"
#include "vulkan/Util/VulkanFunctions.h"
#include "External/shaderc/libshaderc/include/shaderc/shaderc.hpp"
namespace SingularityEngine::SERenderer
{
	VulkanShader::VulkanShader(std::string filePath) : mFilePath(filePath)
	{
		//Fragment Shader
		std::filesystem::path p = mFilePath;
		mName = p.filename().stem().string();
	}

	VulkanShader::~VulkanShader()
	{

	}

	void VulkanShader::reload()
	{

		if (!mShaderStages.empty())
		{
			std::shared_ptr<VulkanDevice> device = std::dynamic_pointer_cast<VulkanDevice>(Renderer::Get()->getGraphicsDevice());
			for (auto& shaderPipelineInfo : mShaderStages)
			{
				vkDestroyShaderModule(device->getLogicalDevice(), shaderPipelineInfo.module, nullptr);
			}
		}
		mShaderStages.resize(2);
		//Load Vertex Shader
		loadAndCreateVertexShader(mShaderStages[0]);
		loadAndCreateFragShader(mShaderStages[1]);

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

	void VulkanShader::loadAndCreateVertexShader(VkPipelineShaderStageCreateInfo& shaderStageInfo)
	{
		std::vector<uint32_t> shaderData = loadBinaryShader(getCacheShaderFilePath("vert"));
		if (shaderData.empty())
		{
			std::filesystem::path p = mFilePath;
			if (p.has_extension() && p.extension() == ".hlsl")
			{
				shaderc::Compiler compiler;
				shaderc::CompileOptions options;
				//convertHLSLtoSPIRV(mFilePath, getCacheShaderFilePath("vert"), "vs_6_5", "VSMain");
				//shaderData = loadBinaryShader(getCacheShaderFilePath("vert"));
				LOG("[SERenderer::VulkanShader] HLSL to SPIRV conversion failed, HLSL shader was improperly setup for conversion!");
			}
			ASSERT(!shaderData.empty(), "[SERenderer::VulkanShader] unable to convert shader and cache it!");
			return;
		}
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

	void VulkanShader::loadAndCreateFragShader(VkPipelineShaderStageCreateInfo& shaderStageInfo)
	{
		std::vector<uint32_t> shaderData = loadBinaryShader(getCacheShaderFilePath("frag"));
		if (shaderData.empty())
		{
			std::filesystem::path p = mFilePath;
			if (p.has_extension() && p.extension() == ".hlsl")
			{
				//convertHLSLtoSPIRV(mFilePath, getCacheShaderFilePath("frag"), "ps_6_5", "PSMain");
				//shaderData = loadBinaryShader(getCacheShaderFilePath("frag"));
				LOG("[SERenderer::VulkanShader] HLSL to SPIRV conversion failed, HLSL shader was improperly setup for conversion!");
			}
			if (shaderData.empty())
			{
				LOG("[SERenderer::VulkanShader] unable to convert shader and cache it!");
				return;
			}
		}

		//get device

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
			fclose(f);
		}
		else
		{
			LOG("[SERenderer::VulkanShader] Failed to load shader cache %s at file location: %s", mName.c_str(), mFilePath.c_str());
		}
		return std::move(binaryShader);
	}

	std::string VulkanShader::getCacheShaderFilePath(std::string shaderType)
	{
		//Fragment Shader
		std::filesystem::path p = mFilePath;
		auto path = p.parent_path() / "cached" / (p.filename().string() + ".cached_vulkan." + shaderType);
		return path.string();
	}


	const std::vector<VkPipelineShaderStageCreateInfo>& VulkanShader::getShaderStages()
	{
		return mShaderStages;
	}

}
