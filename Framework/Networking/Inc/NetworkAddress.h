#ifndef INCLUDED_NETWORK_ADDRESS_
#define INCLUDED_NETWORK_ADDRESS_
#include "Common.h"
#include "NetworkUtil.h"

namespace SingularityEngine
{
	namespace Networking
	{
		class NetworkAddress
		{
		public:
			NetworkAddress();
			NetworkAddress(uint8_t a, uint8_t b, uint8_t c, uint8_t d, unsigned int port);
			NetworkAddress(const char* address, unsigned int port);
			NetworkAddress(uint32_t address, unsigned int port);
			void Set(char* address, unsigned int port);
			void Set(uint32_t address, unsigned int port);
			uint32_t ParseIPV4string(const char* ipAddress);
			short GetAddressFamily() { return m_sockAddress.sin_family; }
			sockaddr_in* GetSockAddress() { return &m_sockAddress; }
			~NetworkAddress();
			inline bool operator< ( const NetworkAddress& rhs) { return (m_address < rhs.m_address) ?  true : false; }
			inline bool operator< (const NetworkAddress& rhs) const { return (m_address < rhs.m_address) ? true : false; }

		private:
			uint32_t m_address;
			unsigned int m_port;
			sockaddr_in m_sockAddress;
			void SetupSockAddress();
		};
	}
}


#endif