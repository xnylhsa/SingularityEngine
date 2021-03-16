#ifndef SINGULARITY_ENGINE_RENDERER_VULKAN_TEXTURE_2D
#define SINGULARITY_ENGINE_RENDERER_VULKAN_TEXTURE_2D
#include "Common.h"
#include "RendererAPI/ITexture.h"


namespace SingularityEngine::SERenderer
{
	class VulkanTexture2D : public ITexture2D
	{
	public:
		VulkanTexture2D(TextureFormat format, uint32_t width, uint32_t height, const void* data, TextureWrapMode wrapMode = TextureWrapMode::Clamp);
		VulkanTexture2D(std::string filePath);
		virtual ~VulkanTexture2D();

		void Invalidate();

		const VkDescriptorImageInfo& GetVulkanDescriptorInfo() const { return mDescriptorImageInfo; }


		void lock() override;
		void unlock() override;
		void resize(uint32_t width, uint32_t height) override;
		Core::Buffer getWriteableBuffer() override;
		bool loaded() const override;
		const std::string& GetPath() const override;
		uint32_t getWidth() const override;
		uint32_t getHeight() const override;
		uint32_t getMipLevelCount() const override;
		TextureFormat getFormat() const override;
		void bind(uint32_t slot = 0) const override;
		uint64_t getHash() const override;
		bool operator ==(const ITexture& other) const override
		{
			return getHash() == other.getHash();
		}

	private:
		void generateMips();

		std::string mAssetPath;
		uint32_t mWidth;
		uint32_t mHeight;
		Core::Buffer mImageData;
		VkDeviceMemory mDeviceMemory;
		VkImage mImage;
		TextureFormat mFormat = TextureFormat::none;

		VkDescriptorImageInfo mDescriptorImageInfo;


	};




}


#endif