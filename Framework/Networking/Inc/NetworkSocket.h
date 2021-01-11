//====================================================================================================
// Filename:	NetworkSocket.h
// Created by:	Ashlyn Gadow
//====================================================================================================

#ifndef INCLUDED_NETWORK_SOCKETS_HEADER_
#define INCLUDED_NETWORK_SOCKETS_HEADER_
#include "Common.h"
#include "NetworkAddress.h"
namespace SingularityEngine
{
	namespace Networking
	{
		struct NetworkSocket
		{
		public:
			NetworkSocket();
			~NetworkSocket();
			bool IsOpen() const;
			void Close();
			bool Open(const NetworkAddress address, const unsigned short port);
			bool SendData(NetworkAddress& destination, const uint8_t* data, int size);
			int RecieveData(NetworkAddress& sender, uint8_t* data, int size);


		private:
			SOCKET socketHandle;
			NetworkAddress m_Address;
		};

	}
}


#endif