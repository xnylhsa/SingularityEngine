#ifndef SINGULARITY_ENGINE_LAYER
#define SINGULARITY_ENGINE_LAYER
#include "Event.h"

namespace SingularityEngine::Core
{
	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach();
		virtual void OnDetach();
		virtual void OnUpdate();
		virtual void OnEvent(Event& event);

		const std::string getName() const;
	private:
		std::string mDebugName;
	};
}

#endif