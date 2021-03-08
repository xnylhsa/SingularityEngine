#ifndef SINGULARITY_ENGINE_LAYER_STACK
#define SINGULARITY_ENGINE_LAYER_STACK
#include "Common.h"
namespace SingularityEngine::Core
{
	class Layer;
	class LayerStack
	{
	public:
		using LayerStackContainer = std::vector<Layer*>;
		LayerStack();
		~LayerStack();

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* layerOverlay);
		void popLayer(Layer* layer);
		void popOverlay(Layer* layerOverlay);

		inline LayerStackContainer::iterator begin() { return mLayers.begin(); }
		inline LayerStackContainer::iterator end() { return mLayers.end(); }

	private:
		LayerStackContainer mLayers;
		LayerStackContainer::iterator mLayerInsertPoint;
	};
}

#endif