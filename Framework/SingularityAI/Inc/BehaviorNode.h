#ifndef SINGULARITY_AI_BEHAVIOR_NODE_
#define SINGULARITY_AI_BEHAVIOR_NODE_
#include "Common.h"
#include "BehaviorTypes.h"


namespace SingularityEngine::AI::Behavior
{
	class BehaviorNode
	{
	public:
		BehaviorStatus Update(float deltaTime);
		BehaviorStatus Abort(float deltaTime);

		bool IsRunning() { return mStatus == BehaviorStatus::RUNNING; }
		bool IsAborting() { return mStatus == BehaviorStatus::ABORTING; }
		bool IsTerminated() { return mStatus == BehaviorStatus::INVALID; }
		bool IsSuspended() { return mStatus == BehaviorStatus::SUSPENDED; }
		void Reset() { mStatus = BehaviorStatus::INVALID; }
		BehaviorStatus GetStatus() { return mStatus; }
	protected:
		BehaviorStatus mStatus;
		BehaviorNode* mParent;
		virtual ~BehaviorNode() {}
		virtual void OnInitialize() {}
		virtual void OnTerminate() { Reset(); }
		virtual BehaviorStatus OnAbort(float) { return BehaviorStatus::ABORTED; }
		virtual BehaviorStatus OnUpdate(float) = 0;
	};
}


#endif