#ifndef INCLUDED_MATH_VECTOR_2_H
#define INCLUDED_MATH_VECTOR_2_H
#include "Common.h"
#include "SimpleFloatingPointMath.h"

namespace SingularityEngine::Math
{
	struct Vector3;
	struct Vector2
	{
		float x{ 0.0f }, y{ 0.0f };
		inline Vector2() {}
		inline Vector2(float X, float Y) : x(X), y(Y) {}

		static Vector2 Zero() { return Vector2(0.0f, 0.0f); }
		static Vector2 One() { return Vector2(1.0f, 1.0f); }
		static Vector2 AxisX() { return Vector2(1.0, 0.0f); }
		static Vector2 AxisY() { return Vector2(0.0f, 1.0f); }
		static Vector2 Random(float min = 0.0f, float max = 1.0f)
		{
			Core::Random* rand = Core::Random::Get();
			return Vector2(rand->UniformFloatDistibution(min, max), rand->UniformFloatDistibution(min, max));
		}
		static Vector2 Random(const Math::Vector2& min, const Math::Vector2& max)
		{
			Core::Random* rand = Core::Random::Get();
			return Vector2(rand->UniformFloatDistibution(min.x, max.x), rand->UniformFloatDistibution(min.y, max.y));
		}
		inline const bool isZero() const { return (IsZero(x) && IsZero(y)) ? true : false; }
		inline const float Magnitude() const { return sqrt(MagnitudeSqr()); }
		inline const float MagnitudeSqr() const { return sqr(x) + sqr(y); }
		inline void Normalize()
		{
			float n = abs(Magnitude());
			float inverseN = 1 / n;
			x *= inverseN; y *= inverseN;
		}
		inline float dot(const Vector2& rhs) { return (x * rhs.x) + (y * rhs.y); }
		inline void Truncate(float max) { x = Min(x, max); y = Min(y, max); }


		inline Vector2 operator -() const { return Vector2(-x, -y); }
		inline Vector2 operator +(const Vector2& right) const { return Vector2(x + right.x, y + right.y); }
		inline Vector2 operator -(const Vector2& right) const { return Vector2(x - right.x, y - right.y); }
		inline Vector2 operator *(int right) const { return Vector2(x * right, y * right); }
		inline Vector2 operator *(float right) const { return Vector2(x * right, y * right); }
		inline Vector2 operator *(const Vector2& right) const { return Vector2(x * right.x, y * right.y); }

		inline Vector2 operator /(int right) const { ASSERT(right != 0, "[MATH::vector2] Error Divide By Zero"); return Vector2(x / right, y / right); }
		inline Vector2 operator /(float right) const { ASSERT(right != 0, "[MATH::vector2] Error Divide By Zero"); return Vector2(x / right, y / right); }


		inline Vector2& operator +=(const Vector2& right) { x += right.x; y += right.y; return *this; }
		inline Vector2& operator -=(const Vector2& right) { x -= right.x; y -= right.y; return *this; }
		inline Vector2& operator *=(int right) { x *= right; y *= right; return *this; }
		inline Vector2& operator *=(float right) { x *= right; y *= right; return *this; }
		inline Vector2& operator *=(const Vector2& right) { x *= right.x, y *= right.y; return *this; }

		inline Vector2& operator /=(int right) { ASSERT(right != 0, "[MATH::vector2] Error Divide By Zero"); x /= right; y /= right; return *this; }
		inline Vector2& operator /=(float right) { ASSERT(right != 0, "[MATH::vector2] Error Divide By Zero"); x /= right; y /= right; return *this; }

		inline Vector2& operator =(const Vector3& rhs);


		BEGIN_SERIALIZATION
			SERIALIZE_FLOAT(this->x)
			SERIALIZE_FLOAT(this->y)
		END_SERIALIZATION


	};
	inline bool operator==(const Vector2& left, const Vector2& right) { return left.x == right.x && left.y == right.y; }

	inline bool operator!=(const Vector2& left, const Vector2& right) { return left.x != right.x || left.y != right.y; }




	inline Vector2 Maximum(const Vector2& left, const Vector2& right)
	{
		Vector2 max;
		max.x = Max<float>(left.x, right.x);
		max.y = Max<float>(left.y, right.y);
		return max;
	}
	inline Vector2 Minimum(const Vector2& left, const Vector2& right)
	{
		Vector2 min;
		min.x = Min<float>(left.x, right.x);
		min.y = Min<float>(left.y, right.y);
		return min;
	}

	inline bool IsZero(const Vector2& val) { return (IsZero(val.x) && IsZero(val.y)) ? true : false; }
	inline float MagnitudeSqr(const Vector2& val) { return sqr(val.x) + sqr(val.y); }

	inline float Magnitude(const Vector2& val) { return sqrt(MagnitudeSqr(val)); }

	inline Vector2 Normalize(const Vector2& val) { float n = abs(val.Magnitude()); return Vector2(val.x / n, val.y / n); }

	inline float DistanceSqr(const Vector2& v1, const Vector2& v2) { return (sqr(v2.x - v1.x) + sqr(v2.y - v1.y)); }

	inline float Distance(const Vector2& v1, const Vector2& v2) { return (sqrt(DistanceSqr(v1, v2))); }

	inline Vector2 Lerp(const Vector2& v1, const Vector2& v2, float t)
	{
		return Vector2(Lerp(v1.x, v2.x, t), Lerp(v1.y, v2.y, t));
	}

	inline float Dot(const Vector2& lhs, const Vector2& rhs) { return (lhs.x * rhs.x) + (lhs.y * rhs.y); }

	inline float EuclideanDistanceSquared(const Vector2& lhs, const Vector2& rhs)
	{
		return sqr(rhs.x - lhs.x) + sqr(rhs.y - lhs.y);
	}

	inline float EuclideanDistance(const Vector2& lhs, const Vector2& rhs)
	{
		return sqrt(EuclideanDistanceSquared(lhs, rhs));
	}

	inline float ManhattenDistance(const Vector2& lhs, const Vector2& rhs)
	{
		return abs(rhs.x - lhs.x) + abs(rhs.y - lhs.y);
	}

	inline Vector2 Reflect(const Vector2& dir, const Vector2& normal)
	{
		float dotProd = Dot(dir, normal);
		return dir - (normal * 2.0f * dotProd);
	}

}

#endif