#ifndef INCLUDED_BEHVAIOR_TREE_HEADER_
#define INCLUDED_BEHVAIOR_TREE_HEADER_
#include "Common.h"
#include "BehvaiorTypes.h"
namespace SingularityEngine::AI::Behavior
{
	class BlackBoard;
	class BehaviorNode;
	class BehaviorTree
	{
	public:
		BehaviorTree() : mNodeBlockBuffer(new uint8_t[MAX_BEHAVIOR_TREE_MEMORY]), mOffset(0) {}
		BehaviorTree() { SafeDeleteArray(mNodeBlockBuffer); }
		void Update(float deltaTime) {}

		template<typename BehaviorNodeType>
		BehaviorNodeType& AllocateNode()
		{
			BehaviorNodeType* node = new ((void*)((uintptr_t)mNodeBlockBuffer+mOffset)) BehaviorNodeType;
			mOffset += sizeof(BehaviorNodeType);
				return *node;
		}

	protected:

		BlackBoard* mBlackBoard;

		BehaviorNode* mRoot;	
		uint8_t* mNodeBlockBuffer;
		size_t mOffset;

	};
}


#endif // 
