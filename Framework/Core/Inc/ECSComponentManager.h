#ifndef SINGULARITY_ENGINE_ECS_COMPONENT_MANAGER
#define SINGULARITY_ENGINE_ECS_COMPONENT_MANAGER
#include "ECS.h"
#include "ECSComponentArray.h"


namespace SingularityEngine::ECS
{
	class ComponentManager
	{
	public:
		template<typename T>
		void registerComponent();
		template<typename T>
		ComponentType getComponentType();
		template<typename T>
		void addComponent(Entity entity, T component);
		template<typename T>
		void removeComponent(Entity entity);

		template<typename T>
		T& getComponent(Entity entity);

		void entityDestroyed(Entity entity);
	private:
		// Map from type string pointer to a component type
		std::unordered_map<const char*, ComponentType> mComponentTypes{};

		// Map from type string pointer to a component array
		std::unordered_map<const char*, std::shared_ptr<IComponentArray>> mComponentArrays{};

		// The component type to be assigned to the next registered component - starting at 0
		ComponentType mNextComponentType{};

		// Convenience function to get the statically casted pointer to the ComponentArray of type T.
		template<typename T>
		std::shared_ptr<ComponentArray<T>> getComponentArray();
	};

	template<typename T>
	std::shared_ptr<ComponentArray<T>> ComponentManager::getComponentArray()
	{
		const char* typeName = typeid(T).name();

		ASSERT(mComponentTypes.find(typeName) != mComponentTypes.end(), "Component not registered before use.");

		return std::static_pointer_cast<ComponentArray<T>>(mComponentArrays[typeName]);
	}

	template<typename T>
	T& ComponentManager::getComponent(Entity entity)
	{
		return getComponentArray<T>()->getData(entity);
	}

	template<typename T>
	void ComponentManager::removeComponent(Entity entity)
	{
		return getComponentArray<T>()->removeData(entity);
	}

	template<typename T>
	void ComponentManager::addComponent(Entity entity, T component)
	{
		getComponentArray<T>()->insertData(entity, component);
	}

	template<typename T>
	ComponentType ComponentManager::getComponentType()
	{
		const char* typeName = typeid(T).name();
		ASSERT(mComponentTypes.find(typeName != mComponentTypes.end()), "Component Not Registered Before Use!");
		return mComponentTypes[typeName];
	}

	template<typename T>
	void ComponentManager::registerComponent()
	{
		const char* typeName = typeid(T).name();
		ASSERT(mComponentTypes.find(typeName) == mComponentTypes.end(), "Registering component type more than once.");

		mComponentTypes.insert({ typeName, mNextComponentType });
		mComponentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});
		++mNextComponentType;
	}

}

#endif //SINGULARITY_ENGINE_ECS_COMPONENT_MANAGER