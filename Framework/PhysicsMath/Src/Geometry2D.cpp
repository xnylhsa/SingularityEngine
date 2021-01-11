#include "Precompiled.h"
#include "Geometry2D.h"

using namespace SingularityEngine::Math;
META_TYPE_CLASS_DEFINE(Line2D,"Line2D")
META_CLASS_BEGIN(Line2D)
META_FIELD_BEGIN
META_FIELD(Line2D::start, "start")
META_FIELD(Line2D::end, "end")
META_FIELD_END
META_CLASS_END

META_TYPE_CLASS_DEFINE(SingularityEngine::Math::Rectangle, "Rectangle")
META_CLASS_BEGIN(SingularityEngine::Math::Rectangle)
META_FIELD_BEGIN
META_FIELD(Rectangle::position, "position")
META_FIELD(Rectangle::size, "size")
META_FIELD_END
META_CLASS_END

META_TYPE_CLASS_DEFINE(Circle, "Circle")
META_CLASS_BEGIN(Circle)
META_FIELD_BEGIN
META_FIELD(Circle::position, "position")
META_FIELD(Circle::radius, "radius")
META_FIELD_END
META_CLASS_END