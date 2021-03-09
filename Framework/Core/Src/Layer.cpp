#include "precompiled.h"
#include "Layer.h"

namespace SingularityEngine::Core
{



	const std::string Layer::getName() const
	{
		return mDebugName;
	}

	void Layer::OnEvent(Event& event)
	{
		UNREFERENCED_PARAMETER(event);
	}

	void Layer::OnUpdate()
	{

	}

	void Layer::OnDetach()
	{

	}

	void Layer::OnAttach()
	{

	}

	Layer::Layer(const std::string& name /*= "Layer"*/) : mDebugName(name)
	{

	}

	Layer::~Layer()
	{

	}

}

