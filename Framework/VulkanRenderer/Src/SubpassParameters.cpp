#include "Precompiled.h"
#include "SubpassParameters.h"

using namespace SingularityEngine::Vulkan;


std::vector<VkSubpassDescription> ConvertSubpassDescriptions(std::vector<SubpassParameters> subpassParameters)
{
	std::vector<VkSubpassDescription> subpassDescriptions;

	for (auto&& subpassDescription : subpassParameters)
	{
		subpassDescriptions.push_back({
			  0,                                                                      // VkSubpassDescriptionFlags        flags
			  subpassDescription.pipelineType,                                       // VkPipelineBindPoint              pipelineBindPoint
			  static_cast<uint32_t>(subpassDescription.inputAttachments.size()),     // uint32_t                         inputAttachmentCount
			  subpassDescription.inputAttachments.data(),                            // const VkAttachmentReference    * pInputAttachments
			  static_cast<uint32_t>(subpassDescription.colorAttachments.size()),     // uint32_t                         colorAttachmentCount
			  subpassDescription.colorAttachments.data(),                            // const VkAttachmentReference    * pColorAttachments
			  subpassDescription.resolveAttachments.data(),                          // const VkAttachmentReference    * pResolveAttachments
			  subpassDescription.depthStencilAttachment,                             // const VkAttachmentReference    * pDepthStencilAttachment
			  static_cast<uint32_t>(subpassDescription.preserveAttachments.size()),  // uint32_t                         preserveAttachmentCount
			  subpassDescription.preserveAttachments.data()                          // const uint32_t                 * pPreserveAttachments
			});
	}
	return subpassDescriptions;
}
