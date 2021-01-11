#ifndef SINGULARITY_AI_BEHAVIOR_ACTION_NODE_
#define SINGULARITY_AI_BEHAVIOR_ACTION_NODE_
#include "Common.h"
#include "BehaviorTypes.h"
#include "BehaviorNode.h"

namespace SingularityEngine::AI::Behavior
{
	class ActionNode : public BehaviorNode
	{
	public:
	protected:
		virtual ~ActionNode() override {}
		virtual void OnInitialize() override {}
		virtual void OnTerminate(BehaviorStatus status) override {}
		virtual BehaviorStatus OnUpdate(float) override = 0;
	};
}


#endif