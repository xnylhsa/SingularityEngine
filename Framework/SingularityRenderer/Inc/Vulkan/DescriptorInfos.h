#pragma once
#include "Common.h"

namespace SingularityEngine::SERenderer::Descriptors
{
	struct ImageDescriptorInfo {
		VkDescriptorSet                     TargetDescriptorSet;
		uint32_t                            TargetDescriptorBinding;
		uint32_t                            TargetArrayElement;
		VkDescriptorType                    TargetDescriptorType;
		std::vector<VkDescriptorImageInfo>  ImageInfos;
	};
	struct BufferDescriptorInfo {
		VkDescriptorSet                     TargetDescriptorSet;
		uint32_t                            TargetDescriptorBinding;
		uint32_t                            TargetArrayElement;
		VkDescriptorType                    TargetDescriptorType;
		std::vector<VkDescriptorBufferInfo> BufferInfos;
	};
	struct TexelBufferDescriptorInfo {
		VkDescriptorSet                     TargetDescriptorSet;
		uint32_t                            TargetDescriptorBinding;
		uint32_t                            TargetArrayElement;
		VkDescriptorType                    TargetDescriptorType;
		std::vector<VkBufferView>           TexelBufferViews;
	};
	struct CopyDescriptorInfo {
		VkDescriptorSet     TargetDescriptorSet;
		uint32_t            TargetDescriptorBinding;
		uint32_t            TargetArrayElement;
		VkDescriptorSet     SourceDescriptorSet;
		uint32_t            SourceDescriptorBinding;
		uint32_t            SourceArrayElement;
		uint32_t            DescriptorCount;
	};
}
