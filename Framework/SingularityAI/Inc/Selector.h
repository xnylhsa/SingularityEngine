#ifndef SINGULARITY_AI_SELECTOR_NODE_
#define SINGULARITY_AI_SELECTOR_NODE_
#include "Composite.h"


namespace SingularityEngine::AI::Behavior
{
	class Selector : public Composite
	{
	public:
	protected:
		size_t mCurrentIndex;
		virtual void OnIntialize() override()
		{
			mCurrentIndex = 0;
		}
		virtual BehaviorStatus OnUpdate(float deltaTime) override
		{
			ASSERT(mCurrentIndex < MAX_COMPOSITE_NODES && mCurrentIndex < GetNumChildren(), "[AI::Behavior] index somhow exceeded number of nodes!");
			while (true)
			{
				BehaviorStatus s = GetChild(mCurrentIndex).Update();
				if (s != BehaviorStatus::FAILED) return s;

				if (++mCurrentIndex == GetNumChildren())
					return BehaviorStatus::FAILED;
			}
			return BehaviorStatus::INVALID;
		}
	};
}


#endif