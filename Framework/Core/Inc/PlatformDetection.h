#ifndef SINGULARITY_PLATFORM_DETECTION
#define SINGULARITY_PLATFORM_DETECTION

// Platform detection using predefined macros
#ifdef _WIN32
	/* Windows x64/x86 */
#ifdef _WIN64
	/* Windows x64  */
#define SINGULARITY_PLATFORM_WINDOWS
#else
	/* Windows x86 */
#error "x86 Builds are not supported!"
#endif
#elif defined(__APPLE__) || defined(__MACH__)
#include <TargetConditionals.h>
/* TARGET_OS_MAC exists on all the platforms
 * so we must check all of them (in this order)
 * to ensure that we're running on MAC
 * and not some other Apple platform */
#if TARGET_IPHONE_SIMULATOR == 1
#error "IOS simulator is not supported!"
#elif TARGET_OS_IPHONE == 1
#define SINGULARITY_PLATFORM_IOS
#error "IOS is not supported!"
#elif TARGET_OS_MAC == 1
#define SINGULARITY_PLATFORM_MACOS
#error "MacOS is not supported!"
#else
#error "Unknown Apple platform!"
#endif
 /* We also have to check __ANDROID__ before __linux__
  * since android is based on the linux kernel
  * it has __linux__ defined */
#elif defined(__ANDROID__)
#define SINGULARITY_PLATFORM_ANDROID
#error "Android is not supported!"
#elif defined(__linux__)
#define SINGULARITY_PLATFORM_LINUX
#error "Linux is not supported!"
#else
	/* Unknown compiler/platform */
#error "Unknown platform!"
#endif // End of platform detection

#if _WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

#if __GNUC__
#if __x86_64__ || __ppc64__
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

#endif