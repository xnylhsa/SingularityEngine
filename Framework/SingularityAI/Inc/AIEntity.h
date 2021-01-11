#ifndef SINGULARITY_AI_ENTITY__
#define SINGULARITY_AI_ENTITY__
#include "Common.h"
#include "AITelegram.h"
namespace SingularityEngine::AI
{
	class AIEntity
	{
	public:
		AIEntity(std::string name)
		{
			mName = name;
			SetID();
		}
		virtual ~AIEntity() {}
		virtual void Update(float deltaTime = 0.0f) = 0;
		virtual bool HandleMessage(const AITelegram& msg) = 0;
		std::string GetName() { return mName; }
		size_t GetID() { return mID; }
	private:
		void SetID() { mID = ++NextValidID; }
		size_t mID;
		std::string mName;
		static size_t NextValidID;
	};
}


#endif // BASICE_FINITE_STATE_MACHINE_
