#ifndef INCLUDED_NETWORK_CONNECTION_MANAGER_
#define INCLUDED_NETWORK_CONNECTION_MANAGER_
#include "NetworkAddress.h"
#include "NetworkSocket.h"
#include "NetworkUtil.h"
#include "NetworkData.h"
#include "Common.h"
#define ACKFIELDSIZE 32
#define POLYNOMIAL 0xEDB88320
#define PACKET_RESEND_TIME 1.0f
namespace SingularityEngine
{
	namespace Networking
	{
		struct UpdateData
		{
			uint8_t userDataType;
			uint32_t dataSize;
			uint8_t* data;
			bool resendDataOnTimeout;
			float ackTimeout;
		};

		struct ConnectionInfo
		{
			float connectionTimeout;
			uint32_t remoteSequenceNumber;
			uint32_t localSequenceNumber;
			std::vector<PacketHeader> mostRecentPacketsRecieved;
			std::map<uint32_t, UpdateData> unAckedPackets;
		};

		class ConnectionManager
		{
		public:
			ConnectionManager() {};
			bool Initialize(const NetworkAddress address, const unsigned short port, uint32_t appVersionNum);
			void AddDataToSendQueue( UpdateData data, bool guarenteeDelivery);
			void SendNetworkData(NetworkAddress address, UpdateData data);
			UpdateData ProccessRecievedData(NetworkAddress address, uint8_t* dataBuffer, uint32_t dataSize);
			std::vector<UpdateData> RecieveNetworkData(float deltaTime);
			std::map<NetworkAddress, ConnectionInfo> connections;
			float connectionDissconnectTime = 10.0f;
			bool IsConnected(NetworkAddress address);
			void ResetConnectionTimer(NetworkAddress address);
			void AddClient(NetworkAddress address);
			void UpdateClientTimers(float deltaTime);
			void SendPacketsToClients();

		private:
			void CreateCrcTable();
			uint32_t crcLookupTable[256];
			uint32_t protocolID;
			std::vector<UpdateData> updateDataQueue;
			NetworkSocket connectionSocket;
		};
	}
}
#endif