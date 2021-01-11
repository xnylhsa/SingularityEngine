#ifndef SINGULARITY_AI_NAV_FUNCS
#define SINGULARITY_AI_NAV_FUNCS
#include "Common.h"
namespace SingularityEngine::AI::Navigation
{
	class INode;
	class Graph;
	class IHeuristicFunctor;
	class SearchContext;
	class InformedSearchContext;
	using NavNodeBucket = std::vector<INode*>;
	void RunBFS(Graph* graph, size_t startIndex, size_t endIndex, float allowedRunDuration, SearchContext& currentPath);
	void RunDFS(Graph* graph, size_t startIndex, size_t endIndex, float allowedRunDuration, SearchContext& currentPath);
	void RunAStar(Graph* graph, size_t startIndex, size_t endIndex, IHeuristicFunctor* heuristicFunction, float allowedRunDuration, SearchContext& currentPath);
	void RunDjikstra(Graph* graph, size_t startIndex, size_t endIndex, float allowedRunDuration, SearchContext& currentPath);

}
#endif // SINGULARITY_AI_NAV_FUNCS

