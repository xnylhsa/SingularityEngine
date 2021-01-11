#ifndef INCLUDED_CORE_DEBUG_H
#define INCLUDED_CORE_DEBUG_H

#define MULTILINE_MACRO_BEGIN do{
#define MULTILINE_MACRO_END } while(false)
#if defined(_DEBUG)
#define LOG(format, ...)\
	MULTILINE_MACRO_BEGIN\
		char buffer[4096];\
		int ret = _snprintf_s(buffer, _countof(buffer), _TRUNCATE, "%s(%d) "##format, __FILE__, __LINE__, __VA_ARGS__);\
		OutputDebugStringA(buffer);\
		if (ret == -1) OutputDebugStringA("** message truncated **\n");\
		OutputDebugStringA("\n");\
	MULTILINE_MACRO_END

#define ASSERT(condition, format, ...)\
	MULTILINE_MACRO_BEGIN\
		if (!(condition))\
		{\
			LOG(format, __VA_ARGS__);\
			DebugBreak();\
		}\
	MULTILINE_MACRO_END

#define VERIFY(condition, format, ...)\
	MULTILINE_MACRO_BEGIN\
		if (!(condition))\
		{\
			LOG(format, __VA_ARGS__);\
			DebugBreak();\
		}\
	MULTILINE_MACRO_END
#else


#define LOG(format, ...)
#define ASSERT(condition, format, ...)
#define VERIFY(condition, format, ...) (condition);



namespace SingularityEngine::Core
{
	template<typename Base, typename Derived>
	void AssertIsSameOrDerivedFrom()
	{
		Derived * testPtr = new Derived();
		Base* basePtr = testPtr;

		ASSERT(dynamic_cast<Derived*>(basePtr), "[Core::Debug] Faied to Prove that Derived class was the same or derived from the base type!");
	}
}
#endif // #if defined(_DEBUG)

#endif // #ifndef INCLUDED_CORE_DEBUG_H