#ifndef SINGULARITY_ENGINE_SERENDERER_VULKAN_SHADER
#define SINGULARITY_ENGINE_SERENDERER_VULKAN_SHADER
#include "Common.h"
#include "RendererAPI/IShader.h"
#include "RendererAPI/ShaderUniform.h"
#include "vulkan/Buffers/VulkanUniformBuffer.h"

namespace SingularityEngine::SERenderer
{
	class VulkanUniformBuffer;
	class VulkanShader : public IShader
	{
	public:
		struct ImageSampler
		{
			uint32_t BindingPoint = 0;
			uint32_t DescriptorSet = 0;
			std::string Name;
			VkShaderStageFlagBits ShaderStage = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
		};

		struct PushConstantRange
		{
			VkShaderStageFlagBits ShaderStage = VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
			uint32_t Offset = 0;
			uint32_t Size = 0;
		};
		struct ShaderMaterialDescriptorSet
		{
			VkDescriptorPool Pool;
			std::vector<VkDescriptorSet> DescriptorSets;
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

		VkDescriptorSet getDescriptorSet() { return mDescriptorSet; }
		VkDescriptorSetLayout getDescriptorSetLayout(uint32_t set) { return mDescriptorSetLayouts.at(set); }
		std::vector<VkDescriptorSetLayout> getAllDescriptorSetLayouts();

		VulkanUniformBuffer& GetUniformBuffer(uint32_t binding = 0, uint32_t set = 0) { ASSERT(mShaderDescriptorSets.at(set).UniformBuffers.size() > binding, ""); return mShaderDescriptorSets.at(set).UniformBuffers[binding]; }
		uint32_t GetUniformBufferCount(uint32_t set = 0)
		{
			if (mShaderDescriptorSets.find(set) == mShaderDescriptorSets.end())
				return 0;

			return (uint32_t)mShaderDescriptorSets.at(set).UniformBuffers.size();
		}


		const std::vector<PushConstantRange>& GetPushConstantRanges() const { return mPushConstantRanges; }
		std::vector<VkPushConstantRange> GetVKPushConstantRanges() const;


		ShaderMaterialDescriptorSet CreateDescriptorSets(uint32_t set = 0);
		ShaderMaterialDescriptorSet CreateDescriptorSets(uint32_t set, uint32_t numberOfSets);
		const VkWriteDescriptorSet* GetDescriptorSet(const std::string& name, uint32_t set = 0) const;


		const std::string& getName() const override;
	private:
		struct ShaderDescriptorSet
		{
			std::unordered_map<uint32_t, VulkanUniformBuffer> UniformBuffers;
			std::unordered_map<uint32_t, ImageSampler> ImageSamplers;
			std::unordered_map<uint32_t, ImageSampler> StorageImages;

			std::unordered_map<std::string, VkWriteDescriptorSet> WriteDescriptorSets;
		};
		void preProcess(const std::string& source);
		void compileOrGetVulkanBinary(std::unordered_map<VkShaderStageFlagBits, std::vector<uint32_t>>& outputBinary, bool forceCompile);
		void createAllShaders(const std::unordered_map<VkShaderStageFlagBits, std::vector<uint32_t>>& shaderData);
		void reflectAllShaders(const std::unordered_map<VkShaderStageFlagBits, std::vector<uint32_t>>& shaderData);
		void reflect(VkShaderStageFlagBits stage, std::vector<uint32_t>& shaderData);
		void createDescriptors();
		void cleanup();

		void* mapUniformBuffer(uint32_t bindingPoint, uint32_t set);
		void  unmapUniformbuffer(uint32_t bindingPoint, uint32_t set);
		std::vector<uint32_t> compileVulkanBinaryFromGLSL(VkShaderStageFlagBits stage);
		std::vector<uint32_t> loadBinaryShader(std::string path);
		std::string getCacheShaderFilePath(VkShaderStageFlagBits stageFlag);


		std::string mAssetPath;
		std::string mName;
		std::vector<VkPipelineShaderStageCreateInfo> mShaderStages;
		std::unordered_map<VkShaderStageFlagBits, std::string> mShaderSources;

		std::vector<PushConstantRange> mPushConstantRanges;


		std::unordered_map<std::string, ShaderResourceDeclaration> mResources;
		std::unordered_map<std::string, ShaderBuffer> mBuffers;
		std::unordered_map<uint32_t, ShaderDescriptorSet> mShaderDescriptorSets;
		std::unordered_map<uint32_t, std::vector<VkDescriptorPoolSize>> mTypeCounts;
		std::unordered_map<uint32_t, VkDescriptorSetLayout> mDescriptorSetLayouts;
		VkDescriptorPool mDescriptorPool;
		VkDescriptorSet mDescriptorSet;
	};

}

#endif