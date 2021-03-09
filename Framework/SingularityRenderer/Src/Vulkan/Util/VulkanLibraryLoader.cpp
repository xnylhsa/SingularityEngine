#include "Precompiled.h"
#include "Vulkan/Util/VulkanLibraryLoader.h"
#include "Vulkan/Util/VulkanFunctions.h"
bool SingularityEngine::SERenderer::VulkanLibraryLoader::init()
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

bool SingularityEngine::SERenderer::VulkanLibraryLoader::loadDeviceLevelEntryPoints(VkDevice device)
{
	UNREFERENCED_PARAMETER(device);
#define DEVICE_LEVEL_VULKAN_FUNCTION(name)                                  \
	name = (PFN_##name)vkGetDeviceProcAddr(device, #name);       \
	if (name == nullptr) {                                                    \
		LOG("Could not load device-level Vulkan function named: %s", #name); \
		return false;                                                         \
}

#include "Vulkan/Util/ListOfVulkanFunctions.inl" 

	return true;
}

bool SingularityEngine::SERenderer::VulkanLibraryLoader::loadDeviceLevelExtensionEntryPoints(VkDevice device, const DesiredExtensions& extensions)
{
	UNREFERENCED_PARAMETER(device);
	UNREFERENCED_PARAMETER(extensions);

#define DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION( name, extension )    \
for( auto & enabled_extension : extensions ) {				        \
  if( std::string( enabled_extension ).compare(std::string( extension )) == 0 )        \
  {																	        \
	name = (PFN_##name)vkGetDeviceProcAddr(device, #name);              \
    if( name == nullptr ) {                                                 \
      LOG("Could not load instance-level Vulkan function named: %s", #name);\
      return false;                                                         \
    }                                                                       \
  }                                                                         \
} 

#include "Vulkan/Util/ListOfVulkanFunctions.inl" 

	return true;
}

bool SingularityEngine::SERenderer::VulkanLibraryLoader::loadExportedEntryPoints()
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

#include "Vulkan/Util/ListOfVulkanFunctions.inl"
	return true;
}

bool SingularityEngine::SERenderer::VulkanLibraryLoader::loadExtensionLevelEntryPoints(VkInstance instance, const DesiredExtensions& extensions)
{
	UNREFERENCED_PARAMETER(instance);
	UNREFERENCED_PARAMETER(extensions);


#define INSTANCE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION( name, extension )    \
for( auto & enabled_extension : extensions ) {				        \
  if( std::string( enabled_extension ).compare(std::string( extension )) == 0 )        \
  {																	        \
	name = (PFN_##name)vkGetInstanceProcAddr(instance, #name);              \
    if( name == nullptr ) {                                                 \
      LOG("Could not load instance-level Vulkan function named: %s", #name);\
      return false;                                                         \
    }                                                                       \
  }                                                                         \
} 

#include "Vulkan/Util/ListOfVulkanFunctions.inl" 

	return true;
}

bool SingularityEngine::SERenderer::VulkanLibraryLoader::loadGlobalLevelEntryPoints()
{
#define GLOBAL_LEVEL_VULKAN_FUNCTION( name )                      \
name = (PFN_##name)vkGetInstanceProcAddr( nullptr, #name );       \
if( name == nullptr ) {                                           \
  LOG("Could not load global-level function named: %s", #name);   \
  return false;                                                   \
} 

#include "Vulkan/Util/ListOfVulkanFunctions.inl" 
	return true;
}

bool SingularityEngine::SERenderer::VulkanLibraryLoader::loadInstanceLevelEntryPoints(VkInstance instance)
{
	UNREFERENCED_PARAMETER(instance);
#define INSTANCE_LEVEL_VULKAN_FUNCTION(name)                                  \
	name = (PFN_##name)vkGetInstanceProcAddr(instance, #name);       \
	if (name == nullptr) {                                                    \
		LOG("Could not load instance-level Vulkan function named: %s", #name); \
		return false;                                                         \
}

#include "Vulkan/Util/ListOfVUlkanFunctions.inl"
	return true;
}

bool SingularityEngine::SERenderer::VulkanLibraryLoader::shutdown()
{
	LOG("[Graphics System] Releasing Vulkan DLL");

#if defined _WIN32 
	FreeLibrary(mVulkanLibrary);
#elif defined __linux 
	dlclose(mVulkanLibrary);
#endif 
	return true;
}

bool SingularityEngine::SERenderer::VulkanLibraryLoader::loadLibrary()
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

bool SingularityEngine::SERenderer::VulkanLibraryLoader::loadInstanceFuncs(VkInstance instance, const DesiredExtensions& extensions)
{
	if (!loadInstanceLevelEntryPoints(instance))
	{
		return false;
	}
	if (!loadExtensionLevelEntryPoints(instance, extensions))
	{
		return false;
	}
	return true;
}

bool SingularityEngine::SERenderer::VulkanLibraryLoader::loadDeviceFuncs(VkDevice device, const DesiredExtensions& extensions)
{
	if (!loadDeviceLevelEntryPoints(device))
	{
		return false;
	}
	if (!loadDeviceLevelExtensionEntryPoints(device, extensions))
	{
		return false;
	}
	return true;
}
