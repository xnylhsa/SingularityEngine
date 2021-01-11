//====================================================================================================
// Filename:	NetworkDataTypes.h
// Created by:	Ashlyn Gadow
//====================================================================================================

#ifndef INCLUDED_NETWORK_DATA_TYPES_HEADER_
#define INCLUDED_NETWORK_DATA_TYPES_HEADER_

#define MAX_GAME_PACKET_SIZE (1024*1024)
#define MAX_DATA_SIZE 512
#include "Common.h"
namespace SingularityEngine
{
	namespace Networking
	{
		enum PacketInfo
		{
			Fragment = 0,
			Packet_Type_A = 1,
			Packet_Type_B = 2,
			Packet_Type_C = 3,
			MESSAGE_NUM_MAX = 4,
		};
		class PacketHeader
		{
		public:
			uint32_t protocolID; // Make something that is unique, some hash representing the version of the game
			uint32_t crc32;
			PacketInfo packetType; //Update Includes Information, Ack(knowledge) is a short response to confirm receipt of information
			uint32_t sequenceNum;
			uint32_t ack;
			uint32_t ackBitField; // size up to this point is 4 + 4 + 2bits + 4 + 4 + 4  + 11bits +1 + userdata | 28 bytes + 1 bit + userdata
			uint32_t packetDataSize;
			uint8_t* packetData;
			uint8_t userPacketType; 
			uint32_t CalculateCRC32(uint8_t* data, uint32_t dataSize, uint32_t protoID, uint32_t* crc32LookUpTable)
			{
				uint32_t crc32calc = 0xFFFFFFFF;
				//Use ProtocolId to sign packet so no one can mess with it, Fuckers 
				for (size_t i = 0; i < 4; i++)
				{
					uint8_t protoByte = (protoID >>(8*i)) & 0xFF;
					uint32_t index = (crc32calc ^ protoByte) & 0xFF;
					crc32calc = (crc32calc >> 8) ^ crc32LookUpTable[index];
				}
				//Redundancy check on the rest of the packet
				for (size_t i = 0; i < dataSize; i++ )
				{
					uint32_t index = (crc32calc ^ data[i]) & 0xFF;
					crc32calc = (crc32calc >> 8) ^ crc32LookUpTable[index];
				}
				return crc32calc;
			}
			template<typename Stream> bool Serialize(Stream stream, uint8_t* data, uint32_t dataSize, uint32_t* crc32LookUpTable)
			{
				Serialize_Int(stream, crc32);
				if(Stream::IsReading )
				{
					uint32_t calcCRC32 = CalculateCRC32(data, dataSize, protocolID, crc32LookUpTable);
					if (calcCRC32 != crc32)
					{
						return false;
					}
				}
				serialize_bits(stream, packetType, 3);
				Serialize_Int(stream, sequenceNum);
				Serialize_Int(stream, ack);
				Serialize_Int(stream, ackBitField);
				Serialize_Int_Compressed(stream, packetDataSize,0, 1500);
				Serialize_Bytes(stream, (uint8_t*)packetData, packetDataSize);
				serialize_bits(stream, userPacketType, 8);
				return true;
			}
			int GetPacketSize()
			{
				return packetDataSize + 8 + 4 + sizeof(PacketInfo) + 4 + 4 + 4 + 4 + 4;
			}
		};


	}
}


#endif