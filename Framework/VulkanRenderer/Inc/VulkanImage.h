#pragma once


namespace  SingularityEngine::Graphics
{
	class VulkanImage
	{
	public:
		VulkanImage(VkImageType type, VkFormat format, VkExtent3D size, uint32_t numMipmaps, uint32_t layers, VkSampleCountFlagBits samples, VkImageUsageFlags usageScenarios, bool isCubemap);
		VulkanImage(VkImageType type, VkFormat format, VkExtent3D size, uint32_t numMipmaps, uint32_t layers, VkSampleCountFlagBits samples, VkImageTiling tiling, VkImageUsageFlags usageScenarios, bool isCubemap);

		~VulkanImage();
		operator VkImage() { return mImage; }
		VkImageType& getImageType() { return mImageType; }
		VkFormat& getFormat() { return mFormat; }
		VkExtent3D& getImageSize() { return mExtent; }
		void copyDataFromBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImageLayout layout, std::vector<VkBufferImageCopy> regions);
		void copyDataToBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkImageLayout layout, std::vector<VkBufferImageCopy> regions);

		bool isValid() { return mImage != VK_NULL_HANDLE; } 
	private:
		VkImageCreateInfo createInfo();
		bool destroy();
		VkSampleCountFlagBits mSamples;
		VkImageUsageFlags mUsageFlags;
		uint32_t mNumberOfMipmaps;
		uint32_t mNumberOfLayers;
		VkFormat mFormat;
		VkExtent3D mExtent;
		VkImageType mImageType;
		VkImageTiling mTiling;
		VkImage mImage = VK_NULL_HANDLE;
		bool mIsCubemap = false;
	};
}