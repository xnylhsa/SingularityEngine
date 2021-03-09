#ifndef SINGULARITY_ENGINE_RENDERER_GRAPHICS_DEVICE
#define SINGULARITY_ENGINE_RENDERER_GRAPHICS_DEVICE
#include "Common.h"

namespace SingularityEngine::SERenderer
{
	class IGraphicsDevice : public std::enable_shared_from_this<IGraphicsDevice>
	{
	public:
		virtual ~IGraphicsDevice() = default;
		virtual bool initialize() = 0;
		virtual void teardown() = 0;
		virtual bool waitUntilIdle() = 0;
		static std::shared_ptr<IGraphicsDevice> Create();
	};
}


#endif