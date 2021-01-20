#pragma once
#include "Common.h"
#include "Image.h"
#include "ImageView.h"
#include "MemoryObject.h"
namespace SingularityEngine::Vulkan
{
	class InputAttachment
	{
	public:
		struct InputAttachmentParams
		{
			VkImageType type;
			VkFormat format;
			VkExtent3D size;
			bool cubemap;
			VkImageViewType viewType;
			VkImageUsageFlags usage;
			VkImageAspectFlags aspect;
			bool linearFiltering;
		};
		InputAttachment(VkPhysicalDevice physicalDevice, VkDevice device, InputAttachmentParams params);
		bool destroy(VkDevice device);
		~InputAttachment();
	private:
		MemoryObject* mMemoryObject = nullptr;
		Image* mImage = nullptr;
		ImageView* mImageView = nullptr;
	};
}
