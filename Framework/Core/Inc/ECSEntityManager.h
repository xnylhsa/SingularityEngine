#ifndef ECS_ENTITY_MANAGER
#define ECS_ENTITY_MANAGER
#include "ECS.h"

namespace SingularityEngine::ECS
{
	class EntityManager
	{
	public:
		EntityManager();
		Entity createEntity();

		void destroyEntity(Entity entity);
		void setSignature(Entity entity, Signature signature);

		Signature getSignature(Entity entity);

	private:
		std::queue<Entity> mAvailableEntities{};

		std::array<Signature, MAX_ENTITIES> mSignatures{};

		uint32_t mLivingEntityCount{};
	};
}

#endif // ECS_ENTITY_MANAGER
