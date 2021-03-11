#ifndef SINGULARITY_ENGINE_SERENDERER_VULKAN_SHADER
#define SINGULARITY_ENGINE_SERENDERER_VULKAN_SHADER
#include "Common.h"
#include "RendererAPI/IShader.h"

namespace SingularityEngine::SERenderer
{
	class VulkanShader : public IShader
	{
	public:
		VulkanShader(std::string filePath);
		~VulkanShader();
		void reload() override;


		void bind() override;


		void unbind() const override;

		void setUniformBuffer(const std::string& name, const void* data, uint32_t size) override;
		void setUniform(const std::string& fullname, float value) override;
		void setUniform(const std::string& fullname, int value) override;
		void setUniform(const std::string& fullname, Math::Vector2 value) override;
		void setUniform(const std::string& fullname, Math::Vector3 value) override;
		void setUniform(const std::string& fullname, Math::Vector4 value) override;
		void setUniform(const std::string& fullname, Math::Matrix3x3 value) override;
		void setUniform(const std::string& fullname, Math::Matrix4x4 value) override;

		void setInt(const std::string& name, int value) override;
		void setIntArray(const std::string& name, int* values, uint32_t count) override;
		void setFloat(const std::string& name, float value) override;
		void setFloat2(const std::string& name, const Math::Vector2& value) override;
		void setFloat3(const std::string& name, const Math::Vector2& value) override;
		void setFloat4(const std::string& name, const Math::Vector2& value) override;
		void setMat4(const std::string& name, const Math::Matrix4x4& value) override;

		const std::vector<VkPipelineShaderStageCreateInfo>& getShaderStages();
		const std::string& getName() const override;
	private:
		void loadAndCreateVertexShader(VkPipelineShaderStageCreateInfo& shaderPipelineInfo);
		void loadAndCreateFragShader(VkPipelineShaderStageCreateInfo& shaderPipelineInfo);
		void convertHLSLtoSPIRV(std::string filePath, std::string outputPath, std::string targetProfile, std::string entryPoint);
		//void loadAndCreateComputeShader();
		//void loadAndCreateTesselationShader();
		//void loadAndCreateRaytracingShaders();
		std::vector<uint32_t> loadBinaryShader(std::string path);
		std::string getCacheShaderFilePath(std::string shaderType);
		std::string mFilePath;
		std::string mName;
		std::vector<VkPipelineShaderStageCreateInfo> mShaderStages;
	};

}

#endif