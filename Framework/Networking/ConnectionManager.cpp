#include "Precompiled.h"
#include "ConnectionManager.h"
using namespace SingularityEngine;
using namespace Networking;

bool ConnectionManager::Initialize(const NetworkAddress address, const unsigned short port, uint32_t appVersionNum)
{
	if (connectionSocket.IsOpen())
		return false;
	connectionSocket.Open(address, port);
	ASSERT(connectionSocket.IsOpen(), "[Networking::ConnectionManager] Failed to Open Port");
	CreateCrcTable();
	//HASH this so that its not just directly the version num,
	//hash it with like the game name or something so its more complicated and harder for an attacker to replicate
	protocolID = appVersionNum;
	return true;
}

void ConnectionManager::AddDataToSendQueue(UpdateData data, bool)
{
	updateDataQueue.emplace_back(data);
}

void ConnectionManager::SendNetworkData(NetworkAddress address, UpdateData data)
{
	std::map<NetworkAddress, ConnectionInfo>::iterator ConnectionIt = connections.find(address);
	ASSERT(ConnectionIt != connections.end(), "[Networking::ConnectionManager] Failed to find address to send to in connected addresses");
	ConnectionInfo* connectInfo = &(ConnectionIt->second);
	connectInfo->localSequenceNumber++;
	data.ackTimeout = PACKET_RESEND_TIME;
	connectInfo->unAckedPackets.emplace(connectInfo->localSequenceNumber, data);
	PacketHeader packtoSend;
	packtoSend.protocolID = protocolID;
	packtoSend.userPacketType = data.userDataType;
	packtoSend.packetData = data.data;
	packtoSend.packetDataSize = data.dataSize;
	packtoSend.sequenceNum = connectInfo->localSequenceNumber;
	packtoSend.ack = connectInfo->remoteSequenceNumber;
	packtoSend.packetType = Packet_Type_A;
	uint32_t ackField = 0;
	for (std::vector<PacketHeader>::iterator it = connectInfo->mostRecentPacketsRecieved.begin(); it != connectInfo->mostRecentPacketsRecieved.end(); it++)
	{
		if (it->sequenceNum >= packtoSend.ack - 32 && it->ack < packtoSend.ack)
		{
			int ackFieldi = packtoSend.ack - it->ack - 1 ;
			ASSERT(ackFieldi < 32, "[Networking::ConnectionManager]AckField iterator was larger than bit field!");
			ackField = ackField | (1 << ackFieldi);
		}
	}
	packtoSend.ackBitField = ackField;
	uint32_t packetSize = packtoSend.GetPacketSize();
	uint8_t* serializedPacket = new uint8_t[packetSize];
	Core::WriteStream preSerializationStream(serializedPacket, packetSize);
	packtoSend.Serialize<Core::WriteStream>(preSerializationStream, serializedPacket, packetSize, crcLookupTable);
	packtoSend.crc32 = packtoSend.CalculateCRC32(serializedPacket, packetSize,packtoSend.protocolID, crcLookupTable);
	std::memset(serializedPacket, 0, packetSize);
	Core::WriteStream serializationStream(serializedPacket, packetSize);
	packtoSend.Serialize<Core::WriteStream>(serializationStream, serializedPacket, packetSize, crcLookupTable);
	connectionSocket.SendData(address, serializedPacket, packetSize);
	SafeDeleteArray(serializedPacket);
}

