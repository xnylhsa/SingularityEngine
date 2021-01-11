#ifndef SINGULARITY_ENTITY_MESSAGE_DISPATCHER_
#define SINGULARITY_ENTITY_MESSAGE_DISPATCHER_
#include "Common.h"
#include "AITelegram.h"


namespace SingularityEngine::AI
{
	class AIEntity;
	class MessageDispatcher
	{
	public:


		static MessageDispatcher* Get();
		static void StaticInitialize();
		static void StaticTerminate();

		void SendAIMessage(float delay, size_t sender, size_t reciever, uint32_t msgEnum, void* otherInfo);

		void Update();

		MessageDispatcher(const MessageDispatcher&) = delete;
		MessageDispatcher& operator=(const MessageDispatcher&) = delete;
		float GetMsgTime() { return msgTimer.GetTotalTime(); }
	private:
		void Initialize();
		void Terminate();
		std::vector<AITelegram> mTelegramQueue;
		void Discharge(AIEntity* recipient, const AITelegram& msg);
		Core::Timer msgTimer;
		MessageDispatcher();
		~MessageDispatcher();
	};


}



#endif