#ifndef SINGULARITY_AI_BEHAVIOR_TYPES_
#define SINGULARITY_AI_BEHAVIOR_TYPES_
#include "Common.h"
#include "BehaviorTypes.h"
#define MAX_BEHAVIOR_TREE_MEMORY  251337420
#define MAX_COMPOSITE_NODES 7
namespace SingularityEngine::AI::Behavior
{
	enum class BehaviorStatus
	{
		INVALID = 0,
		RUNNING,
		FAILED,
		ABORTED,
		SUCCEEDED,
		ABORTING,
		SUSPENDED,

	};


}


#endif