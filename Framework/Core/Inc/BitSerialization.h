#ifndef INCLUDED_BIT_SERIALIZATION_HEADER_
#define INCLUDED_BIT_SERIALIZATION_HEADER_
#include "Common.h"
#include "Bitpacker.h"
namespace SingularityEngine
{
	namespace Core
	{



		class WriteStream
		{
		public:
			enum { IsReading = 0 };
			enum { IsWriting = 1 };
			WriteStream(uint8_t* buffer, uint32_t bufSize) :m_writer(buffer, bufSize) {}

			bool SerializeInteger(int32_t value, int32_t min, int32_t max)
			{
				ASSERT(min < max, "[Networking:WriteSerializer] Min Was less than max when serializing integer!");
				ASSERT(value >= min, "[Networking:WriteSerializer] value Was less than min when serializing integer!");
				ASSERT(value <= max, "[Networking:WriteSerializer] value Was less than max when serializing integer!");
				const int bits = bits_required(min, max);
				uint32_t unsigned_value = value - min;
				m_writer.WriteBits(unsigned_value, bits);
				return true;
			}

			bool SerializeBits(uint32_t value, int bits)
			{
				ASSERT(bits > 0,"[WriteStream] bits < 0");
				ASSERT(bits <= 32, "[WriteStream] bits > 32");
				m_writer.WriteBits(value, bits);
				return true;

			}
			bool SerializeBytes(uint8_t*data, int bytes)
			{
				SerializeAlign();
				m_writer.WriteBytes(data, bytes);
				return true;
			}
			void CompleteWrite() { m_writer.FlushBits(); }
			const uint8_t* GetData() { return m_writer.GetData(); }
			bool SerializeAlign() { m_writer.WriteAlign(); return true; }
		private:
			BitWriter m_writer;
		};

		class ReadStream
		{
		public:
			enum { IsReading = 1 };
			enum { IsWriting = 0 };
			ReadStream(uint8_t* buffer, uint32_t bufSize) : m_reader(buffer, bufSize) {}

			bool SerializeInteger(int32_t &value, int32_t min, int32_t max)
			{
				ASSERT(min < max, "[ReadStream] Max is greater than min");
				const int bits = bits_required(min, max);
				if (m_reader.WouldOverFlow(bits))
				{
					return false;
				}
				uint32_t unsignedValue = m_reader.ReadBits(bits);
				value = (int32_t)(unsignedValue + min);
				return true;
			}
			bool SerializeBits(uint32_t& value, size_t bits)
			{
				ASSERT(bits > 0, "[ReadStream] bits < 0");
				ASSERT(bits <= 32, "[ReadStream] bits > 31");
				uint32_t readValue = m_reader.ReadBits( (int)bits);
				value = readValue;
				return true;
			}
			bool SerializeBytes(uint8_t* data, int bytes)
			{
				SerializeAlign();
				m_reader.ReadBytes(data, bytes);
				return true;
			}
			const uint8_t* GetData() { return m_reader.GetData(); }

			bool SerializeAlign() { return m_reader.ReadAlign(); }
		private:
			BitReader m_reader;
		};

#define serialize_int(stream, value, min ,max)																					\
		do																														\
		{																														\
			ASSERT(min < max,"[Networking:WriteSerializer] Min Was less than max when serializing integer!");				\
			int32_t normVal;																									\
			if (Stream::IsWriting)																								\
			{																													\
				ASSERT(value >= min,"[Networking:WriteSerializer] value Was less than min when serializing integer!");	\
				ASSERT(value <= max,"[Networking:WriteSerializer] value Was less than min when serializing integer!");	\
				normVal = value;																						\
			}																													\
				if (!stream.SerializeInteger(normVal, min, max))																\
				{																												\
					return false;																								\
				}																												\
					if (Stream::IsReading)																						\
					{																											\
						value = normVal;																						\
						if (value < min || value > max)																			\
						{																										\
							return false;																						\
						}																										\
					}																											\
		} while (0)

#define serialize_bits(stream, value, bits)																			\
		do 																											\
		{																											\
			ASSERT(bits > 0, "[Networking::Serializer] bits was less than 1");								\
			ASSERT(bits <= 32, "[Networking::Serializer] bits larger than 32");								\
			uint32_t bitValue = 0;																					\
			if(Stream::IsWriting)																					\
			{																										\
				bitValue = (uint32_t)value;																			\
			}																										\
			stream.SerializeBits(bitValue, bits);																	\
			if(Stream::IsReading)																					\
			{																										\
				value = (decltype(value))bitValue;																	\
			}																										\
		} while (0)


		template<typename Stream> bool Serialize_bool(Stream& stream, bool& val)
		{
			serialize_bits(stream, val, 1);
			return true;
		}

		template<typename Stream, typename Value>bool Serialize_Int(Stream& stream, Value& val)
		{
			if (std::is_integral<Value>::value)
			{
				int size = sizeof(Value);
				size *= 8;
				if (size <= 32)
				{
					serialize_bits(stream, val, size);
					if (std::is_signed<Value>::value && Stream::IsReading)
					{
						val = (int)val;
					}
					return true;
				}
				else
				{
					uint32_t lo;
					uint32_t hi;
					if (Stream::IsWriting)
					{
						lo = val & 0xFFFFFFFF;
						hi = static_cast<uint64_t>(val) >> 32;
					}
					serialize_bits(stream, hi, 32);
					serialize_bits(stream, lo, 32);
					if (Stream::IsReading)
					{
						val = uint64_t(hi) << 32 | lo;

					}
				}
					return true;
			}
		}

