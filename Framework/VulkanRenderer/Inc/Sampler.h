#pragma once
#include "common.h"

namespace SingularityEngine::Vulkan
{

	class Sampler
	{
	public:
		struct SamplerParams
		{
			VkFilter magFilter;
			VkFilter minFilter;
			VkSamplerMipmapMode mipMapMode;
			VkSamplerAddressMode uAddressMode;
			VkSamplerAddressMode vAddressMode;
			VkSamplerAddressMode wAddressMode;
			float lodBias;
			bool anisotropyEnable;
			float maxAnisotropy;
			bool compareEnable;
			VkCompareOp compareOperator;
			float minLOD;
			float maxLOD;
			VkBorderColor borderColor;
			bool unnormalizedCoords;
		};

		Sampler(VkDevice device, SamplerParams params);
		bool destroy(VkDevice device);
		~Sampler();
	private:
		VkSampler mSampler;
	};
}