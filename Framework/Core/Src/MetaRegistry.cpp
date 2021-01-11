#include "Precompiled.h"
#include "MetaRegistry.h"
#include "MetaClass.h"
#include "Debug.h"
namespace
{
	using MetaClassMap = std::map<std::string, const SingularityEngine::Core::MetaClass*>;
	inline MetaClassMap& GetRegistry()
	{
		static MetaClassMap sMetaClassMap;
		return sMetaClassMap;
	}
}
using namespace SingularityEngine::Core;
void MetaRegistry::Register(const MetaClass* metaClass)
{
	GetRegistry().emplace(metaClass->GetName(), metaClass);
}

const MetaClass* MetaRegistry::FindMetaClass(const char* className)
{
	MetaClassMap::const_iterator iter = GetRegistry().find(className);
	ASSERT(iter != GetRegistry().end(), "[MetaRegistry] Meta class for %s not found.", className);
	return iter->second;
}