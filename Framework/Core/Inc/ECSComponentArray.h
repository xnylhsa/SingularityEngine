#ifndef SINGULARITY_COMPONENT_ARRAY
#define SINGULARITY_COMPONENT_ARRAY
#include "ECS.h"
#include "Debug.h"
namespace SingularityEngine::ECS
{
	class IComponentArray
	{
	public:
		virtual ~IComponentArray() = default;
		virtual void entityDestroyed(Entity entity) = 0;
	};

	template<typename T>
	class ComponentArray : public IComponentArray
	{
	public:
		void insertData(Entity entity, T Component);
		void removeData(Entity entity);
		T& getData(Entity entity);

		void entityDestroyed(Entity entity) override;
	private:
		std::vector<T> mComponentVector;
		std::unordered_map<Entity, size_t> mEntityToIndexMap;
		std::unordered_map<size_t, Entity> mIndexToEntityMap;
	};

}
template<typename T>
void SingularityEngine::ECS::ComponentArray<T>::entityDestroyed(Entity entity)
{
	if (mEntityToIndexMap.find(entity) != mEntityToIndexMap.end())
	{
		// Remove the entity's component if it existed
		removeData(entity);
	}
}

template<typename T>
T& SingularityEngine::ECS::ComponentArray<T>::getData(Entity entity)
{
	ASSERT(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end(), "Retrieving non-existent component.");

	// Return a reference to the entity's component
	return mComponentVector[mEntityToIndexMap[entity]];
}

template<typename T>
void SingularityEngine::ECS::ComponentArray<T>::removeData(Entity entity)
{
	ASSERT(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end(), "Removing non-existent component!");
	size_t indexOfRemovedEntity = mEntityToIndexMap[entity];
	size_t indexOfLastElement = mComponentVector.size() - 1;
	if (indexOfRemovedEntity < indexOfLastElement)
	{
		mComponentVector[indexOfRemovedEntity] = mComponentVector[indexOfLastElement];
		mComponentVector.pop_back();
		Entity entityOfLastElement = mIndexToEntityMap[indexOfLastElement];
		mEntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
		mIndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;
	}
	else
	{
		mComponentVector.pop_back();
	}
	mEntityToIndexMap.erase(entity);
	mIndexToEntityMap.erase(indexOfLastElement);
}

template<typename T>
void SingularityEngine::ECS::ComponentArray<T>::insertData(Entity entity, T component)
{
	ASSERT(mEntityToIndexMap.find(entity) == mEntityToIndexMap.end(), "Component Already Exists for Entity: %d", entity);

	size_t newIndex = mComponentVector.size();
	mEntityToIndexMap[entity] = newIndex;
	mIndexToEntityMap[newIndex] = entity;
	mComponentVector.push_back(component);
}
#endif