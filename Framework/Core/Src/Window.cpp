#include "precompiled.h"
#include "Window.h"
#include "Debug.h"

#ifdef SINGULARITY_USE_GLFW
#include "GLFWWindow.h"
#elif defined SINGULARITY_PLATFORM_WINDOWS
#include "WindowsWindow.h"
#endif

namespace SingularityEngine::Core
{

	std::unique_ptr<Window> Window::create(const WindowProperties& props)
	{
		#ifdef SINGULARITY_USE_GLFW
		return std::make_unique<WindowGLFW>(props);
		#elif defined SINGULARITY_PLATFORM_WINDOWS
		return std::make_unique<WindowsWindow>(props);
		#else
		ASSERT(false, "UNKNOWN PLATFORM");
		return nullptr;
		#endif
	}

}
