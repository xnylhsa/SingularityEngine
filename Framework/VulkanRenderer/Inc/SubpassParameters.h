#pragma once
#include "Common.h"

namespace SingularityEngine::Graphics
{
	struct SubpassParameters 
	{
		VkPipelineBindPoint pipelineType;
		std::vector<VkAttachmentReference> inputAttachments;
		std::vector<VkAttachmentReference> colorAttachments;
		std::vector<VkAttachmentReference> resolveAttachments;
		VkAttachmentReference const* depthStencilAttachment;
		std::vector<uint32_t> preserveAttachments;
	};
	std::vector<VkSubpassDescription> ConvertSubpassDescriptions(std::vector<SubpassParameters> subpassParameters);
}