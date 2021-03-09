#include "Precompiled.h"
#include "Vulkan/TextureUniformBufferDescriptor.h"
#include "Vulkan/DescriptorInfos.h"
#include "Vulkan/Context/VulkanContext.h"
using namespace SingularityEngine::SERenderer;

bool TextureUniformBufferDescriptor::create(VkPhysicalDevice physicalDevice, VkDevice device, VkExtent3D size, VkDeviceSize uniformBufferSize)
{
	CombinedImageSampler::CombinedImageSamplerParams samplerParams;
	samplerParams.samplerParams.magFilter = VK_FILTER_LINEAR;
	samplerParams.samplerParams.minFilter = VK_FILTER_LINEAR;
	samplerParams.samplerParams.uAddressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerParams.samplerParams.vAddressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerParams.samplerParams.wAddressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerParams.samplerParams.compareOperator = VK_COMPARE_OP_ALWAYS;
	samplerParams.samplerParams.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
	samplerParams.samplerParams.lodBias = 0.0f;
	samplerParams.samplerParams.maxLOD = 0.0f;
	samplerParams.samplerParams.minLOD = 0.0f;
	samplerParams.samplerParams.anisotropyEnable = false;
	samplerParams.samplerParams.maxAnisotropy = 1.0f;
	samplerParams.samplerParams.compareEnable = false;


	samplerParams.sampleImageParams.type = VK_IMAGE_TYPE_2D;
	samplerParams.sampleImageParams.format = VK_FORMAT_R8G8B8A8_UNORM;
	samplerParams.sampleImageParams.viewType = VK_IMAGE_VIEW_TYPE_2D;
	samplerParams.sampleImageParams.aspect = VK_IMAGE_ASPECT_COLOR_BIT;
	samplerParams.sampleImageParams.linearFiltering = VK_FILTER_LINEAR;
	samplerParams.sampleImageParams.numLayers = 1;
	samplerParams.sampleImageParams.numMipMaps = 1;
	samplerParams.sampleImageParams.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT;
	samplerParams.sampleImageParams.cubemap = false;
	samplerParams.sampleImageParams.size = size;
	mCombinedImageSampler = new CombinedImageSampler(physicalDevice, device, samplerParams);
	mUniformBuffer = new UniformBuffer(physicalDevice, device, uniformBufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT);
	std::vector<VkDescriptorSetLayoutBinding> bindings = {
	 {
	   0,                                                          // uint32_t             binding
	   VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,                  // VkDescriptorType     descriptorType
	   1,                                                          // uint32_t             descriptorCount
	   VK_SHADER_STAGE_FRAGMENT_BIT,                               // VkShaderStageFlags   stageFlags
	   nullptr                                                     // const VkSampler    * pImmutableSamplers
	 },
	 {
	   1,                                                          // uint32_t             binding
	   VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,                          // VkDescriptorType     descriptorType
	   1,                                                          // uint32_t             descriptorCount
	   VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,  // VkShaderStageFlags   stageFlags
	   nullptr                                                     // const VkSampler    * pImmutableSamplers
	 }
	};
	mDescriptorSetLayouts = new DescriptorSetLayout(device, bindings);
	std::vector<VkDescriptorPoolSize> descriptor_types = {
	{
		VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,                  // VkDescriptorType     type
		1                                                           // uint32_t             descriptorCount
	},
	{
		VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,                          // VkDescriptorType     type
		1                                                           // uint32_t             descriptorCount
	}
	};
	mDescriptorPool = new DescriptorPool(device, descriptor_types, 1, false);
	mDescriptorSets = new DescriptorSet(device, mDescriptorPool, { mDescriptorSetLayouts->get() });
	std::vector<Descriptors::ImageDescriptorInfo> image_descriptor_infos = {
   {
	 mDescriptorSets->get(0),                         // VkDescriptorSet                      TargetDescriptorSet
	 0,                                          // uint32_t                             TargetDescriptorBinding
	 0,                                          // uint32_t                             TargetArrayElement
	 VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,  // VkDescriptorType                     TargetDescriptorType
	 {                                           // std::vector<VkDescriptorImageInfo>   ImageInfos
	   {
		 mCombinedImageSampler->getSampler(),                                  // VkSampler                            sampler
		 mCombinedImageSampler->getImageView(),                       // VkImageView                          imageView
		 VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL  // VkImageLayout                        imageLayout
	   }
	 }
   }
	};

	std::vector<Descriptors::BufferDescriptorInfo> buffer_descriptor_infos = {
	  {
		 mDescriptorSets->get(0),                         // VkDescriptorSet                      TargetDescriptorSet
		1,                                          // uint32_t                             TargetDescriptorBinding
		0,                                          // uint32_t                             TargetArrayElement
		VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,          // VkDescriptorType                     TargetDescriptorType
		{                                           // std::vector<VkDescriptorBufferInfo>  BufferInfos
		  {
			mUniformBuffer->getBuffer(),                           // VkBuffer                             buffer
			0,                                        // VkDeviceSize                         offset
			VK_WHOLE_SIZE                             // VkDeviceSize                         range
		  }
		}
	  }
	};

	//VulkanContext* gs = VulkanContext::Get();
	//gs->updateDescriptorSets(image_descriptor_infos, buffer_descriptor_infos, {}, {});
	return true;
}

bool TextureUniformBufferDescriptor::destroy(VkDevice device)
{
	mDescriptorSets->destroy(device, mDescriptorPool);
	mDescriptorPool->destroy(device);
	mDescriptorSetLayouts->destroy(device);
	mUniformBuffer->destroy(device);
	mCombinedImageSampler->destroy(device);
	return true;
}
