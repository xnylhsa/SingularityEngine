#include "Precompiled.h"
#include "VulkanLibraryLoader.h"
#include "VulkanFunctions.h"
bool SingularityEngine::Graphics::VulkanLibraryLoader::init()
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

bool SingularityEngine::Graphics::VulkanLibraryLoader::loadDeviceLevelEntryPoints(VkDevice device)
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

bool SingularityEngine::Graphics::VulkanLibraryLoader::loadDeviceLevelExtensionEntryPoints(VkDevice device, StartupParameters startupInfo)
{
	UNREFERENCED_PARAMETER(device);
	std::vector<const char*>& desiredExtensions = startupInfo.mDeviceExtensions;
	UNREFERENCED_PARAMETER(desiredExtensions);

#define DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION( name, extension )    \
for( auto & enabled_extension : desiredExtensions ) {				        \
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

bool SingularityEngine::Graphics::VulkanLibraryLoader::loadExportedEntryPoints()
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

bool SingularityEngine::Graphics::VulkanLibraryLoader::loadExtensionLevelEntryPoints(VkInstance instance, StartupParameters startupInfo)
{
	UNREFERENCED_PARAMETER(instance);
	std::vector<const char*>& desiredExtensions = startupInfo.mInstanceExtensions;
	UNREFERENCED_PARAMETER(desiredExtensions);


#define INSTANCE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION( name, extension )    \
for( auto & enabled_extension : desiredExtensions ) {				        \
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

bool SingularityEngine::Graphics::VulkanLibraryLoader::loadGlobalLevelEntryPoints()
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

bool SingularityEngine::Graphics::VulkanLibraryLoader::loadInstanceLevelEntryPoints(VkInstance instance)
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

bool SingularityEngine::Graphics::VulkanLibraryLoader::shutdown()
{
	LOG("[Graphics System] Releasing Vulkan DLL");

#if defined _WIN32 
	FreeLibrary(mVulkanLibrary);
#elif defined __linux 
	dlclose(mVulkanLibrary);
#endif 
	return true;
}

bool SingularityEngine::Graphics::VulkanLibraryLoader::loadLibrary()
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

bool SingularityEngine::Graphics::VulkanLibraryLoader::loadInstanceFuncs(VkInstance instance, StartupParameters startupInfo)
{
	if (!loadInstanceLevelEntryPoints(instance))
	{
		return false;
	}
	if (!loadExtensionLevelEntryPoints(instance, startupInfo))
	{
		return false;
	}
	return true;
}

bool SingularityEngine::Graphics::VulkanLibraryLoader::loadDeviceFuncs(VkDevice device, StartupParameters startupInfo)
{
	if (!loadDeviceLevelEntryPoints(device))
	{
		return false;
	}
	if (!loadDeviceLevelExtensionEntryPoints(device, startupInfo))
	{
		return false;
	}
	return true;
}
