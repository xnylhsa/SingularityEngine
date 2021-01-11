#include "Precompiled.h"
#include "ECSCoordinator.h"

void SingularityEngine::ECS::Coordinator::init()
{
	mComponentManager = std::make_unique<ComponentManager>();
	mEntityManager = std::make_unique<EntityManager>();
	mSystemManager = std::make_unique<SystemManager>();
}

SingularityEngine::ECS::Entity SingularityEngine::ECS::Coordinator::createEntity()
{
	return mEntityManager->createEntity();

}

void SingularityEngine::ECS::Coordinator::destroyEntity(Entity entity)
{
	mEntityManager->destroyEntity(entity);

	mComponentManager->entityDestroyed(entity);

	mSystemManager->entityDestroyed(entity);
}
