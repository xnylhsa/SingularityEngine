#include "precompiled.h"
#include "LayerStack.h"
#include "Layer.h"
namespace SingularityEngine::Core
{


	LayerStack::LayerStack()
	{
		mLayerInsertPoint = mLayers.begin();

	}

	LayerStack::~LayerStack()
	{
		for (auto& layer : mLayers)
		{
			delete layer;
			layer = nullptr;
		}
	}

	void LayerStack::pushLayer(Layer* layer)
	{
		mLayerInsertPoint = mLayers.emplace(mLayerInsertPoint, layer);
	}

	void LayerStack::pushOverlay(Layer* layerOverlay)
	{
		mLayers.emplace_back(layerOverlay);
	}

	void LayerStack::popLayer(Layer* layer)
	{
		auto it = std::find(mLayers.begin(), mLayers.end(), layer);
		if (it != mLayers.end())
		{
			mLayers.erase(it);
			mLayerInsertPoint--;
		}
	}

	void LayerStack::popOverlay(Layer* layerOverlay)
	{
		auto it = std::find(mLayers.begin(), mLayers.end(), layerOverlay);
		if (it != mLayers.end())
		{
			mLayers.erase(it);
		}
	}

}