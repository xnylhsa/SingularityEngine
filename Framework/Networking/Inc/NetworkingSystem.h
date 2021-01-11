//====================================================================================================
// Filename:	NetworkingSystem.h
// Created by:	Ashlyn Gadow
//====================================================================================================


#ifndef INCLUDED_NETWORK_SYSTEM_HEADER_
#define INCLUDED_NETWORK_SYSTEM_HEADER_
#include "Common.h"

namespace SingularityEngine
{
	namespace Networking
	{
		struct NetworkSystem
		{
		public:
			static bool StaticInitialize();
			static void StaticTerminate();
			static NetworkSystem* Get();
			NetworkSystem(const NetworkSystem&) = delete;
			NetworkSystem& operator=(const NetworkSystem&) = delete;
		private:
			static bool networkInitialized;
			static NetworkSystem * sNetworkSystem;
		};

	}
}

#endif