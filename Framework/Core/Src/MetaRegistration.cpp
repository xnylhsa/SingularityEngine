#include "Precompiled.h"
#include "MetaRegistration.h"
#include "MetaRegistry.h"


META_TYPE_DEFINE(int, "Int")
META_TYPE_DEFINE(float, "Float")
META_TYPE_DEFINE(bool, "Bool")
META_TYPE_DEFINE(std::string, "String")

void SingularityEngine::Core::MetaRegistration()
{
#define REGISTER(x)  MetaRegistry::MetaRegistration(x::StaticGetMetaClass())


	//REGISTER(Class)

#undef REGISTER
}