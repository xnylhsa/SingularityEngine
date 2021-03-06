#include "Precompiled.h"
#include "CombinedImageSampler.h"

using namespace SingularityEngine::Graphics;


CombinedImageSampler::CombinedImageSampler(VkPhysicalDevice physicalDevice, VkDevice device, CombinedImageSampler::CombinedImageSamplerParams params) : mSampler(device, params.samplerParams), mSampledImage(physicalDevice, device, params.sampleImageParams)
{
	
}

CombinedImageSampler::~CombinedImageSampler()
{
	//assert mSampler and sampled images arent valid.
}

bool CombinedImageSampler::destroy(VkDevice device)
{
	return mSampler.destroy(device) && mSampledImage.destroy(device);
}

