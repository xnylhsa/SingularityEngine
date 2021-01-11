#ifndef MATH_META_REGISTRATION_
#define MATH_META_REGISTRATION_
#include "Common.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"
#include "Color.h"

META_TYPE_DECLARE(SingularityEngine::Math::Vector2);
META_TYPE_DECLARE(SingularityEngine::Math::Vector3);
META_TYPE_DECLARE(SingularityEngine::Math::Vector4);
META_TYPE_DECLARE(SingularityEngine::Math::Quaternion);
META_TYPE_DECLARE(SingularityEngine::Math::Color);

namespace SingularityEngine::Math
{
	void MetaRegistration();
}


#endif // 
