#ifndef SINGULARITY_ENGINE_ECS_SYSTEM
#define SINGULARITY_ENGINE_ECS_SYSTEM
#include <set>
#include "ECS.h"

namespace SingularityEngine::ECS
{
	class System
	{
	public:
		std::set<Entity> mEntities;
	};

	class SystemManager
	{
	public:
		template<typename T>
		std::shared_ptr<T> registerSystem();

		template<typename T>
		void setSignature(Signature signature);

		void entityDestroyed(Entity entity);
		void entitySignatureChanged(Entity entity, Signature entitySignature);


	private:
		std::unordered_map<const char*, Signature> mSignatures{};
		std::unordered_map<const char*, std::shared_ptr<System>> mSystems{};
	};

	template<typename T>
	void SystemManager::setSignature(Signature signature)
	{
		const char* typeName = typeid(T).name();
		ASSERT(mSignatures.find(typeName) == mSignatures.end(), "Registering System more than once.");
		mSignatures.insert({ typeName, signature });
	}

	template<typename T>
	std::shared_ptr<T> SystemManager::registerSystem()
	{
		const char* typeName = typeid(T).name();
		ASSERT(mSystems.find(typeName) == mSystems.end(), "Registering System more than once.");

		auto system = std::make_shared<T>();
		mSystems.insert({ typeName, system });
		return system;
	}

}

#endif // SINGULARITY_ENGINE_ECS_SYSTEM
