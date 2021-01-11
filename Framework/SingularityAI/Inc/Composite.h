#ifndef SINGULARITY_AI_COMPOSITE_NODE_
#define SINGULARITY_AI_COMPOSITE_NODE_
#include "Common.h"
#include "BehaviorNode.h"


namespace SingularityEngine::AI::Behavior
{
	class Composite : public BehaviorNode
	{
	public:
		void AddChild(BehaviorNode&);
		void ClearChildren();
		BehaviorNode& GetChild(size_t index);
		size_t GetNumChildren() const;
		void RemoveChild(size_t index);

	protected:
#ifdef ENVIRONMENT64 || ENVIRONMENT32
#ifdef ENVIRONMENT64
		uint32_t mChildren[MAX_COMPOSITE_NODES];
		uint32_t mChildCount;
#else
		uint16_t mChildren[MAX_COMPOSITE_NODES];
		uint16_t mChildCount;
#endif
#endif
	};
}


#endif