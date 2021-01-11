#include "Precompiled.h"
#include "NetworkAddress.h"
using namespace SingularityEngine;
using namespace Networking;



NetworkAddress::NetworkAddress() : m_address(INADDR_ANY), m_port((DEFAULT_PORT))
{
	SetupSockAddress();
}

void NetworkAddress::Set(uint32_t address, unsigned int port)
{
	m_address = address;
	m_port = port;
	SetupSockAddress();

}
void NetworkAddress::Set(char* address, unsigned int port)
{
	m_address = ParseIPV4string(address);
	m_port = port;
	SetupSockAddress();
}

NetworkAddress::~NetworkAddress()
{

}

NetworkAddress::NetworkAddress(uint32_t address, unsigned int port) : m_address(address), m_port(port)
{
	SetupSockAddress();
}

NetworkAddress::NetworkAddress(const char* address, unsigned int port) : m_port(port)
{
	m_address = ParseIPV4string(address);
	SetupSockAddress();
}

NetworkAddress::NetworkAddress(uint8_t a, uint8_t b, uint8_t c, uint8_t d, unsigned int port) : m_address(a << 24 | b << 16 | c << 8 | d), m_port(port)
{
	SetupSockAddress();
}

void NetworkAddress::SetupSockAddress()
{
	m_sockAddress.sin_family = AF_INET;
	m_sockAddress.sin_addr.s_addr = htonl(m_address);
	m_sockAddress.sin_port = htons(static_cast<u_short>(m_port));
}

uint32_t NetworkAddress::ParseIPV4string(const char* ipAddress)
{
	uint32_t value[4] = { 0,0,0,0 };
	size_t index = 0;
	while (*ipAddress) {
		if (isdigit((unsigned char)*ipAddress)) {
			value[index] *= 10;
			value[index] += *ipAddress - '0';
		}
		else {
			index++;
		}
		ipAddress++;

	}

	return value[0] | value[1] << 8 | value[2] << 16 | value[3] << 24;
}
