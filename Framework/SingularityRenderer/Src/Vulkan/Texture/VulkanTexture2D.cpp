#include "Precompiled.h"
#include "vulkan/Texture/VulkanTexture2D.h"
#include "vulkan/Util/VulkanFunctions.h"
#include "RendererAPI/Renderer.h"
#include "vulkan/device/VulkanDevice.h"
#include "vulkan/Memory/VulkanMemoryAllocator.h"



namespace SingularityEngine::SERenderer
{
	namespace Utils {

		static VkFormat TextureFormatToVkFormat(TextureFormat format)
		{
			switch (format)
			{
			case TextureFormat::RGBA:  return VK_FORMAT_R8G8B8A8_UNORM;
			case TextureFormat::Float16: return VK_FORMAT_R32G32B32A32_SFLOAT;
			}
			ASSERT(false, "[SERenderer::VulkanFormat] could not convert format!");
			return VK_FORMAT_UNDEFINED;
		}

		static uint32_t MipCount(uint32_t width, uint32_t height)
		{
			return (uint32_t)std::floor(std::log2(Math::Min(width, height))) + 1;
		}

		static void InsertImageMemoryBarrier(
			VkCommandBuffer cmdbuffer,
			VkImage image,
			VkAccessFlags srcAccessMask,
			VkAccessFlags dstAccessMask,
			VkImageLayout oldImageLayout,
			VkImageLayout newImageLayout,
			VkPipelineStageFlags srcStageMask,
			VkPipelineStageFlags dstStageMask,
			VkImageSubresourceRange subresourceRange)
		{
			VkImageMemoryBarrier imageMemoryBarrier{};
			imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

			imageMemoryBarrier.srcAccessMask = srcAccessMask;
			imageMemoryBarrier.dstAccessMask = dstAccessMask;
			imageMemoryBarrier.oldLayout = oldImageLayout;
			imageMemoryBarrier.newLayout = newImageLayout;
			imageMemoryBarrier.image = image;
			imageMemoryBarrier.subresourceRange = subresourceRange;

			vkCmdPipelineBarrier(
				cmdbuffer,
				srcStageMask,
				dstStageMask,
				0,
				0, nullptr,
				0, nullptr,
				1, &imageMemoryBarrier);
		}

	}
	void VulkanTexture2D::lock()
	{
	}

	void VulkanTexture2D::unlock()
	{
	}

	void VulkanTexture2D::resize(uint32_t width, uint32_t height)
	{
		UNREFERENCED_PARAMETER(width);
		UNREFERENCED_PARAMETER(height);
	}

	SingularityEngine::Core::Buffer VulkanTexture2D::getWriteableBuffer()
	{
		return mImageData;
	}

	bool VulkanTexture2D::loaded() const
	{
		return true;
	}

	const std::string& VulkanTexture2D::GetPath() const
	{
		return mAssetPath;
	}

	uint32_t VulkanTexture2D::getWidth() const
	{
		return mWidth;
	}

	uint32_t VulkanTexture2D::getHeight() const
	{
		return mHeight;
	}

	uint32_t VulkanTexture2D::getMipLevelCount() const
	{
		return Utils::MipCount(mWidth, mHeight);
	}

	SingularityEngine::SERenderer::TextureFormat VulkanTexture2D::getFormat() const
	{
		return mFormat;
	}

	void VulkanTexture2D::bind(uint32_t slot /*= 0*/) const
	{
		UNREFERENCED_PARAMETER(slot);
	}

	uint64_t VulkanTexture2D::getHash() const
	{
		return (uint64_t)mImage;
	}

	VulkanTexture2D::VulkanTexture2D(TextureFormat format, uint32_t width, uint32_t height, const void* data, TextureWrapMode wrapMode)
	{
		UNREFERENCED_PARAMETER(wrapMode);
		mWidth = width;
		mHeight = height;

		ASSERT(format == TextureFormat::RGBA, "invalid texture format passed into function");
		uint32_t size = width * height * 4;

		mImageData = Core::Buffer::Copy(data, size);
		memcpy(mImageData.Data, data, mImageData.Size);
		Invalidate();
	}

	VulkanTexture2D::VulkanTexture2D(std::string filePath)
	{
		int width, height, channels;
		if (stbi_is_hdr(filePath.c_str()))
		{
			mImageData.Data = (byte*)stbi_loadf(filePath.c_str(), &width, &height, &channels, 4);
			mImageData.Size = width * height * 4 * sizeof(float);
			mFormat = TextureFormat::Float16;
		}
		else
		{
			//stbi_set_flip_vertically_on_load(1);
			mImageData.Data = stbi_load(filePath.c_str(), &width, &height, &channels, 4);
			mImageData.Size = width * height * 4;
			mFormat = TextureFormat::RGBA;
		}

		ASSERT(mImageData.Data, "Failed to load image!");
		mWidth = width;
		mHeight = height;

		ASSERT(mFormat != TextureFormat::none, "Failed to set format");
		Invalidate();
	}

