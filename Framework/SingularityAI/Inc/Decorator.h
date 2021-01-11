#ifndef SINGULARITY_AI_BEHAVIOR_DECORATOR_NODE_
#define SINGULARITY_AI_BEHAVIOR_DECORATOR_NODE_
#include "Common.h"
#include "BehaviorTypes.h"
#include "BehaviorNode.h"

namespace SingularityEngine::AI::Behavior
{
	class Decorator : public BehaviorNode
	{
	public:

		Decorator(BehaviorNode* child) : pChildNode(child) {}
	protected:
		BehaviorNode* pChildNode;
	};
	//Example Repeater
	class Repeater : public Decorator
	{
	public:

		Repeater(BehaviorNode* child, uint32_t numRepeats) : Decorator(child), mNumRepeats(numRepeats) { }
	protected:
		virtual void OnInitialize() { mCounter = 0; }
		virtual BehaviorStatus OnUpdate(float) override;
	private:
		uint32_t mNumRepeats;
		uint32_t mCounter{ 0 };
	};
	//Simple Negate Decorator, reverses the return status of the node below it
	class Negate : public Decorator
	{
	public:

		Negate(BehaviorNode* child) : Decorator(child) { }
	protected:
		virtual BehaviorStatus OnUpdate(float) override;
	};

	class Monitor : public Decorator
	{
	public:
		void SetCondtion(BehaviorNode* node) { mCondtion = node; }
		void SetAction(BehaviorNode*node) { pChildNode = node; }
		void ClearAction() { mCondtion = nullptr; }
		void ClearCondtion() { pChildNode = nullptr; }

	protected:
		virtual BehaviorStatus OnUpdate(float deltaTime) override
		{
			if (!(mCondtion->IsTerminated())) mCondtion->Update(deltaTime);
			if (mCondtion->GetStatus() == BehaviorStatus::SUCCEEDED)
			{
				if (!(pChildNode->IsTerminated())) pChildNode->Update(deltaTime);
				return pChildNode->GetStatus();
			}
			return BehaviorStatus::FAILED;
		}
		virtual BehaviorStatus OnAbort(float deltaTime)
		{
			if (!(mCondtion->IsTerminated())) mCondtion->Update(deltaTime);
			if (!(pChildNode->IsTerminated())) pChildNode->Abort(deltaTime);
			ASSERT(pChildNode->GetStatus() != BehaviorStatus::FAILED, "[AI::Behavior] Failed to properly abort Task at this Index!");
			return pChildNode->GetStatus();
		}
		BehaviorNode* mCondtion;
	};
}


#endif