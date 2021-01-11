#ifndef SINGULARITY_AI_SEQUENCE_NODE_
#define SINGULARITY_AI_SEQUENCE_NODE_
#include "Composite.h"


namespace SingularityEngine::AI::Behavior
{
	class Sequence : public Composite
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

				if (s != BehaviorStatus::SUCCEEDED) return s;

				if (++mCurrentIndex == GetNumChildren())
					return BehaviorStatus::SUCCEEDED;
			}
			return BehaviorStatus::INVALID;
		}
	};

	class ConditionFilter : public Sequence
	{
	public:
		void SetCondition(BehaviorNode&);
		void SetAction(BehaviorNode&);
		void ClearCondtion();
		void ClearAction();
		virtual BehaviorStatus OnUpdate(float deltaTime) override
		{
			ASSERT(GetNumChildren() < 2 && mCurrentIndex < GetNumChildren(), "[AI::Behavior] index somehow exceeded number of nodes!");
			while (true)
			{
				BehaviorStatus s = GetChild(mCurrentIndex).Update();

				if (s != BehaviorStatus::SUCCEEDED) return s;

				if (++mCurrentIndex == GetNumChildren())
					return BehaviorStatus::SUCCEEDED;
			}
			return BehaviorStatus::INVALID;
		}
	protected:
		size_t numCondtions;
		size_t numActions;
	};
}


#endif