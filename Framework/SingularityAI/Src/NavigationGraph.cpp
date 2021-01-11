#include "Precompiled.h"
#include "NavigationGraph.h"
#include "NavigationNode.h"
using namespace SingularityEngine::AI::Navigation;



Graph::Graph()
{

}
Graph::~Graph()
{

}

void Graph::RemoveNode(uint32_t )
{


}
size_t Graph::AddNode(INode* )
{

	return 0;
}
INode* Graph::GetNode(size_t nodeIndex) const
{
	ASSERT(nodeIndex >= 0 && nodeIndex < mNavigationNodes.size(), "[AI::NavigationGraph] Index Out Of Range");
	return mNavigationNodes[nodeIndex];
}
void Graph::ResetGraphForNewSearch()
{
	for (auto it : mNavigationNodes)
	{
		it->ResetSearchParameters();
	}
}
