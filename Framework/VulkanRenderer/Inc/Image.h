#pragma once


namespace  SingularityEngine::Vulkan
{
	class Image
	{
	public:
		Image(VkDevice device, VkImageType type, VkFormat format, VkExtent3D size, uint32_t numMipmaps, uint32_t layers, VkSampleCountFlagBits samples, VkImageUsageFlags usageScenarios, bool isCubemap);
		~Image();
		VkImage& getImage() { return mImage; }
		VkImageType& getImageType() { return mImageType; }
		VkFormat& getFormat() { return mFormat; }
		VkExtent3D& getImageSize() { return mExtent; }
		void copyDataFromBuffer(VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkImageLayout layout, std::vector<VkBufferImageCopy> regions);
		void copyDataToBuffer(VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkImageLayout layout, std::vector<VkBufferImageCopy> regions);

		bool isValid() { return mImage != VK_NULL_HANDLE; } 
		bool destroy(VkDevice device);
	private:
		VkImageCreateInfo createInfo(VkDevice device);
		VkSampleCountFlagBits mSamples;
		VkImageUsageFlags mUsageFlags;
		uint32_t mNumberOfMipmaps;
		uint32_t mNumberOfLayers;
		VkFormat mFormat;
		VkExtent3D mExtent;
		VkImageType mImageType;
		VkImage mImage = VK_NULL_HANDLE;
		bool mIsCubemap = false;
	};
}