#ifndef INCLUDED_BIT_PACKER_
#define INCLUDED_BIT_PACKER_
#include "Common.h"
#include "Debug.h"
#include "MemoryUtil.h"


namespace SingularityEngine
{
	namespace Core
	{
		class BitWriter
		{
		public:
			BitWriter();
			BitWriter(void* data, int dataByteSize);
			void WriteBits(uint32_t value, int valueBitSize);
			bool WriteAlign();
			void WriteBytes(const uint8_t* data, int databyteSize);
			void FlushBits();
			int GetAlignBits() const
			{
				return (8 - m_bitsWritten % 8) % 8;
			}
			int GetBitsWritten() const
			{
				return m_bitsWritten;
			}
			int GetBitsAvaliable() const
			{
				return m_numBits - m_bitsWritten;
			}
			const uint8_t* GetData() const
			{
				return (uint8_t*)m_data;
			}
			int GetBytesWritten() const
			{
				//word size is uint32_t so multiply index by 4 for num bytes
				return m_wordIndex * 4;
			}
			int GetTotalByes() const
			{
				return m_numWords * 4;
			}
			bool IsOverFlow() const
			{
				return m_overflow;
			}
		private:
			uint32_t * m_data;
			uint64_t m_scratch;
			int m_numBits;
			int m_numWords;
			int m_bitsWritten;
			int m_bitIndex;
			int m_wordIndex;
			bool m_overflow;
		};

		class BitReader
		{
		public:
			BitReader();
			BitReader(const void* data, int dataByteSize);
			uint32_t ReadBits(int bits);
			bool ReadAlign();
			void ReadBytes(uint8_t * data, int dataByteSize);

			int GetAlignBits() const
			{
				return (8 - m_bitsRead % 8) % 8;
			}
			int GetBitsRead() const
			{
				return m_bitsRead;
			}
			int GetBitsRemaining() const
			{
				return m_numBits - m_bitsRead;
			}
			const uint8_t* GetData() const
			{
				return (uint8_t*)m_data;
			}
			int GetTotalByes() const
			{
				return m_numBits * 8;
			}
			bool WouldOverFlow(int bits) 
			{
				return( m_bitsRead + bits > m_numBits) ? true : false;
			}
			bool IsOverFlow() const
			{
				return m_overflow;
			}
		private:
			const uint32_t* m_data;
			uint64_t m_scratch;
			int m_numBits;
			int m_numWords;
			int m_bitsRead;
			int m_bitIndex;
			int m_wordIndex;
			bool m_overflow;
		};
	}
}

#endif // 
