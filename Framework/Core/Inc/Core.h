#ifndef INCLUDED_CORE_H
#define INCLUDED_CORE_H

#include "Common.h"
#include "Application.h"
#include "AppInfoManaager.h"
#include "Bitpacker.h"
#include "BitSerialization.h"
#include "BlockAllocator.h"
#include "Buffer.h"
#include "Debug.h"
#include "DeleteUtil.h"
#include "ECS.h"
#include "ECSComponentArray.h"
#include "ECSComponentManager.h"
#include "ECSCoordinator.h"
#include "ECSEntityManager.h"
#include "ECSSystem.h"
#include "MemoryUtil.h"
#include "OperatingSystemAgnostic.h"
#include "Timer.h"
#include "Random.h"
#include "hashids.h"
#include "Handle.h"
#include "HandlePool.h"
#include "Meta.h"
#include "MetaType.h"
#include "MetaClass.h"
#include "MetaField.h"
#include "MetaUtil.h"
#include "MetaRegistration.h"
#include "MetaRegistry.h"
#include "SerializationUtil.h"
#include "Window.h"
#include "InputManager.h"
#include "Layer.h"
#include "LayerStack.h"
#include "stb_image.h"

#ifndef SINGULARITY_USE_GLFW
#ifdef SINGULARITY_PLATFORM_WINDOWS
#include "WindowsWindow.h"
#include "InputManagerWindows.h"
#endif
#else
#include "GLFWWindow.h"
#include "InputManagerGLFW.h"
#endif


#define SINGULARITY_ENGINE_MAJOR_VERSION 0
#define SINGULARITY_ENGINE_MINOR_VERSION 2
#define SINGULARITY_ENGINE_PATCH_VERSION 2
#define SINGULARITY_ENGINE_NAME "Singularity Engine"
#endif //INCLUDED_CORE_H