#include "Precompiled.h"
#include "ECSEntityManager.h"
#include "Debug.h"

using namespace SingularityEngine::ECS;

EntityManager::EntityManager()
{
	//maybe remove this and replace with iterator static to this class?
	for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
	{
		mAvailableEntities.push(entity);
	}
}

Entity EntityManager::createEntity()
{
	ASSERT(mLivingEntityCount < MAX_ENTITIES,  "Too many entities in existence.");

	Entity id = mAvailableEntities.front();
	mAvailableEntities.pop();
	++mLivingEntityCount;

	return id;
}

void EntityManager::destroyEntity(Entity entity)
{
	ASSERT(entity < MAX_ENTITIES, "Entity out of range.");

	mSignatures[entity].reset();

	mAvailableEntities.push(entity);
	--mLivingEntityCount;
}

void EntityManager::setSignature(Entity entity, Signature signature)
{
	ASSERT(entity < MAX_ENTITIES, "Entity out of range.");
	mSignatures[entity] = signature;
}

Signature EntityManager::getSignature(Entity entity)
{
	ASSERT(entity < MAX_ENTITIES, "Entity out of range.");
	return mSignatures[entity];
}
