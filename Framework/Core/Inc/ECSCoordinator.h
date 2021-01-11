#ifndef SINGULARITY_ENGINE_ECS_COORDINATOR
#define SINGULARITY_ENGINE_ECS_COORDINATOR
#include "ECS.h"
#include "ECSComponentManager.h"
#include "ECSSystem.h"
#include "ECSEntityManager.h"
namespace SingularityEngine::ECS
{
	class Coordinator
	{
	public:
		void init();
		Entity createEntity();
		void destroyEntity(Entity entity);

		template<typename T>
		void registerComponent();
		template<typename T>
		void addComponent(Entity entity, T component);

		template<typename T>
		T& getComponent(Entity entity);

		template<typename T>
		ComponentType getComponentType();

		template<typename T>
		std::shared_ptr<T> registerSystem();

		template<typename T>
		void setSystemSignature(Signature signature);
	private:
		std::unique_ptr<ComponentManager> mComponentManager;
		std::unique_ptr<EntityManager> mEntityManager;
		std::unique_ptr<SystemManager> mSystemManager;
	};

	template<typename T>
	void Coordinator::setSystemSignature(Signature signature)
	{
		mSystemManager->setSignature<T>(signature);
	}

	template<typename T>
	std::shared_ptr<T> Coordinator::registerSystem()
	{
		return mSystemManager->registerSystem<T>();
	}

	template<typename T>
	ComponentType Coordinator::getComponentType()
	{
		return mComponentManager->getComponentType<T>();
	}

	template<typename T>
	T& Coordinator::getComponent(Entity entity)
	{
		return mComponentManager->getComponent<T>(entity);
	}

	template<typename T>
	void Coordinator::addComponent(Entity entity, T component)
	{
		mComponentManager->addComponent<T>(entity, component);
		auto signature = mEntityManager->getSignature(entity);
		signature.set(mComponentManager->getComponentType<T>(), true);

	}

	template<typename T>
	void Coordinator::registerComponent()
	{
		mComponentManager->registerComponent<T>();
	}
}
#endif //SINGULARITY_ENGINE_ECS_COORDINATOR