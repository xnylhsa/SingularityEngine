#ifndef SINGULARITY_ENTITY_MANAGER_MACHINE_
#define SINGULARITY_ENTITY_MANAGER_MACHINE_
#include "Common.h"

namespace SingularityEngine::AI
{
	class AIEntity;
	class EntityManager
	{
	public:
		void RegisterEntity(AIEntity* newEntity);
		AIEntity* GetEntityFromID(size_t id) const;
		void RemoveEntity(AIEntity* entityForRemoval);
		static EntityManager* Get();
		static void StaticInitialize();
		static void StaticTerminate();
		EntityManager(const EntityManager&) = delete;
		EntityManager& operator=(const EntityManager&) = delete;
	private:
		EntityManager();
		~EntityManager();
		void Initialize();
		void Terminate();
		typedef std::map<size_t, AIEntity*> entityMap;
		entityMap mEntitys;

	};
}

#endif