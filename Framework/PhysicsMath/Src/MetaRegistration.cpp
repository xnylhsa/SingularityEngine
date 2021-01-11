#include "Precompiled.h"
#include "MetaRegistration.h"

void SingularityEngine::PhysicsMath::MetaRegistration()
{
#define REGISTER(x)  Core::MetaRegistry::Register(x::StaticGetMetaClass());
	REGISTER(Math::Line2D)
	REGISTER(Math::Rectangle)
	REGISTER(Math::Circle)
	REGISTER(Math::Line)
	REGISTER(Math::Ray)
	REGISTER(Math::Plane)
	REGISTER(Math::Quad)
	REGISTER(Math::AABB)
	REGISTER(Math::OBB)
	REGISTER(Math::Sphere)

#undef REGISTER
}