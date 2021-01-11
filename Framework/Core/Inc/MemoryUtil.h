#ifndef _INCLUDED_MEMORY_UTILITY_HEADER
#define _INCLUDED_MEMORY_UTILITY_HEADER

#define LITTLE_ENDIAN_ORDER 1
#define BIG_ENDIAN_ORDER 2
#if defined(__386) || defined(i386) || defined(__i386__) || defined(__X86) || defined(_M_IX86) || defined(_M_X64) \
			||defined(__x86_64__) || defined(alpha) || defined(__alpha) || defined(__alpha) || defined(__alpha__) \
			|| defined(_M_ALPHA) || defined(ARM) || defined(__ARM) || defined(__arm__) || defined(WIN32) || defined(_WIN32) \
			|| defined(__WIN32__) || defined(_WIN32_WCE) || defined(__NT__) || defined(__MIPSEL__)
#define ENDIANNESS LITTLE_ENDIAN_ORDER
#else
#define ENDIANNESS BIG_ENDIAN_ORDER
#endif
namespace SingularityEngine
{
	namespace Core
	{

		inline bool File_Exists(const std::string& name) {
			struct stat buffer;
			return (stat(name.c_str(), &buffer) == 0);
		}
		template <uint32_t x> struct PopCount
		{
			enum {
				a = x - ((x >> 1) & 0x55555555),
				b = (((a >> 2) & 0x33333333) + (a & 0x33333333)),
				c = (((b >> 4) + b) & 0x0f0f0f0f),
				d = c + (c >> 8),
				e = d + (d >> 16),

				result = e & 0x0000003f
			};
		};

		template <uint32_t x> struct Log2
		{
			enum {
				a = x | (x >> 1),
				b = a | (a >> 2),
				c = b | (b >> 4),
				d = c | (c >> 8),
				e = d | (d >> 16),
				f = e >> 1,

				result = PopCount<f>::result
			};
		};

		template<uint64_t min, uint64_t max> struct BitsRequired
		{
			static const uint32_t result = (min == max) ? 0 : (log2<min, max>::result + 1);
		};
		inline uint32_t popcount(uint32_t x)
		{
			const uint32_t a = x - ((x >> 1) & 0x55555555);
			const uint32_t b = (((a >> 2) & 0x33333333) + (a & 0x33333333));
			const uint32_t c = (((b >> 4) + b) & 0x0f0f0f0f);
			const uint32_t d = c + (c >> 8);
			const uint32_t e = d + (d >> 16);
			const uint32_t result = e & 0x0000003f;
			return result;
		};


#ifdef __GNUC__

		inline int bits_required(uint32_t min, uint32_t max)
		{
			return 32 - __builtin_clz(max - min);
		}

#else
		inline uint32_t log2(uint32_t x)
		{
			const uint32_t a = x | (x >> 1);
			const uint32_t b = a | (a >> 2);
			const uint32_t c = b | (b >> 4);
			const uint32_t d = c | (c >> 8);
			const uint32_t e = d | (d >> 16);
			const uint32_t f = e >> 1;
			return popcount(f);
		};

		inline int bits_required(uint32_t min, uint32_t max)
		{
			return (min == max) ? 0 : log2(max - min) + 1;
		}
#endif
		inline uint32_t Host_To_Network_Byte_Order(uint32_t val)
		{
#if ENDIANNESS == BIG_ENDIAN_ORDER
			return __builtin_bswap32(val);
#else
			return val;
#endif
		}

		inline uint32_t Network_To_Host_Byte_Order(uint32_t val)
		{
#if ENDIANNESS == BIG_ENDIAN_ORDER
			return __builtin_bswap32(val);
#else
			return val;
#endif
		}
	}
}


#endif // _INCLUDED_NETWORK_UTILITY_HEADER
