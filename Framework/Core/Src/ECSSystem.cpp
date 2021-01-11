#include "Precompiled.h"
#include "ECSSystem.h"

using namespace SingularityEngine::ECS;

void SingularityEngine::ECS::SystemManager::entityDestroyed(Entity entity)
{
	for (auto const& pair : mSystems)
	{
		auto const& system = pair.second;
		system->mEntities.erase(entity);
	}
}

void SingularityEngine::ECS::SystemManager::entitySignatureChanged(Entity entity, Signature entitySignature)
{
	for (auto const& pair : mSystems)
	{
		auto const& type = pair.first;
		auto const& system = pair.second;
		auto const& systemSignature = mSignatures[type];

		if ((entitySignature & systemSignature) == systemSignature)
		{
			system->mEntities.insert(entity);
		}
		else
		{
			system->mEntities.erase(entity);
		}
	}
}
