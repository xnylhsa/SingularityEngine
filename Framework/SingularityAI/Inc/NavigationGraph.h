#ifndef SINGULARITY_AI_GRAPH_
#define SINGULARITY_AI_GRAPH_
#include "Common.h"
namespace SingularityEngine::AI::Navigation
{
	class INode;

	class Graph
	{
	public:
		Graph();
		~Graph();
		void ResetGraphForNewSearch();
		virtual INode* GetNode(const size_t nodeIndex) const;
	protected:
		bool mInitialized = false;
		virtual void RemoveNode(uint32_t nodeIndex);
		virtual size_t AddNode(INode* newNode);
		std::vector<INode*> mNavigationNodes;

	private:

	};


}


#endif