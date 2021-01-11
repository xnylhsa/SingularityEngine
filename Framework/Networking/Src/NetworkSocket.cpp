#include "Precompiled.h"
#include "NetworkSocket.h"
#include "NetworkUtil.h"


using namespace SingularityEngine;
using namespace Networking;
NetworkSocket::NetworkSocket()
{

}

NetworkSocket::~NetworkSocket()
{

}

bool NetworkSocket::IsOpen() const
{
	return socketHandle > 0;
}

void NetworkSocket::Close()
{
	closesocket(socketHandle);
	socketHandle = 0;
}

bool NetworkSocket::Open(const NetworkAddress address, const unsigned short)
{
	m_Address = address;
	socketHandle = socket(m_Address.GetAddressFamily(), SOCK_DGRAM, IPPROTO_UDP);
	if (socketHandle == SOCKET_ERROR)
	{
		std::cout << "Bind failed with error" << NET_ErrorString() << std::endl;
		return false;
	}

	if (bind(socketHandle, (const sockaddr*)(m_Address.GetSockAddress()), sizeof(sockaddr_in)) < 0)
	{
		std::cout << "Bind failed with error" << NET_ErrorString() << std::endl;
		Close();
		return false;
	}

	DWORD nonBlocking = 1;
	if (ioctlsocket(socketHandle, FIONBIO, &nonBlocking) != 0)
	{
		std::cout << ("failed to set non-blocking \n") << std::endl;
		return false;
	}
	return true;

}

bool NetworkSocket::SendData(NetworkAddress& destination, const uint8_t* data, int size)
{
	int sent_bytes = sendto(socketHandle,(const char*)data, size, 0, (struct sockaddr*)(destination.GetSockAddress()), sizeof(sockaddr));

	if (sent_bytes != size)
	{
		printf("failed to send packet\n");
		return false;
	}
	return true;
}

int NetworkSocket::RecieveData(NetworkAddress& sender, uint8_t* data, int size)
{
	sockaddr_in from;
	int fromLength = sizeof(from);

	int bytes = recvfrom(socketHandle,
		(char*)data,
		size,
		0,
		(sockaddr*)&from,
		&fromLength);

	if (bytes <= 0)
		return bytes;

	unsigned int a = from.sin_addr.S_un.S_addr;
	unsigned int from_port = ntohs(from.sin_port);
	sender.Set(from_port, a);

	return bytes;
}