	void VulkanTexture2D::generateMips()
	{
		std::shared_ptr<VulkanDevice> device = std::dynamic_pointer_cast<VulkanDevice>(Renderer::Get()->getGraphicsDevice());

		VkCommandBuffer blitCmd = device->getCommandBuffer(true);

		VkImageMemoryBarrier barrier = {};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.image = mImage;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

		auto mipLevels = getMipLevelCount();
		for (uint32_t i = 1; i < mipLevels; i++)
		{
			VkImageBlit imageBlit{};

			// Source
			imageBlit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageBlit.srcSubresource.layerCount = 1;
			imageBlit.srcSubresource.mipLevel = i - 1;
			imageBlit.srcOffsets[1].x = uint32_t(mWidth >> (i - 1));
			imageBlit.srcOffsets[1].y = uint32_t(mHeight >> (i - 1));
			imageBlit.srcOffsets[1].z = 1;

			// Destination
			imageBlit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageBlit.dstSubresource.layerCount = 1;
			imageBlit.dstSubresource.mipLevel = i;
			imageBlit.dstOffsets[1].x = int32_t(mWidth >> i);
			imageBlit.dstOffsets[1].y = int32_t(mHeight >> i);
			imageBlit.dstOffsets[1].z = 1;

			VkImageSubresourceRange mipSubRange = {};
			mipSubRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			mipSubRange.baseMipLevel = i;
			mipSubRange.levelCount = 1;
			mipSubRange.layerCount = 1;

			// Prepare current mip level as image blit destination
			Utils::InsertImageMemoryBarrier(blitCmd, mImage,
				0, VK_ACCESS_TRANSFER_WRITE_BIT,
				VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
				mipSubRange);

			// Blit from previous level
			vkCmdBlitImage(
				blitCmd,
				mImage,
				VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				mImage,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				1,
				&imageBlit,
				VK_FILTER_LINEAR);

			// Prepare current mip level as image blit source for next level
			Utils::InsertImageMemoryBarrier(blitCmd, mImage,
				VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
				mipSubRange);
		}

		// After the loop, all mip layers are in TRANSFER_SRC layout, so transition all to SHADER_READ
		VkImageSubresourceRange subresourceRange = {};
		subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		subresourceRange.layerCount = 1;
		subresourceRange.levelCount = mipLevels;

		Utils::InsertImageMemoryBarrier(blitCmd, mImage,
			VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_SHADER_READ_BIT,
			VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
			subresourceRange);

		device->submit(blitCmd);
	}

