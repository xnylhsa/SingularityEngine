#ifndef SINGULARITY_ENGINE_MATH_GEOMETRY_3D_HEADER_
#define SINGULARITY_ENGINE_MATH_GEOMETRY_3D_HEADER_
#include "Common.h"


namespace SingularityEngine::Math
{
	struct Line
	{
		META_CLASS_DECLARE
		Vector3 start;
		Vector3 end;
		Line() {}
		inline float LengthSq()
		{
			return DistanceSqr(start, end);
		}
		inline float Length()
		{
			return sqrt(LengthSq());
		}
		Line(const Vector3& p1, const Vector3& p2) : start(p1), end(p2) {}
	};


	inline float LengthSq(const Line& line)
	{
		return DistanceSqr(line.start, line.end);
	}
	inline float Length(const Line& line)
	{
		return sqrt(LengthSq(line));
	}

	struct Ray
	{
		META_CLASS_DECLARE
		Vector3 origin{ 0.0f,0.0f,0.0f };
		Vector3 direction{ 0.0f,0.0f, 1.0f };
		Ray() {}
		Ray(const Vector3& o, const Vector3& d) : origin(o), direction(d) {}
		Ray(const Line& line) : origin(line.start), direction(Normalize(line.end - line.start)) { }
		static inline Ray FromPoints(const Vector3& from, const Vector3& to)
		{
			return Ray(from, Normalize(to - from));
		}
		void NormalizeDirection()
		{
			Normalize(direction);
		}
	};

	struct Triangle
	{
		META_CLASS_DECLARE
		union
		{
			struct {
				Vector3 a;
				Vector3 b;
				Vector3 c;
			};
			Vector3 points[3] = { {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
		};
		inline Triangle() {}

		inline Vector3 CalcTriangleNormal() const
		{
			Vector3 ab = b - a;
			Vector3 ac = c - a;
			Vector3 normal = Cross(ab, ac);
			normal.Normalize();
			return normal;
		}
		inline Triangle(const Vector3& p1, const Vector3& p2, const Vector3& p3) : a(p1), b(p2), c(p3) { points[0] = a; points[1] = b; points[2] = c; }
	};

	struct Plane
	{
		META_CLASS_DECLARE
		Vector3 normal{ 1.0f,0.0f,0.0f };
		float distance{ 0.0f };
		inline Plane() {}
		inline Plane(const Vector3& n, float d) : normal(n), distance(d) {}
	};


	inline float PlaneEquation(const Vector3& pt, const Plane& plane)
	{
		return Dot(pt, plane.normal) - plane.distance;
	}

	struct Quad
	{
		META_CLASS_DECLARE
		Vector2 halfExtent{ 1.0f,1.0f };
		Vector3 position{ 0.0f,0.0f,0.0f };
		Quad() {}
		Quad(Vector3 pos, Vector2 s) : halfExtent(s), position(pos) {}
	};


	inline Vector3 GetMin(const Quad& rect)
	{
		Vector3 p1 = rect.position - Vector3(rect.halfExtent.x, rect.halfExtent.y, 0.0f);
		Vector3 p2 = rect.position + Vector3(rect.halfExtent.x, rect.halfExtent.y, 0.0f);
		return Vector3(Min<float>(p1.x, p2.x), Min<float>(p1.y, p2.y), rect.position.z);
	}

	inline Vector3 GetMax(const Quad& rect)
	{
		Vector3 p1 = rect.position - Vector3(rect.halfExtent.x, rect.halfExtent.y, 0.0f);
		Vector3 p2 = rect.position + Vector3(rect.halfExtent.x, rect.halfExtent.y, 0.0f);
		return Vector3(Max<float>(p1.x, p2.x), Max<float>(p1.y, p2.y), rect.position.z);
	}

	//AxisAlignedBoundBox
	struct AABB
	{
		META_CLASS_DECLARE
		Vector3 position{ 0.0f,0.0f,0.0f };
		Vector3 halfExtent{ 1.0f,1.0f,1.0f };
		inline AABB() {}

		inline AABB(const Vector3& pos, const Vector3& s) : position(pos), halfExtent(s) {}

		static inline AABB FromMinMax(const Vector3& min, const Vector3& max)
		{
			return AABB(min + max * 0.5f, (max - min) * 0.5f);
		}

		inline Vector3 GetMin() const
		{
			Vector3 p1 = position + halfExtent;
			Vector3 p2 = position - halfExtent;
			return Minimum(p1, p2);
		}

		inline Vector3 GetMax() const
		{
			Vector3 p1 = position + halfExtent;
			Vector3 p2 = position - halfExtent;
			return Maximum(p1, p2);
		}
	};


	inline Vector3 GetMin(const AABB& aabb)
	{
		Vector3 p1 = aabb.position + aabb.halfExtent;
		Vector3 p2 = aabb.position - aabb.halfExtent;
		return Minimum(p1, p2);
	}

	inline Vector3 GetMax(const AABB& aabb)
	{
		Vector3 p1 = aabb.position + aabb.halfExtent;
		Vector3 p2 = aabb.position - aabb.halfExtent;
		return Maximum(p1, p2);
	}

	struct OBB
	{
		META_CLASS_DECLARE
		Vector3 center{ 0.0f, 0.0f, 0.0f };
		Vector3 size{ 1.0f, 1.0f, 1.0f };
		Quaternion orientation{ 0.0f, 0.0f, 0.0f,1.0f };
		inline OBB() { }
		inline OBB(const Vector3& p, const Vector3& s) : center(p), size(s) { }
		inline OBB(const Vector3& p, const Vector3& s, const Quaternion& q) : center(p), size(s), orientation(q) { }
		inline OBB(const Vector3& p, const Vector3& s, const Vector3& eulerOrientation);
	};


	struct Sphere
	{
		META_CLASS_DECLARE
		Vector3 position{ 0.0f, 0.0f, 0.0f };
		float radius{ 1.0f };
		inline Sphere() {}
		inline Sphere(const Vector3& pos, float r) : position(pos), radius(r) { }
	};




}
META_TYPE_DECLARE(SingularityEngine::Math::Line)
META_TYPE_DECLARE(SingularityEngine::Math::Ray)
META_TYPE_DECLARE(SingularityEngine::Math::Triangle)
META_TYPE_DECLARE(SingularityEngine::Math::Quad)
META_TYPE_DECLARE(SingularityEngine::Math::Plane)
META_TYPE_DECLARE(SingularityEngine::Math::AABB)
META_TYPE_DECLARE(SingularityEngine::Math::OBB)
META_TYPE_DECLARE(SingularityEngine::Math::Sphere)


#endif //SINGULARITY_ENGINE_MATH_GEOMETRY_3D_HEADER_