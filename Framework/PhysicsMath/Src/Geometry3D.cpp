#include "Precompiled.h"
#include "Geometry3D.h"

using namespace SingularityEngine::Math;
META_TYPE_CLASS_DEFINE(Line, "Line")
META_CLASS_BEGIN(Line)
META_FIELD_BEGIN
META_FIELD(Line::start, "start")
META_FIELD(Line::end, "end")
META_FIELD_END
META_CLASS_END

META_TYPE_CLASS_DEFINE(Ray, "Ray")
META_CLASS_BEGIN(Ray)
META_FIELD_BEGIN
META_FIELD(Ray::origin, "origin")
META_FIELD(Ray::direction, "direction")
META_FIELD_END
META_CLASS_END

META_TYPE_CLASS_DEFINE(Triangle, "Triangle")
META_CLASS_BEGIN(Triangle)
META_FIELD_BEGIN
META_FIELD(Triangle::a, "a")
META_FIELD(Triangle::b, "b")
META_FIELD(Triangle::c, "c")
META_FIELD_END
META_CLASS_END

META_TYPE_CLASS_DEFINE(Plane, "Plane")
META_CLASS_BEGIN(Plane)
META_FIELD_BEGIN
META_FIELD(Plane::normal, "normal")
META_FIELD(Plane::distance, "distance")
META_FIELD_END
META_CLASS_END

META_TYPE_CLASS_DEFINE(Quad, "Quad")
META_CLASS_BEGIN(Quad)
META_FIELD_BEGIN
META_FIELD(Quad::halfExtent, "halfExtent")
META_FIELD(Quad::position, "position")
META_FIELD_END
META_CLASS_END

META_TYPE_CLASS_DEFINE(AABB, "AABB")
META_CLASS_BEGIN(AABB)
META_FIELD_BEGIN
META_FIELD(AABB::halfExtent, "halfExtent")
META_FIELD(AABB::position, "position")
META_FIELD_END
META_CLASS_END

META_TYPE_CLASS_DEFINE(OBB, "OBB")
META_CLASS_BEGIN(OBB)
META_FIELD_BEGIN
META_FIELD(OBB::size , "size")
META_FIELD(OBB::orientation, "orientation")
META_FIELD(OBB::center, "center")
META_FIELD_END
META_CLASS_END


META_TYPE_CLASS_DEFINE(Sphere, "Sphere")
META_CLASS_BEGIN(Sphere)
META_FIELD_BEGIN
META_FIELD(Sphere::radius, "radius")
META_FIELD(Sphere::position, "position")
META_FIELD_END
META_CLASS_END