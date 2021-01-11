#ifndef SINGULARITY_AI_NAVIGATION_VOLUME_
#define SINGULARITY_AI_NAVIGATION_VOLUME_
#include "Common.h"
#include "NavigationGraph.h"
namespace SingularityEngine::AI::Navigation
{
	class INode;

	class NavVolume : public Graph
	{
	public:

		NavVolume();
		~NavVolume();

		void RemoveNode(uint32_t nodeIndex);
		size_t AddNode(INode* newNode);
		INode* GetNode(const size_t nodeIndex) const;


	private:
		std::vector<INode*> mNavigationNodes;
	};


}


#endif