#ifndef STRING_CASTING_HEADER
#define STRING_CASTING_HEADER
#include "Common.h"
namespace
{
	template <typename T>
	struct string_type_of;
	template <>
	struct string_type_of<const char *>
	{
		typedef std::string wrap;
	};
	template <>
	struct string_type_of<const wchar_t*>
	{
		typedef std::wstring wrap;
	};

	template<typename T>
	struct string_traits;

	template<>
	struct string_traits<std::string>
	{
		typedef char char_trait;
		static int byte_convert(const int codepage, LPCSTR data, int data_length, LPWSTR buffer, int buffer_size)
		{
			return ::MultiByteToWideChar(static_cast<UINT>(codepage), 0, data, data_length, buffer, buffer_size);
		}
	};
	template<>
	struct string_traits<std::wstring>
	{
		typedef wchar_t char_trait;
		static int byte_convert(const int codepage, LPCWSTR  data, int data_length, LPSTR buffer, int buffer_size)
		{
			return ::WideCharToMultiByte(static_cast<UINT>(codepage), 0, data, data_length, buffer, buffer_size, NULL, NULL);
		}
	};

	template<typename TO, typename FROM>
	struct string_cast_impl
	{
		static const TO cast(const FROM& source)
		{
			int length = string_traits<FROM>::byte_convert(CP_ACP, source.data(), static_cast<int>(source.length()), NULL, 0);
			if (length == 0)
			{
				return TO();
			}
			std::vector<typename string_traits<TO>::char_trait> buffer(static_cast<unsigned int>(length));
			string_traits<FROM>::byte_convert(CP_ACP, source.data(), static_cast<int>(source.length()), &buffer[0], length);
			return TO(buffer.begin(), buffer.end());
		}
	};

	template<typename TO>
	struct string_cast_impl<TO, TO>
	{
		static const TO& cast(const TO& source)
		{
			return source;
		}
	};
}
namespace SingularityEngine::Core
{



	template<typename TO,typename FROM>
	TO string_cast(const FROM& source)
	{
		return string_cast_impl<TO, FROM>::cast(source);
	}
	template <typename TO, typename FROM>
	TO string_cast(FROM *source)
	{
		return string_cast_impl<TO, typename string_type_of<const FROM *>::wrap >::cast(source);
	}

}
#endif // STRING_CASTING_HEADER
