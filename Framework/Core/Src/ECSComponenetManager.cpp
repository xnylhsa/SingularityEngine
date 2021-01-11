#include "Precompiled.h"
#include "ECSComponentManager.h"


using namespace SingularityEngine::ECS;


void ComponentManager::entityDestroyed(Entity entity)
{
	for (auto const& pair : mComponentArrays)
	{
		auto const& component = pair.second;

		component->entityDestroyed(entity);
	}
}