		template<typename Stream> bool Serialize_Int_Compressed(Stream& stream, int& val, int min, int max)
		{
			serialize_int(stream, val, min, max);
			return true;
		}
		template<typename Stream> bool Serialize_Int_Compressed(Stream& stream, uint32_t& val, unsigned int min, unsigned int max)
		{
			serialize_int(stream, val, min, max);
			return true;
		}

		template<typename Stream, typename Value> bool Serialize_Int(Stream& stream, uint16_t& val)
		{
			serialize_bits(stream, val, 16);
			return true;
		}
		template<typename Stream, typename Value> bool Serialize_Int(Stream& stream, uint32_t& val)
		{
			serialize_bits(stream, val, 32);
			return true;
		}
		template<typename Stream, typename Value> bool Serialize_Int(Stream& stream, uint64_t& val)
		{
			uint32_t lo;
			uint32_t hi;
			if (Stream::IsWriting)
			{
				lo = val & 0xFFFFFFFF;
				hi = val >> 32;
			}
			serialize_bits(stream, hi, 32);

			serialize_bits(stream, lo, 32);
			if (Stream::IsReading)
			{
				val = uint64_t(hi) << 32 | lo;
			}
			return true;
		}

		template<typename Stream, typename Value> bool Serialize_Int(Stream& stream, int16_t& val)
		{
			serialize_bits(stream, val, 16);
			return false;
		}
		template<typename Stream, typename Value> bool Serialize_Int(Stream& stream, int32_t& val)
		{
			serialize_bits(stream, val, 32);
			return false;

		}
		template<typename Stream, typename Value> bool Serialize_Int(Stream& stream, int64_t& val)
		{
			uint32_t lo;
			uint32_t hi;
			if (Stream::IsWriting)
			{
				lo = uint64_t(val) & 0xFFFFFFFF;
				hi = uint64_t(val) >> 32;
			}
			serialize_bits(stream, hi, 32);
			serialize_bits(stream, lo, 32);
			if (Stream::IsReading)
			{
				val = int64_t(hi) << 32 | lo;
			}
			return true;
		}

		template<typename Stream> bool Serialize_Float(Stream& stream, float& val)
		{
			union FloatInt
			{
				float float_value;
				uint32_t int_value;
			};
			FloatInt tmp;
			if (Stream::IsWriting)
			{
				tmp.float_value = val;
			}
			bool result = stream.SerializeBits(tmp.int_value, 32);
			if (Stream::IsReading)
			{
				val = tmp.float_value;
			}
			return result;
		}

		template<typename Stream> bool Serialize_Compressed_Float(Stream& stream, float& value, float min, float max, float precision)
		{
			ASSERT(max > min, "[Networking:WriteSerializer] max Was less than min when serializing Float!");
			ASSERT(value >= min, "[Networking:WriteSerializer] value Was less than min when serializing Float!");
			ASSERT(value <= max, "[Networking:WriteSerializer] value Was less than min when serializing Float!");
			const float delta = max - min;
			const float values = delta / precision;
			const uint32_t maxIntegerValue = (uint32_t )ceil(values);
			const size_t bits = bits_required(0, maxIntegerValue);
			uint32_t integerValue = 0;
			if (Stream::IsWriting)
			{
				float normalizedValue = (value - min)/ delta;
				if (normalizedValue < 0.0f)
				{
					normalizedValue = 0.0f;
				}
				if (normalizedValue > 1.0f)
				{
					normalizedValue = 1.0f;
				}
				integerValue = (uint32_t) std::floor(normalizedValue*maxIntegerValue + 0.5f);
			}
			if (!stream.SerializeBits(integerValue, bits))
			{
				return false;
			}
			if (Stream::IsReading)
			{
				const float normalizedValue = integerValue / (float)maxIntegerValue;
				value = normalizedValue * delta + min;
			}
			return true;
		}

		template<typename Stream> bool Serialize_Bytes(Stream& stream, uint8_t* data, size_t buffSize)
		{
			ASSERT(buffSize > 0, "[Networking::Serializer] buffersize was less than 1");
			bool result = stream.SerializeBytes(data, (int)buffSize);
			return result;
		}

		template<typename Stream> bool Serialize_String(Stream& stream, char* string, int buffSize)
		{
			ASSERT(buffSize > 0, "[Networking::Serializer] buffersize was less than 1");
			int length;
			if (Stream::IsWriting)
			{
				length = (int)std::strlen(string);
				assert(length < buffSize - 1);
			}
			bool result;
			result = Serialize_Int_Compressed(stream, length, 0, buffSize - 1);
			if (!result)
			{
				return false;
			}
			result = Serialize_Bytes(stream,(uint8_t*) string, length);
			if (!result)
			{
				return false;
			}
			if (Stream::IsReading)
			{
				string[length] = '\0';
			}
			return true;
		}

		template<typename Stream> bool Serialize_String(Stream& stream, std::string& string, size_t largestExpectedSize)
		{
			if (string.length() == 0)
			{
				string = "empty";
			}
			size_t buffSize = largestExpectedSize;
			char* buffer = new char[buffSize];
			strcpy_s(buffer, buffSize, string.c_str());
			if (!(Serialize_String(stream, buffer,(int) buffSize)))
			{
				return false;
			}
			if (Stream::IsReading)
			{
				string = buffer;
			}
			return true;
		}


		//class ISerializable
		//{
		//public:
		//	virtual uint64_t CalcByteSize() = 0;

		//	 template <typename Stream> virtual bool Serialize(Stream& stream) = 0;
		//};
	}


}

#endif