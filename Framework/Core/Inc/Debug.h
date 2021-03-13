#ifndef SINGULARITY_CORE_DEBUG_H
#define SINGULARITY_CORE_DEBUG_H


#pragma warning(push, 2)
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"


namespace SingularityEngine::Core
{
	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}
#pragma warning(pop)


#define MULTILINE_MACRO_BEGIN do{
#define MULTILINE_MACRO_END } while(false)
#define SENGINE_TRACE(...) SingularityEngine::Core::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define SENGINE_INFO(...)  SingularityEngine::Core::Log::GetCoreLogger()->info(__VA_ARGS__)
#define SENGINE_WARN(...)  SingularityEngine::Core::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define SENGINE_ERROR(...) SingularityEngine::Core::Log::GetCoreLogger()->error(__VA_ARGS__)
#define SENGINE_FATAL(...) SingularityEngine::Core::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define SINGULARITY_TRACE(...) SingularityEngine::Core::Log::GetClientLogger()->trace(__VA_ARGS__)
#define SINGULARITY_INFO(...)  SingularityEngine::Core::Log::GetClientLogger()->info(__VA_ARGS__)
#define SINGULARITY_WARN(...)  SingularityEngine::Core::Log::GetClientLogger()->warn(__VA_ARGS__)
#define SINGULARITY_ERROR(...) SingularityEngine::Core::Log::GetClientLogger()->error(__VA_ARGS__)
#define SINGULARITY_FATAL(...) SingularityEngine::Core::Log::GetClientLogger()->critical(__VA_ARGS__)


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