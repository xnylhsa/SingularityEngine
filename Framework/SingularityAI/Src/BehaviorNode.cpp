#include "Precompiled.h"
#include "BehaviorNode.h"
		
using namespace SingularityEngine;
using namespace AI;
using namespace Behavior;

BehaviorStatus BehaviorNode::Update(float deltaTime)
{
	if (!(mStatus == BehaviorStatus::RUNNING))
	{
		OnInitialize();
	}
	mStatus = OnUpdate(deltaTime);
	if (!(mStatus == BehaviorStatus::RUNNING))
	{
		OnTerminate();
	}
	return mStatus;
}
BehaviorStatus BehaviorNode::Abort(float deltaTime)
{
	if ((mStatus == BehaviorStatus::RUNNING) || (mStatus == BehaviorStatus::ABORTING))
		mStatus = OnAbort(deltaTime);

	if (!(mStatus == BehaviorStatus::ABORTING) && !(mStatus == BehaviorStatus::RUNNING))
	{
		OnTerminate();
	}
	return mStatus;
}
