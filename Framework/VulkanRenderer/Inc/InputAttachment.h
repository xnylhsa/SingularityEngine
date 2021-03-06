//#pragma once
//#include "Common.h"
//#include "VulkanImage.h"
//#include "VulkanImageView.h"
//#include "VulkanMemoryObject.h"
//namespace SingularityEngine::Graphics
//{
//	class InputAttachment
//	{
//	public:
//		struct InputAttachmentParams
//		{
//			VkImageType type;
//			VkFormat format;
//			VkExtent3D size;
//			bool cubemap;
//			VkImageViewType viewType;
//			VkImageUsageFlags usage;
//			VkImageAspectFlags aspect;
//			bool linearFiltering;
//		};
//		InputAttachment(VkPhysicalDevice physicalDevice, VkDevice device, InputAttachmentParams params);
//		bool destroy(VkDevice device);
//		~InputAttachment();
//	private:
//		VulkanMemoryObject* mMemoryObject = nullptr;
//		VulkanImage* mImage = nullptr;
//		VulkanImageView* mImageView = nullptr;
//	};
//}
