#include "Precompiled.h"
#include "Bitpacker.h"

using namespace SingularityEngine;
using namespace SingularityEngine::Core;
//======================
//==    Bit Writer    ==
//======================
SingularityEngine::Core::BitWriter::BitWriter(void* data, int dataByteSize)
{
	ASSERT(data, "[BitWriter]Failed to pass Data to Bit writer");
	ASSERT(dataByteSize % 4 == 0, "[BitWriter] size of data in bytes its not divisible by 4!");
	m_numWords = (dataByteSize / 4);
	m_data = (uint32_t*)data;
	m_numBits = m_numWords * 32;
	m_scratch = 0;
	m_bitIndex = 0;
	m_wordIndex = 0;
	m_bitsWritten = 0;
	m_overflow = false;
}

void SingularityEngine::Core::BitWriter::WriteBits(uint32_t value, int valueBitSize)
{
	ASSERT(valueBitSize > 0, "[BitWriter] Value to small");
	ASSERT(valueBitSize <= 32, "[BitWriter] Value too large");
	ASSERT(m_bitsWritten + valueBitSize <= m_numBits, "[BitWriter] Value Would overflow buffer!");
		
	if (m_bitsWritten + valueBitSize > m_numBits)
	{
		m_overflow = true;
		return;
	}

	//ensure the value is the correct binary placement
	value &= (uint64_t(1) << valueBitSize) - 1;
	//or the value shift by the total size of 
	//scratch - current bit index and the bits 
	//so its placed correctly from left to right
	m_scratch |= (uint64_t)(value) << (64 - m_bitIndex - valueBitSize);
	m_bitIndex += valueBitSize;
	if (m_bitIndex >= 32)
	{
		ASSERT(m_wordIndex < m_numWords, "[BitWriter] somehow passed word index Fatal!");
		m_data[m_wordIndex] = Network_To_Host_Byte_Order(m_scratch >> 32);
		m_scratch <<= 32;
		m_bitIndex -= 32;
		m_wordIndex++;
	}

	m_bitsWritten += valueBitSize;
}

bool SingularityEngine::Core::BitWriter::WriteAlign()
{
	const int remainderBits = m_bitsWritten % 8;
	if (remainderBits != 0)
	{
		uint32_t zero = 0;
		WriteBits(zero, 8 - remainderBits);
		ASSERT(m_bitsWritten % 8 == 0, "[BitWriter] bits its not aligned!");
	}
	return true;
}

void SingularityEngine::Core::BitWriter::WriteBytes(const uint8_t* data, int databyteSize)
{
	ASSERT(GetAlignBits() == 0,"[BitWriter] bits its not aligned!");
	if (m_bitsWritten + databyteSize * 8 >= m_numBits)
	{
		m_overflow = true;
		return;
	}
	ASSERT(m_bitIndex == 0 || m_bitIndex == 8 || m_bitIndex == 16 || m_bitIndex == 24, "[BitWriter] bits its not aligned!");

	//write Head
	int headBytes = (4 - m_bitIndex / 8) % 4;

	if (headBytes > databyteSize)
		headBytes = databyteSize;

	for (int i = 0; i < headBytes; ++i)
		WriteBits(data[i], 8);

	if (headBytes == databyteSize)
		return;

	ASSERT(GetAlignBits() == 0, "[BitWriter] bits its not aligned!");
	//Write Body
	int numWords = (databyteSize - headBytes) / 4;
	if (numWords > 0)
	{
		ASSERT(m_bitIndex == 0, "[BitWriter] Bits not aligned!");
		memcpy(&m_data[m_wordIndex], data + headBytes, numWords * 4);
		m_bitsWritten += numWords * 32;
		m_wordIndex += numWords;
		m_scratch = 0;
	}
	//Write tail
	ASSERT(GetAlignBits() == 0, "[BitWriter] bits its not aligned!");
	int tailStart = headBytes + numWords * 4;
	int tailBytes = databyteSize - tailStart;
	ASSERT(tailBytes >= 0 && tailBytes < 4, "[BitWriter] bits its not aligned!");
	for (int i = 0; i < tailBytes; i++)
	{
		WriteBits(data[tailStart + i], 8);
	}
	ASSERT(GetAlignBits() == 0, "[BitWriter] bits its not aligned!");
	ASSERT(headBytes + numWords * 4 + tailBytes == databyteSize, "[BitWriter] bits its not aligned!");

}

void SingularityEngine::Core::BitWriter::FlushBits()
{
	if (m_bitIndex != 0)
	{
		ASSERT(m_wordIndex < m_numWords, "[BitWriter] word index is larger than the number of words!");
		if (m_wordIndex >= m_numWords)
		{
			m_overflow = true;
			return;
		}
		m_data[m_wordIndex++] = Host_To_Network_Byte_Order(uint32_t(m_scratch >> 32));
	}
}



