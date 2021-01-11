#ifndef INCLUDED_AI_TELEGRAM_
#define INCLUDED_AI_TELEGRAM_

namespace SingularityEngine::AI
{
	struct AITelegram
	{
		//the entity that sent this telegram
		size_t Sender;
		//the entity that is to receive this telegram
		size_t Receiver;
		//the message itself. These are all enumerated in the file
		//"MessageTypes.h"
		uint32_t Msg;
		//messages can be dispatched immediately or delayed for a specified amount
		//of time. If a delay is necessary, this field is stamped with the time
		//the message should be dispatched.
		float DispatchTime;
		//any additional information that may accompany the message
		void* ExtraInfo;
		AITelegram( float dispatchTime,size_t sender, size_t reciever, uint32_t msg, void* otherInfo) :Sender(sender), Receiver(reciever), Msg(msg), DispatchTime(dispatchTime), ExtraInfo(otherInfo) {}
		bool operator<(const AITelegram& right)
		{
			return DispatchTime < right.DispatchTime;
		}
		bool operator>( const AITelegram& right)
		{
			return DispatchTime > right.DispatchTime;
		}
	};
}

#endif // INCLUDED_AI_TELEGRAM_
