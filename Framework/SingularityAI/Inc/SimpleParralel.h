#ifndef SINGULARITY_AI_SIMPLE_PARRALLEL_NODE_
#define SINGULARITY_AI_SIMPLE_PARRALLEL_NODE_
#include "Composite.h"


namespace SingularityEngine::AI::Behavior
{

	class SimpleParallel : public Composite
	{
	public:
		SimpleParallel(Policy successPolicy, Policy failurePolicy) : Composite(), mFailPolicy(failurePolicy), mSucceedPolicy(successPolicy) { }
	protected:
		enum Policy
		{
			RequireOne,
			RequireAll,
		};

		virtual BehaviorStatus OnUpdate(float deltaTime) override
		{
			size_t successCount = 0, failCount = 0;
			for (size_t index = 0; i < GetNumChildren(); i++)
			{

				BehaviorNode& b = GetChild(index);
				if (!b.IsTerminated()) b.Update();
				if (b.GetStatus() == BehaviorStatus::SUCCEEDED)
				{
					successCount++;
					if (mSucceedPolicy == Policy::RequireOne)
					{
						return BehaviorStatus::SUCCEEDED;
					}
				}
				if (b.GetStatus() == BehaviorStatus::FAILED)
				{
					failCount++;
					if (mFailPolicy == Policy::RequireOne)
					{
						return BehaviorStatus::FAILED;
					}
				}
			}
			if (mFailPolicy == Policy::RequireAll && failCount == GetNumChildren())
			{
				return BehaviorStatus::FAILED;
			}
			if (mSucceedPolicy == Policy::RequireAll && successCount == GetNumChildren())
			{
				return BehaviorStatus::SUCCEEDED;
			}

			return BehaviorStatus::RUNNING;
		}
		virtual BehaviorStatus OnAbort(float deltaTime)
		{
			size_t successCount = 0;
			for (size_t index = 0; i < GetNumChildren(); i++)
			{

				BehaviorNode& b = GetChild(index);
				if (!b.IsTerminated()) b.Abort(deltaTime);
				ASSERT(b.GetStatus() != BehaviorStatus::FAILED, "[AI::Behavior] Failed to properly abort Task at this Index!");
				if (b.GetStatus() == BehaviorStatus::ABORTED)
				{
					successCount++;
				}
			}
			if (successCount == GetNumChildren())
			{
				return BehaviorStatus::ABORTED;
			}
			return BehaviorStatus::ABORTING;
		}
		virtual void OnTerminate(Status status) override
		{
			for (auto it : m_Children)
			{
				BehaviorNode& b = **it;
				b.OnTerminate();
			}
		}
		Policy mSucceedPolicy;
		Policy mFailPolicy;
	};



}


#endif