//======================
//==    Bit Reader    ==
//======================
SingularityEngine::Core::BitReader::BitReader(const void* data, int dataByteSize)
{
	ASSERT(data, "[BitWriter]data was not passed!");
	ASSERT(dataByteSize % 4 == 0, "[BitWriter] bits its not aligned!");
	m_numWords = (dataByteSize / 4);
	m_data = (const uint32_t*)data;
	m_numBits = m_numWords * 32;
	m_scratch = Network_To_Host_Byte_Order(m_data[0]);
	m_bitIndex = 0;
	m_wordIndex = 0;
	m_bitsRead = 0;
	m_overflow = false;
}

uint32_t SingularityEngine::Core::BitReader::ReadBits(int bits)
{
	ASSERT(bits > 0, "[BitReader] bits less than 0!");
	ASSERT(bits <= 32, "[BitReader] bits greater than 0!");
	ASSERT(m_bitsRead + bits <= m_numBits, "[BitReader] value would overflow!");

	if (m_bitsRead + bits > m_numBits)
	{
		m_overflow = true;
		return 0;
	}

	m_bitsRead += bits;
	ASSERT(m_bitIndex < 32, "[BitReader] bitindex passed maximum!");
	if (m_bitIndex + bits < 32)
	{
		m_scratch <<= bits;
		m_bitIndex += bits;
	}
	else
	{
		m_wordIndex++;
		ASSERT(m_wordIndex < m_numWords, "[BitReader] value would overflow!");
		const uint32_t a = 32 - m_bitIndex;
		const uint32_t b = bits - a;
		m_scratch <<= a;
		m_scratch |= Network_To_Host_Byte_Order(m_data[m_wordIndex]);
		m_scratch <<= b;
		m_bitIndex = b;
	}
	const uint32_t output = uint32_t(m_scratch >> 32);
	m_scratch &= 0xFFFFFFFF;
	return output;
}

bool SingularityEngine::Core::BitReader::ReadAlign()
{
	const int remainderBits = m_bitsRead % 8;
	if(remainderBits != 0)
	{
		uint32_t value = ReadBits(8 - remainderBits);
		ASSERT(m_bitsRead % 8 == 0, "[BitReader] Bits not aligned!");
		if (value != 0)
			return false;
	}
	return true;
}

void SingularityEngine::Core::BitReader::ReadBytes(uint8_t * data, int dataByteSize)
{
	ASSERT(GetAlignBits() == 0, "[BitReader] Bits not aligned!");
	if ((m_bitsRead + dataByteSize * 8 ) >= m_numBits)
	{
		memset(data, dataByteSize, 0);
		m_overflow = true;
		return;
	}

	ASSERT(m_bitIndex == 0 || m_bitIndex == 8 || m_bitIndex == 16 || m_bitIndex == 24, "[BitReader] Bits not aligned!");
	
	//Read Head
	int headBytes = (4 - m_bitIndex / 8) % 4;
	if (headBytes > dataByteSize)
		headBytes = dataByteSize;

	for (int i = 0; i < headBytes; ++i)
		data[i] = static_cast<uint8_t>(ReadBits(8));

	if (headBytes == dataByteSize)
		return;

	ASSERT(GetAlignBits() == 0, "[BitReader] Bits not aligned!");

	//Read body

	int numWords = (dataByteSize - headBytes) / 4;
	if (numWords > 0)
	{
		ASSERT(m_bitIndex == 0, "[BitReader] Bits not aligned!");
		memcpy(data + headBytes, &m_data[m_wordIndex], numWords * 4);
		m_bitsRead += numWords * 32;
		m_wordIndex += numWords;
		m_scratch = Network_To_Host_Byte_Order(m_data[m_wordIndex]);
	}

	//read tail
	ASSERT(GetAlignBits() == 0, "[BitReader] Bits not aligned!");
	int tailStart = headBytes + numWords * 4;
	int tailBytes = dataByteSize - tailStart;
	ASSERT(tailBytes >= 0 && tailBytes < 4, "[BitReader] Bits not aligned!");
	for (int i = 0; i < tailBytes; i++)
	{
		data[tailStart + i] = static_cast<uint8_t>(ReadBits(8));
	}
	ASSERT(GetAlignBits() == 0, "[BitReader] Bits not aligned!");
	ASSERT(headBytes + numWords * 4 + tailBytes == dataByteSize, "[BitReader] Bits not aligned!");
}
