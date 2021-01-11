#include "Precompiled.h"
#include "Decorator.h"

using namespace SingularityEngine;
using namespace AI;
using namespace Behavior;



BehaviorStatus Repeater::OnUpdate(float deltaTime)
{
	while (true)
	{
		ASSERT(pChildNode != nullptr, "[AI::Behaviors] pointer to child was null!");
		pChildNode->Update(deltaTime);
		if (pChildNode->GetStatus() == BehaviorStatus::RUNNING) break;
		if (pChildNode->GetStatus() == BehaviorStatus::FAILED ) return BehaviorStatus::FAILED;
		if(++mCounter >=  mNumRepeats) return BehaviorStatus::SUCCEEDED;
	}
	return BehaviorStatus::RUNNING;
}
BehaviorStatus Negate::OnUpdate(float deltaTime)
{
	ASSERT(pChildNode != nullptr, "[AI::Behaviors] pointer to child was null!");
	BehaviorStatus returnStatus = pChildNode->Update(deltaTime);
	if(returnStatus == Behavior::BehaviorStatus::SUCCEEDED || returnStatus == Behavior::BehaviorStatus::FAILED)
		returnStatus = returnStatus == Behavior::BehaviorStatus::SUCCEEDED ? BehaviorStatus::FAILED : BehaviorStatus::SUCCEEDED;
	return returnStatus;
}
