#ifndef SINGULARITY_ENGINE_MATH_GEOMETRY_2D_HEADER_
#define SINGULARITY_ENGINE_MATH_GEOMETRY_2D_HEADER_

#include "Common.h"

namespace SingularityEngine::Math
{


	struct Line2D
	{
		META_CLASS_DECLARE
		Vector2 start;
		Vector2 end;
		inline Line2D() {}
		inline Line2D(const Vector2& p1, const Vector2& p2) : start(p1), end(p2) {}
	};

	inline float LengthSq(const Line2D& line)
	{
		return DistanceSqr(line.start, line.end);
	}

	inline float Length(const Line2D& line)
	{
		return sqrt(LengthSq(line));
	}





	struct Rectangle
	{
		META_CLASS_DECLARE

		Vector2 size;
		Vector2 position;
		Rectangle() : size(0.0f, 0.0f), position(0.0f, 0.0f) {}

		Rectangle(const Vector2& pos, const Vector2& s) : size(s), position(pos) {}

		inline Vector2 GetMin() const
		{
			Vector2 p1 = position - size;
			Vector2 p2 = position + size;
			return Vector2(Min<float>(p1.x, p2.x), Min<float>(p1.y, p2.y));
		}

		inline Vector2 GetMax() const
		{
			Vector2 p1 = position - size;
			Vector2 p2 = position + size;
			return Vector2(Max<float>(p1.x, p2.x), Max<float>(p1.y, p2.y));
		}

		static inline Rectangle FromMinMax(const Vector2& min, const Vector2& max)
		{
			return Rectangle(min, max - min);
		}
	};


	inline Vector2 GetMin(const Rectangle& rect)
	{
		Vector2 p1 = rect.position - rect.size;
		Vector2 p2 = rect.position + rect.size;
		return Vector2(Min<float>(p1.x, p2.x), Min<float>(p1.y, p2.y));
	}

	inline Vector2 GetMax(const Rectangle& rect)
	{
		Vector2 p1 = rect.position - rect.size;
		Vector2 p2 = rect.position + rect.size;
		return Vector2(Max<float>(p1.x, p2.x), Max<float>(p1.y, p2.y));
	}

	inline Rectangle FromMinMax(const Vector2& min, const Vector2& max)
	{
		return Rectangle(min, max - min);
	}


	struct OrientedRectangle
	{
		Vector2 position{ 0.0f, 0.0f };
		Vector2 halfExtent{ 1.0f,1.0f };
		float rotationInDegrees{ 0.0f };
		OrientedRectangle() : halfExtent(1.0f, 1.0f), rotationInDegrees(0.0f) {}
		OrientedRectangle(const Vector2& pos, const Vector2& hExtents, float rotDeg) : position(pos), halfExtent(hExtents), rotationInDegrees(rotDeg) {}
		OrientedRectangle(const Vector2& pos, const Vector2& hExtents) : position(pos), halfExtent(hExtents), rotationInDegrees(0.0f) {}
	};

	struct Circle
	{
		META_CLASS_DECLARE
		float radius;
		Vector2 position;
		inline Circle() : radius(1.0f), position(0.0f, 0.0f) {  }
		inline Circle(float r, Vector2 pos) : radius(r), position(pos) {}
		inline Circle(Vector2 pos, float r) : radius(r), position(pos) {}

	};



}

META_TYPE_DECLARE(SingularityEngine::Math::Line2D)
META_TYPE_DECLARE(SingularityEngine::Math::Rectangle)
META_TYPE_DECLARE(SingularityEngine::Math::Circle)
#endif // SINGULARITY_ENGINE_MATH_GEOMETRY_2D_HEADER_
