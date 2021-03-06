#ifndef SINGULARITY_ENGINE_SHADER_MODULE
#define SINGULARITY_ENGINE_SHADER_MODULE
#include "Common.h"
namespace SingularityEngine::Graphics
{
	class ShaderModule
	{
	public:
		ShaderModule(VkDevice device, std::vector<unsigned char> sourceCode);
		VkShaderModule& get() { return mShaderModule; }
	private:
		VkShaderModule mShaderModule;
	};
}

#endif //SINGULARITY_ENGINE_SHADER_MODULE