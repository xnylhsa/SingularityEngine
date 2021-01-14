#pragma once
namespace SingularityEngine::Core::OS
{
#if defined(_WIN32) || (_WIN64)
	typedef HMODULE LibraryHandle;

#else
	typedef void* LibraryHandle;
#endif

}