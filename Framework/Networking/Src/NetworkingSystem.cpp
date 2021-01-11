#include "Precompiled.h"
#include "NetworkingSystem.h"
#include "NetworkUtil.h"
using namespace SingularityEngine;
using namespace Networking;

bool NetworkSystem::StaticInitialize()
{
	if (!networkInitialized)
	{
		int iResult;
		WSADATA m_wsaData;
		iResult = WSAStartup(MAKEWORD(2, 2), &m_wsaData);
		if (iResult != 0)
		{
			std::cout << "WSAStatup Failed with error " << NET_ErrorString() << " iResult was:" << iResult << std::endl;
			WSACleanup();
			return false;
		}
		networkInitialized = true;
		return true;
	}
	return false;
}
void NetworkSystem::StaticTerminate()
{
	if (networkInitialized)
	{
		networkInitialized = false;
		WSACleanup();

	}
}
NetworkSystem* NetworkSystem::Get()
{
	return nullptr;

}