	void VulkanTexture2D::Invalidate()
	{
		std::shared_ptr<VulkanDevice> device = std::dynamic_pointer_cast<VulkanDevice>(Renderer::Get()->getGraphicsDevice());
		auto vulkanDevice = device->getLogicalDevice();
		VkDeviceSize size = mImageData.Size;

		VkFormat format = Utils::TextureFormatToVkFormat(mFormat);
		uint32_t mipCount = getMipLevelCount();

		VkMemoryAllocateInfo memAllocInfo{};
		memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingMemory;

		VulkanMemoryAllocator allocator("Texture2D");

		VkBufferCreateInfo bufferCreateInfo{};
		bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferCreateInfo.size = size;
		// This buffer is used as a transfer source for the buffer copy
		bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		ASSERT(vkCreateBuffer(vulkanDevice, &bufferCreateInfo, nullptr, &stagingBuffer) == VK_SUCCESS, "[SERenderer::Texture2D] failed to create image buffer!");

		VkMemoryRequirements memoryRequirements = {};
		vkGetBufferMemoryRequirements(vulkanDevice, stagingBuffer, &memoryRequirements);
		allocator.allocate(memoryRequirements, &stagingMemory, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		ASSERT(vkBindBufferMemory(vulkanDevice, stagingBuffer, stagingMemory, 0) == VK_SUCCESS, "[SERenderer::Texture2D] failed to bind image buffer!");

		// Copy texture data into host local staging buffer
		uint8_t* destData;
		ASSERT(vkMapMemory(vulkanDevice, stagingMemory, 0, memoryRequirements.size, 0, (void**)&destData) == VK_SUCCESS, "[SERenderer::Texture2D] failed to bind image buffer!");
		memcpy(destData, mImageData.Data, size);
		vkUnmapMemory(vulkanDevice, stagingMemory);

		// Create optimal tiled target image on the device
		VkImageCreateInfo imageCreateInfo{};
		imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
		imageCreateInfo.format = format;
		imageCreateInfo.mipLevels = mipCount;
		imageCreateInfo.arrayLayers = 1;
		imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageCreateInfo.extent = { mWidth, mHeight, 1 };
		imageCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		ASSERT(vkCreateImage(vulkanDevice, &imageCreateInfo, nullptr, &mImage) == VK_SUCCESS, "[SERenderer::Texture2D] failed to bind image buffer!");

		vkGetImageMemoryRequirements(vulkanDevice, mImage, &memoryRequirements);
		allocator.allocate(memoryRequirements, &mDeviceMemory, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		ASSERT(vkBindImageMemory(vulkanDevice, mImage, mDeviceMemory, 0) == VK_SUCCESS, "[SERenderer::Texture2D] failed to bind image buffer!");

		VkCommandBuffer copyCmd = device->getCommandBuffer(true);

		// Image memory barriers for the texture image

		// The sub resource range describes the regions of the image that will be transitioned using the memory barriers below
		VkImageSubresourceRange subresourceRange = {};
		// Image only contains color data
		subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		// Start at first mip level
		subresourceRange.baseMipLevel = 0;
		subresourceRange.levelCount = 1;
		subresourceRange.layerCount = 1;

		// Transition the texture image layout to transfer target, so we can safely copy our buffer data to it.
		VkImageMemoryBarrier imageMemoryBarrier{};
		imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageMemoryBarrier.image = mImage;
		imageMemoryBarrier.subresourceRange = subresourceRange;
		imageMemoryBarrier.srcAccessMask = 0;
		imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

		// Insert a memory dependency at the proper pipeline stages that will execute the image layout transition 
		// Source pipeline stage is host write/read exection (VK_PIPELINE_STAGE_HOST_BIT)
		// Destination pipeline stage is copy command exection (VK_PIPELINE_STAGE_TRANSFER_BIT)
		vkCmdPipelineBarrier(
			copyCmd,
			VK_PIPELINE_STAGE_HOST_BIT,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			0,
			0, nullptr,
			0, nullptr,
			1, &imageMemoryBarrier);

		VkBufferImageCopy bufferCopyRegion = {};
		bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		bufferCopyRegion.imageSubresource.mipLevel = 0;
		bufferCopyRegion.imageSubresource.baseArrayLayer = 0;
		bufferCopyRegion.imageSubresource.layerCount = 1;
		bufferCopyRegion.imageExtent.width = mWidth;
		bufferCopyRegion.imageExtent.height = mHeight;
		bufferCopyRegion.imageExtent.depth = 1;
		bufferCopyRegion.bufferOffset = 0;

		// Copy mip levels from staging buffer
		vkCmdCopyBufferToImage(
			copyCmd,
			stagingBuffer,
			mImage,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1,
			&bufferCopyRegion);

		Utils::InsertImageMemoryBarrier(copyCmd, mImage,
			VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
			subresourceRange);

		device->submit(copyCmd);

		// Clean up staging resources
		vkFreeMemory(vulkanDevice, stagingMemory, nullptr);
		vkDestroyBuffer(vulkanDevice, stagingBuffer, nullptr);


		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// CREATE TEXTURE SAMPLER
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Create a texture sampler
		VkSamplerCreateInfo sampler{};
		sampler.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		sampler.maxAnisotropy = 1.0f;
		sampler.magFilter = VK_FILTER_LINEAR;
		sampler.minFilter = VK_FILTER_LINEAR;
		sampler.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		sampler.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		sampler.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		sampler.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		sampler.mipLodBias = 0.0f;
		sampler.compareOp = VK_COMPARE_OP_NEVER;
		sampler.minLod = 0.0f;
		sampler.maxLod = (float)mipCount;
		// Enable anisotropic filtering
		// This feature is optional, so we must check if it's supported on the device

		// TODO:
		/*if (vulkanDevice->features.samplerAnisotropy) {
			// Use max. level of anisotropy for this example
			sampler.maxAnisotropy = 1.0f;// vulkanDevice->properties.limits.maxSamplerAnisotropy;
			sampler.anisotropyEnable = VK_TRUE;
		}
		else {
			// The device does not support anisotropic filtering
			sampler.maxAnisotropy = 1.0;
			sampler.anisotropyEnable = VK_FALSE;
		}*/
		sampler.maxAnisotropy = 1.0;
		sampler.anisotropyEnable = VK_FALSE;
		sampler.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
		ASSERT(vkCreateSampler(vulkanDevice, &sampler, nullptr, &mDescriptorImageInfo.sampler) == VK_SUCCESS, "[SERenderer::Texture2D] failed to bind image buffer!");

		// Create image view
		// Textures are not directly accessed by the shaders and
		// are abstracted by image views containing additional
		// information and sub resource ranges
		VkImageViewCreateInfo view{};
		view.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		view.viewType = VK_IMAGE_VIEW_TYPE_2D;
		view.format = format;
		view.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
		// The subresource range describes the set of mip levels (and array layers) that can be accessed through this image view
		// It's possible to create multiple image views for a single image referring to different (and/or overlapping) ranges of the image
		view.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		view.subresourceRange.baseMipLevel = 0;
		view.subresourceRange.baseArrayLayer = 0;
		view.subresourceRange.layerCount = 1;
		view.subresourceRange.levelCount = mipCount;
		view.image = mImage;
		ASSERT(vkCreateImageView(vulkanDevice, &view, nullptr, &mDescriptorImageInfo.imageView) == VK_SUCCESS, "[SERenderer::Texture2D] failed to bind image buffer!");

		generateMips();

		// Store current layout for later reuse
		mDescriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	}

}
