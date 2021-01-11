#ifndef INCLUDED_SERVER_HEADER_
#define INCLUDED_SERVER_HEADER_
#include "PacketFactory.h"
#include "NetworkAddress.h"
#include "Common.h"
namespace SingularityEngine
{
	namespace Networking
	{
		class ReliableUDPServer
		{
			std::map<NetworkAddress, float> ClientTimeouts;
			float ClientDisconnectionTime = 10.0f;
			void IsClientConnected(NetworkAddress address);
			void ResetClient(NetworkAddress address);
			void AddClient(NetworkAddress address);
			void UpdateClientTimers(float deltaTime);
			void SendPacketToClients();
			void SendPacketToClient(NetworkAddress address);

		};
	}
}

#endif