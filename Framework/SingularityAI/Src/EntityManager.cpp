#include "Precompiled.h"
#include "EntityManager.h"
#include "AIEntity.h"
namespace
{
	SingularityEngine::AI::EntityManager* pEntityManger;
}


using namespace SingularityEngine;
using namespace SingularityEngine::AI;

void EntityManager::RegisterEntity(AIEntity* newEntity)
{
	auto it = mEntitys.find(newEntity->GetID());
	ASSERT(it == mEntitys.end(), "Error in registering entity, Entity already registered!");
	mEntitys.emplace(newEntity->GetID(), newEntity);
}
AIEntity* EntityManager::GetEntityFromID(size_t id) const
{
	auto it = mEntitys.find(id);
	ASSERT(it != mEntitys.end(), "Error in retreiving entity, Entity was not found!");
	return it->second;
}

void EntityManager::RemoveEntity(AIEntity* entityForRemoval)
{
	auto it = mEntitys.find(entityForRemoval->GetID());
	ASSERT(it != mEntitys.end(), "Error in removing entity, Entity was not found!");
	mEntitys.erase(it);
}
void EntityManager::StaticInitialize()
{
	pEntityManger = new SingularityEngine::AI::EntityManager();
	pEntityManger->Initialize();
}
void EntityManager::StaticTerminate()
{
	pEntityManger->Terminate();
	delete pEntityManger;
	pEntityManger = nullptr;
}

EntityManager::EntityManager()
{

}

EntityManager::~EntityManager()
{

}
void EntityManager::Initialize()
{
	mEntitys.clear();
}
void EntityManager::Terminate()
{
	mEntitys.clear();

}

EntityManager* EntityManager::Get()
{
	return pEntityManger;
}