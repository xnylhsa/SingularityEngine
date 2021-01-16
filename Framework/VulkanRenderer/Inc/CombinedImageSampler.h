#pragma once
#include "common.h"
#include "Sampler.h"
#include "ImageClasses/SampledImage.h"
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
		~CombinedImageSampler();
	private:
		Sampler mSampler;
		SampledImage mSampledImage;
	};
}