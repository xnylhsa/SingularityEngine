#ifndef INCLUDED_CORE_H
#define INCLUDED_CORE_H

#include "Common.h"
#include "Application.h"
#include "AppInfoManaager.h"
#include "Bitpacker.h"
#include "BitSerialization.h"
#include "BlockAllocator.h"
#include "Debug.h"
#include "DeleteUtil.h"
#include "ECS.h"
#include "ECSComponentArray.h"
#include "ECSComponentManager.h"
#include "ECSCoordinator.h"
#include "ECSEntityManager.h"
#include "ECSSystem.h"
#include "GLFWWindow.h"
#include "MemoryUtil.h"
#include "OperatingSystemAgnostic.h"
#include "Window.h"
#include "WindowsWindow.h"
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

#define SINGULARITY_ENGINE_MAJOR_VERSION 0
#define SINGULARITY_ENGINE_MINOR_VERSION 2
#define SINGULARITY_ENGINE_PATCH_VERSION 2
#define SINGULARITY_ENGINE_NAME "Singularity Engine"
#endif //INCLUDED_CORE_H