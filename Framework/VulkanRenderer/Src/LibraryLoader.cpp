#include "Precompiled.h"
#include "LibraryLoader.h"
#include "VulkanFunctions.h"

bool SingularityEngine::Vulkan::LibraryLoader::init()
{
	if (!loadLibrary())
	{
		return false;
	}
	if (!loadExportedEntryPoints())
	{
		return false;
	}
	if (!loadGlobalLevelEntryPoints())
	{
		return false;
	}
	return true;
}

bool SingularityEngine::Vulkan::LibraryLoader::loadDeviceLevelEntryPoints(VkDevice& device)
{
	UNREFERENCED_PARAMETER(device);
#define DEVICE_LEVEL_VULKAN_FUNCTION(name)                                  \
	name = (PFN_##name)vkGetDeviceProcAddr(device, #name);       \
	if (name == nullptr) {                                                    \
		LOG("Could not load device-level Vulkan function named: %s", #name); \
		return false;                                                         \
}

#include "ListOfVulkanFunctions.inl" 

	return true;
}

bool SingularityEngine::Vulkan::LibraryLoader::loadDeviceLevelExtensionEntryPoints(VkDevice& device)
{
	UNREFERENCED_PARAMETER(device);
#define DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION( name, extension )    \
for( auto & enabled_extension : mDesiredDeviceExtensions ) {				        \
  if( std::string( enabled_extension ).compare(std::string( extension )) == 0 )        \
  {																	        \
	name = (PFN_##name)vkGetDeviceProcAddr(device, #name);              \
    if( name == nullptr ) {                                                 \
      LOG("Could not load instance-level Vulkan function named: %s", #name);\
      return false;                                                         \
    }                                                                       \
  }                                                                         \
} 

#include "ListOfVulkanFunctions.inl" 

	return true;
}

bool SingularityEngine::Vulkan::LibraryLoader::loadExportedEntryPoints()
{
#if defined VK_USE_PLATFORM_WIN32_KHR
#define LoadFunction GetProcAddress
#elif defined __linux
#define LoadFunction dlsym
#endif

#define EXPORTED_VULKAN_FUNCTION( name )                          \
name = (PFN_##name)LoadFunction( mVulkanLibrary, #name );         \
if( name == nullptr ) {                                           \
  LOG("Could not load exported Vulkan function named: %s", #name); \
  return false;                                                   \
} 

#include "ListOfVulkanFunctions.inl"
	return true;
}

bool SingularityEngine::Vulkan::LibraryLoader::loadExtensionLevelEntryPoints(VkInstance& instance)
{
	UNREFERENCED_PARAMETER(instance);
#define INSTANCE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION( name, extension )    \
for( auto & enabled_extension : mDesiredExtensions ) {				        \
  if( std::string( enabled_extension ).compare(std::string( extension )) == 0 )        \
  {																	        \
	name = (PFN_##name)vkGetInstanceProcAddr(instance, #name);              \
    if( name == nullptr ) {                                                 \
      LOG("Could not load instance-level Vulkan function named: %s", #name);\
      return false;                                                         \
    }                                                                       \
  }                                                                         \
} 

#include "ListOfVulkanFunctions.inl" 

	return true;
}

bool SingularityEngine::Vulkan::LibraryLoader::loadGlobalLevelEntryPoints()
{
#define GLOBAL_LEVEL_VULKAN_FUNCTION( name )                      \
name = (PFN_##name)vkGetInstanceProcAddr( nullptr, #name );       \
if( name == nullptr ) {                                           \
  LOG("Could not load global-level function named: %s", #name);   \
  return false;                                                   \
} 

#include "ListOfVulkanFunctions.inl" 
	return true;
}

bool SingularityEngine::Vulkan::LibraryLoader::loadInstanceLevelEntryPoints(VkInstance& instance)
{
	UNREFERENCED_PARAMETER(instance);
#define INSTANCE_LEVEL_VULKAN_FUNCTION(name)                                  \
	name = (PFN_##name)vkGetInstanceProcAddr(instance, #name);       \
	if (name == nullptr) {                                                    \
		LOG("Could not load instance-level Vulkan function named: %s", #name); \
		return false;                                                         \
}

#include "ListOfVUlkanFunctions.inl"
	return true;
}

bool SingularityEngine::Vulkan::LibraryLoader::shutdown()
{
#if defined _WIN32 
	FreeLibrary(mVulkanLibrary);
#elif defined __linux 
	dlclose(mVulkanLibrary);
#endif 
	return true;
}

bool SingularityEngine::Vulkan::LibraryLoader::loadLibrary()
{
#if defined _WIN32 
	mVulkanLibrary = LoadLibrary(L"vulkan-1.dll");
#elif defined __linux 
	mVulkanLibrary = dlopen("libvulkan.so.1", RTLD_NOW);
#endif 
	if (mVulkanLibrary == nullptr) {
		LOG("Could not connect with a Vulkan Runtime library.");
		return false;
	}
	return true;
}

bool SingularityEngine::Vulkan::LibraryLoader::loadInstanceFuncs(VkInstance& instance)
{
	if (!loadInstanceLevelEntryPoints(instance))
	{
		return false;
	}
	if (!loadExtensionLevelEntryPoints(instance))
	{
		return false;
	}
	return true;
}

bool SingularityEngine::Vulkan::LibraryLoader::loadDeviceFuncs(VkDevice& device)
{
	if (!loadDeviceLevelEntryPoints(device))
	{
		return false;
	}
	if (!loadDeviceLevelExtensionEntryPoints(device))
	{
		return false;
	}
	return true;
}
