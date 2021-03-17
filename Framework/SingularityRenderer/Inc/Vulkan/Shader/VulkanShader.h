#ifndef SINGULARITY_ENGINE_SERENDERER_VULKAN_SHADER
#define SINGULARITY_ENGINE_SERENDERER_VULKAN_SHADER
#include "Common.h"
#include "RendererAPI/IShader.h"
//#include "RendererAPI/ShaderUniform.h"
//#include "vulkan/Buffers/VulkanUniformBuffer.h"
#include "vulkan/DescriptorSets/VulkanDescriptorSet.h"


namespace SingularityEngine::SERenderer
{
	class VulkanUniformBuffer;
	class VulkanShader : public IShader
	{
	public:
		struct ReflectedBinding
		{
			uint32_t set;
			uint32_t binding;
			uint32_t type;
		};
		VulkanShader(std::string filePath);
		~VulkanShader();
		void reload(bool forceCompile) override;

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

		const std::vector<VkPipelineShaderStageCreateInfo>& getShaderStages();

		const std::array<VkDescriptorSetLayout, VULKAN_NUM_SETS>& getAllDescriptorSetLayouts() const { return mDescriptorSetLayouts; }
		std::array<VkDescriptorSetLayout, VULKAN_NUM_SETS>& getAllDescriptorSetLayouts() { return mDescriptorSetLayouts; }
		std::vector<VkPushConstantRange>& getPushConstantRanges() { return mPushConstantRanges; };
		const std::vector<VkPushConstantRange>& getPushConstantRanges() const { return mPushConstantRanges; }
		const std::string& getName() const override;
	private:
		struct DescriptorSetLayoutData {
			uint32_t setNumber;
			VkDescriptorSetLayoutCreateInfo createInfo;
			std::vector<VkDescriptorSetLayoutBinding> bindings;
		};
		void preProcess(const std::string& source);
		void compileOrGetVulkanBinary(std::unordered_map<VkShaderStageFlagBits, std::vector<uint32_t>>& outputBinary, bool forceCompile);
		void createAllShaders(const std::unordered_map<VkShaderStageFlagBits, std::vector<uint32_t>>& shaderData);
		void reflectAllShaders(const std::unordered_map<VkShaderStageFlagBits, std::vector<uint32_t>>& shaderData);
		DescriptorSetLayoutData reflect(VkShaderStageFlagBits stage, std::vector<uint32_t>& shaderData);
		void cleanup();

		std::vector<uint32_t> compileVulkanBinaryFromGLSL(VkShaderStageFlagBits stage);
		std::vector<uint32_t> loadBinaryShader(std::string path);
		std::string getCacheShaderFilePath(VkShaderStageFlagBits stageFlag);


		std::string mAssetPath;
		std::string mName;
		std::vector<VkPipelineShaderStageCreateInfo> mShaderStages;
		std::vector<VkPushConstantRange> mPushConstantRanges;
		std::unordered_map<VkShaderStageFlagBits, std::string> mShaderSources;
		std::unordered_map<std::string, ReflectedBinding> mBindings;
		std::array<VkDescriptorSetLayout, VULKAN_NUM_SETS> mDescriptorSetLayouts;
		std::array<uint32_t, VULKAN_NUM_SETS> mSetHashes;
	};

}

#endif