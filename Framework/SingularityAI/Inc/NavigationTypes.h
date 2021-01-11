#ifndef SINGULARITY_AI_NAVIGATION_TYPES_
#define SINGULARITY_AI_NAVIGATION_TYPES_
#include "Common.h"

namespace SingularityEngine::AI::Navigation
{
	class INode;
	enum State
	{
		SEARCH_NOT_STARTED = 0,
		PATH_NOT_FOUND = 1,
		PATH_FOUND = 2,
		SEARCH_INCOMPLETE = 3,
	};

	class IHeuristicFunctor
	{
	public:
		virtual float operator()(INode* currentNode, INode* goalNode) = 0;
	};

}

#endif
