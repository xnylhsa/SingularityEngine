#pragma once
#include "common.h"
#include "Sampler.h"
#include "SampledImage.h"
namespace SingularityEngine::Vulkan
{
	
	class CombinedImageSampler
	{
	public:
		struct CombinedImageSamplerParams
		{
			Sampler::SamplerParams samplerParams;
			SampledImage::SampleImageParams sampleImageParams;
		};

		CombinedImageSampler(VkPhysicalDevice physicalDevice, VkDevice device, CombinedImageSamplerParams params);
		bool destroy(VkDevice device);
		VkSampler getSampler() { return mSampler.get(); }
		VkImageView getImageView() { return mSampledImage.getImageView(); }
		~CombinedImageSampler();
	private:
		Sampler mSampler;
		SampledImage mSampledImage;
	};
}