std::vector<UpdateData> ConnectionManager::RecieveNetworkData(float deltaTime)
{
	std::vector<UpdateData> recievedUpdatedData;
	bool failedToRecieveData = false;
	uint8_t recvBuffer[MAX_PACKET_SIZE];
	NetworkAddress recvAddr;
	while (!failedToRecieveData)
	{
		std::memset(recvBuffer, 0, MAX_PACKET_SIZE);
		uint32_t recvSize = connectionSocket.RecieveData(recvAddr, recvBuffer, MAX_PACKET_SIZE);
		if (recvSize <= 0)
		{
			failedToRecieveData = true;
			break;
		}
		ResetConnectionTimer(recvAddr);
		recievedUpdatedData.emplace_back(ProccessRecievedData(recvAddr, recvBuffer, recvSize));
	}
	ConnectionInfo* connectInfo;
	for (std::map<NetworkAddress, ConnectionInfo>::iterator addressIt = connections.begin(); addressIt != connections.end(); addressIt++)
	{
		connectInfo = &(addressIt->second);
		std::vector<uint32_t> keysToRemove;
		for (std::map<uint32_t, UpdateData>::iterator ackIt = connectInfo->unAckedPackets.begin(); ackIt != connectInfo->unAckedPackets.end(); ackIt++)
		{
			UpdateData* updateInfo= &(ackIt->second);
			updateInfo->ackTimeout -= deltaTime;
			if (updateInfo->ackTimeout <= 0.0f)
			{
				if (updateInfo->resendDataOnTimeout)
				{
					SendNetworkData(addressIt->first, *updateInfo);
				}
				else
				{
					keysToRemove.emplace_back(ackIt->first);
				}
			}
		}
		for (std::vector<uint32_t>::iterator it = keysToRemove.begin();it != keysToRemove.end(); it++)
		{
			connectInfo->unAckedPackets.erase(*it);
		}
	}
	UpdateClientTimers(deltaTime);
	return recievedUpdatedData;
}
UpdateData ConnectionManager::ProccessRecievedData(NetworkAddress address, uint8_t* data, uint32_t dataSize)
{
	std::map<NetworkAddress, ConnectionInfo>::iterator it = connections.find(address);
	ASSERT(it != connections.end(), "[Networking::ConnectionManager] Recieve Packet Data Failed to find address in connected addresses");
	ConnectionInfo* connectInfo = &(it->second);
	PacketHeader packetRecieved;
	//todo deserializeData
	uint8_t serializedPacket[MAX_PACKET_SIZE];
	Core::ReadStream serializationStream(serializedPacket, MAX_PACKET_SIZE);
	packetRecieved.Serialize(serializationStream, data, dataSize, crcLookupTable);
	if(packetRecieved.sequenceNum > connectInfo->remoteSequenceNumber)
		connectInfo->remoteSequenceNumber = packetRecieved.sequenceNum;

	std::map<uint32_t, UpdateData>::iterator ackIt = connectInfo->unAckedPackets.find(packetRecieved.ack);
	if (ackIt != connectInfo->unAckedPackets.end())
	{
		connectInfo->unAckedPackets.erase(ackIt);
	}
	
	for (int i = 0; i < ACKFIELDSIZE; i++)
	{
		bool bitActive = (packetRecieved.ackBitField & (1 << i));
		if (bitActive)
		{
			uint32_t key = (packetRecieved.ack - 1) - i;
			connectInfo->unAckedPackets.erase(key);
		}
	}
	UpdateData dataRecieved;
	dataRecieved.data = packetRecieved.packetData;
	dataRecieved.userDataType = packetRecieved.userPacketType;
	dataRecieved.dataSize = packetRecieved.packetDataSize;
	connectInfo->mostRecentPacketsRecieved.emplace_back(packetRecieved);
	while (connectInfo->mostRecentPacketsRecieved.size() > 32)
	{
		connectInfo->mostRecentPacketsRecieved.erase(connectInfo->mostRecentPacketsRecieved.begin());
	}
	return dataRecieved;
}

void ConnectionManager::CreateCrcTable()
{
	uint32_t remainder;
	unsigned char b = 0;
	do 
	{
		remainder = b;
		for (int bit = 8; bit > 0; --bit)
		{
			if (remainder & 1)
			{
				remainder = (remainder >> 1) ^ POLYNOMIAL;
			}
			else
			{
				remainder = remainder >> 1;
			}
			crcLookupTable[(size_t)b] = remainder;
		}
	} while (0 != ++b);
}

bool ConnectionManager::IsConnected(NetworkAddress address)
{
	std::map<NetworkAddress, ConnectionInfo>::iterator it = connections.find(address);
	if (it != connections.end())
		return true;
	else
		return false;
}

void ConnectionManager::ResetConnectionTimer(NetworkAddress address)
{
	std::map<NetworkAddress, ConnectionInfo>::iterator it = connections.find(address);
	if (it != connections.end())
	{
		it->second.connectionTimeout = 1.0f;
	}
}

void ConnectionManager::AddClient(NetworkAddress address)
{
	if (IsConnected(address))
	{
		return;
	}
	ConnectionInfo newConnectionInfo;
	newConnectionInfo.connectionTimeout = 1.0f;
	newConnectionInfo.localSequenceNumber = 0;
	newConnectionInfo.remoteSequenceNumber = 0;
	connections.emplace(address, newConnectionInfo);
}

void ConnectionManager::UpdateClientTimers(float deltaTime)
{
	std::vector<NetworkAddress> disconnectedClients;
	for (std::map<NetworkAddress, ConnectionInfo>::iterator it = connections.begin(); it != connections.end(); it++)
	{
		it->second.connectionTimeout -= deltaTime;
		if (it->second.connectionTimeout <= 0.0f)
		{
			//Add a send Disconnection message just in case, might want to eventually add a way to know that clients have disconnected
			disconnectedClients.emplace_back(it->first);
		}
	}
	for (std::vector<NetworkAddress>::iterator discIt = disconnectedClients.begin(); discIt != disconnectedClients.end(); discIt++)
	{
		connections.erase(*discIt);
	}
}

void ConnectionManager::SendPacketsToClients()
{
	for (std::vector<UpdateData>::iterator it = updateDataQueue.begin(); it != updateDataQueue.end(); it++)
	{
		for (std::map<NetworkAddress, ConnectionInfo>::iterator clientIt = connections.begin(); clientIt != connections.end(); clientIt++)
		{
			SendNetworkData(clientIt->first, *it);
		}
	}
}
