#ifndef SINGULARITY_AI_NAVIGATION_CONTEXT_
#define SINGULARITY_AI_NAVIGATION_CONTEXT_
#include "Common.h"
#include "NavigationTypes.h"
namespace SingularityEngine::AI::Navigation
{
	class INode;
	class SearchContext
	{
	public:
		std::list<INode*> mOpenList;
		std::list<INode*> mClosedList;
		std::vector<INode*> mTotalPath;

		State mSearchState = State::SEARCH_NOT_STARTED;
		void Clear()
		{
			mOpenList.clear();
			mClosedList.clear();
			mTotalPath.clear();
			mSearchState = State::SEARCH_NOT_STARTED;
		}
	};

}


#endif