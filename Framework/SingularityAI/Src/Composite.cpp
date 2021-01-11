#include "Precompiled.h"
#include "Composite.h"

using namespace SingularityEngine;
using namespace AI;
using namespace Behavior;
void Composite::AddChild(BehaviorNode& child)
{
	ASSERT(mChildCount < MAX_COMPOSITE_NODES, "[AI::Behaviors::Composite] You have exceeded the maximum number of child nodes. try using a sequence or selector to seperate allow for more!");
	ptrdiff_t pDiff = (uintptr_t)&child - (uintptr_t)this;
	//Ensures its within limits;
#ifdef ENVIRONMENT64 || ENVIRONMENT32
#ifdef ENVIRONMENT64
	ASSERT(pDiff < std::numeric_limits<uint32_t>::max(), "[AI::Behaviors::Compoisite] difference is greater than the max size of the offset ptr!");
	mChildren[mChildCount++] = static_cast<uint32_t>(pDiff);
#else
	ASSERT(pDiff < std::numeric_limits<uint16_t>::max(), "[AI::Behaviors::Compoisite] difference is greater than the max size of the offset ptr!");
	mChildren[mChildCount++] = static_cast<uint16_t>(pDiff);
#endif
#endif

}


BehaviorNode& Composite::GetChild(size_t index)
{
	ASSERT(index < mChildCount, "[AI::Behaviors::Composite] Out of bounds Index");
	return *(BehaviorNode*)((uintptr_t)this + mChildren[index]);
}

size_t Composite::GetNumChildren() const
{
	return mChildCount;
}
void Composite::RemoveChild(size_t index)
{
	ASSERT(index > 0, "[AI::Behaviors::Composite] Out of bounds Index");
	ASSERT(index < mChildCount, "[AI::Behaviors::Composite] Out of bounds Index");
	ASSERT(mChildCount > 0, "[AI::Behaviors::Composite] Composite node is already empty!");
	for (size_t i = index; i < mChildCount - 1; i++)
	{
		mChildren[i] = mChildren[i + 1];
	}
	mChildCount--;
}
void Composite::ClearChildren()
{
	mChildCount = 0;
	for (size_t i = 0; i < MAX_COMPOSITE_NODES; i++)
	{
		mChildren[i] = 0;
	}
}
