#include "Precompiled.h"
#include "MetaRegistration.h"

META_TYPE_DEFINE(SingularityEngine::Math::Vector2, "Vector2");
META_TYPE_DEFINE(SingularityEngine::Math::Vector3, "Vector3");
META_TYPE_DEFINE(SingularityEngine::Math::Vector4, "Vector4");
META_TYPE_DEFINE(SingularityEngine::Math::Quaternion, "Quaternion");
META_TYPE_DEFINE(SingularityEngine::Math::Color, "Color");

void SingularityEngine::Math::MetaRegistration()
{
#define REGISTER(x)  MetaRegistry::Register(x::StaticGetMetaClass())


	//REGISTER(Class)

#undef REGISTER
}