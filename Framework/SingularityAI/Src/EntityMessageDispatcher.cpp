#include "Precompiled.h"
#include "EntityMessageDispatcher.h"
#include "EntityManager.h"
#include "AIEntity.h"
namespace
{
	SingularityEngine::AI::MessageDispatcher* pMsgDispatcher;
}
using namespace SingularityEngine;
using namespace SingularityEngine::AI;


MessageDispatcher* MessageDispatcher::Get()
{
	return pMsgDispatcher;
}

void MessageDispatcher::StaticInitialize()
{
	pMsgDispatcher = new MessageDispatcher();
	pMsgDispatcher->Initialize();
}

void MessageDispatcher::StaticTerminate()
{
	pMsgDispatcher->Terminate();
	delete pMsgDispatcher;
	pMsgDispatcher = nullptr;
}

void MessageDispatcher::SendAIMessage(float delay, size_t sender, size_t reciever, uint32_t msgEnum, void* otherInfo)
{
	AIEntity* recipeient = AI::EntityManager::Get()->GetEntityFromID(reciever);
	AITelegram telegram(0, sender, reciever, msgEnum, otherInfo);
	if (delay <= 0.0f)
	{
		Discharge(recipeient, telegram);
	}
	else
	{
		float currentTime = msgTimer.GetTotalTime();
		telegram.DispatchTime = currentTime + delay;
		mTelegramQueue.push_back(telegram);
		std::make_heap(mTelegramQueue.begin(), mTelegramQueue.end());
		std::sort_heap(mTelegramQueue.begin(), mTelegramQueue.end());

	}
}

void MessageDispatcher::Update()
{
	msgTimer.Update();
	float CurrentTime = msgTimer.GetTotalTime();
	bool continueProcessingMessages = true;
	while (mTelegramQueue.size() > 0)
	{
		continueProcessingMessages = mTelegramQueue.begin()->DispatchTime < CurrentTime && mTelegramQueue.back().DispatchTime > 0;
		if (!continueProcessingMessages)
			return;
		AITelegram telegram = *mTelegramQueue.begin();
		AIEntity* pReciever = EntityManager::Get()->GetEntityFromID(telegram.Receiver);
		Discharge(pReciever, telegram);
		mTelegramQueue.pop_back();
	}
}

void MessageDispatcher::Initialize()
{

}

void MessageDispatcher::Terminate()
{

}

void MessageDispatcher::Discharge(AI::AIEntity* entity , const AITelegram& telegram)
{
	if (entity)
	{
		entity->HandleMessage(telegram);
	}
}

MessageDispatcher::MessageDispatcher()
{

}

MessageDispatcher::~MessageDispatcher()
{

}
