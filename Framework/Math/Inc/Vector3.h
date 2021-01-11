#ifndef INCLUDED_MATH_VECTOR_3_H
#define INCLUDED_MATH_VECTOR_3_H
#include "Common.h"
#include "SimpleFloatingPointMath.h"
#include "MathForwards.h"
namespace SingularityEngine::Math
{
	struct Vector3
	{
		float x, y, z;
		inline Vector3() {}
		inline Vector3(float val) : x(val), y(val), z(val) {}

		inline Vector3(float X, float Y, float Z) : x(X), y(Y), z(Z) {}
		inline Vector3(const Vector2& v2D, float Z);

		const static Vector3 Zero() { return Vector3(0.0f, 0.0f, 0.0f); }
		const static Vector3 One() { return Vector3(1.0f, 1.0f, 1.0f); }
		const static Vector3 AxisX() { return Vector3(1.0f, 0.0f, 0.0f); }
		const static Vector3 AxisY() { return Vector3(0.0f, 1.0f, 0.0f); }
		const static Vector3 AxisZ() { return Vector3(0.0f, 0.0f, 1.0f); }
		static Vector3 Random(float min = 0.0f, float max = 1.0f) { Core::Random* rand = Core::Random::Get(); return Vector3(rand->UniformFloatDistibution(min, max), rand->UniformFloatDistibution(min, max), rand->UniformFloatDistibution(min, max)); }
		static Vector3 Random(Vector3 min, Vector3 max) { Core::Random* rand = Core::Random::Get(); return Vector3(rand->UniformFloatDistibution(min.x, max.x), rand->UniformFloatDistibution(min.y, max.y), rand->UniformFloatDistibution(min.z, max.z)); }

		inline Vector3 operator -() const { return Vector3(-x, -y, -z); }
		inline Vector3 operator +(const Vector3& right) const { return Vector3(x + right.x, y + right.y, z + right.z); }
		inline Vector3 operator -(const Vector3& right) const { return Vector3(x - right.x, y - right.y, z - right.z); }
		inline Vector3 operator *(int right) const { return Vector3(x * right, y * right, z * right); }
		inline Vector3 operator *(float right) const { return Vector3(x * right, y * right, z * right); }
		inline Vector3 operator *(int right) { return Vector3(x * right, y * right, z * right); }
		inline Vector3 operator *(float right) { return Vector3(x * right, y * right, z * right); }
		inline Vector3 operator *(const Vector3& right) const { return Vector3(x * right.x, y * right.y, z * right.z); }

		inline Vector3 operator /(int right) const { ASSERT(right != 0, "[MATH::vector3] Error Divide By Zero"); return Vector3(x / (float)right, y / (float)right, z / (float)right); }
		inline Vector3 operator /(float right) const { ASSERT(right != 0, "[MATH::vector3] Error Divide By Zero"); return Vector3(x / right, y / right, z / right); }

		inline Vector3& operator +=(const Vector3& right) { x += right.x; y += right.y; z += right.z; return *this; }
		inline Vector3& operator -=(const Vector3& right) { x -= right.x; y -= right.y; z -= right.z; return *this; }
		inline Vector3& operator *=(int right) { x *= right; y *= right; z *= right; return *this; }
		inline Vector3& operator *=(float right) { x *= right; y *= right; z *= right; return *this; }
		inline Vector3& operator /=(int right) { ASSERT(right != 0, "[MATH::vector3] Error Divide By Zero"); x /= right; y /= right; z /= right; return *this; }
		inline Vector3& operator /=(float right) { ASSERT(right != 0, "[MATH::vector3] Error Divide By Zero"); x /= right; y /= right; z /= right; return *this; }
		inline Vector3& operator *=(const Vector3& right) { x *= right.x; y *= right.y; z *= right.z; return *this; }



		inline void Truncate(float max) { x = Min(x, max); y = Min(y, max); z = Min(z, max); }
		inline const bool isZero() const { return (IsZero(x) && IsZero(y) && IsZero(z)) ? true : false; }
		inline const float Magnitude() const { return sqrt(MagnitudeSqr()); }
		inline const float MagnitudeSqr() const { return sqr(x) + sqr(y) + sqr(z); }

		inline void Normalize();

		inline Vector3 Cross(const Vector3& rhs) { return Vector3(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x); }
		inline float dot(const Vector3& rhs) { return (x * rhs.x) + (y * rhs.y) + (z * rhs.z); }
		template<typename Stream>
		inline bool Serialize(Stream& stream);
	};

	template<typename Stream>
	inline bool Vector3::Serialize(Stream& stream)
	{
		if (!(Core::Serialize_Float(stream, x)))
		{
			return false;
		}
		if (!(Core::Serialize_Float(stream, y)))
		{
			return false;
		}
		if (!(Core::Serialize_Float(stream, z)))
		{
			return false;
		}
		return true;
	}


	inline void Vector3::Normalize() 
	{ 
		float n = abs(Magnitude());
		float inverseN = 1 / n; 
		x *= inverseN; y *= inverseN; z *= inverseN;
	}

	inline Vector3 Maximum(const Vector3& left, const Vector3& right)
	{
		Vector3 max;

		max.x = Max<float>(left.x, right.x);
		max.y = Max<float>(left.y, right.y);
		max.z = Max<float>(left.z, right.z);

		return max;
	}

	inline Vector3 Minimum(const Vector3& left, const Vector3& right)
	{
		Vector3 min;
		min.x = Min<float>(left.x, right.x);
		min.y = Min<float>(left.y, right.y);
		min.z = Min<float>(left.z, right.z);

		return min;
	}

	inline bool IsZero(const Vector3& val) { return (IsZero(val.x) && IsZero(val.y) && IsZero(val.z)) ? true : false; }

	inline float MagnitudeSqr(const Vector3& val) { return sqr(val.x) + sqr(val.y) + sqr(val.z); }

	inline float Magnitude(const Vector3& val) { return sqrt(sqr(val.x) + sqr(val.y) + sqr(val.z)); }

	inline Vector3 Normalize(const Vector3& val) { float n = abs(val.Magnitude()); return Vector3(val.x / n, val.y / n, val.z / n); }

	inline Vector3 Normalize(const Vector3& val, const float len) { float n = (val.Magnitude()) / len; return Vector3(val.x / n, val.y / n, val.z / n); }

	inline float DistanceSqr(const Vector3& v1, const Vector3& v2) { return (sqr(v2.x - v1.x) + sqr(v2.y - v1.y) + sqr(v2.z - v1.z)); }

	inline float Distance(const Vector3& v1, const Vector3& v2) { return (sqrt(DistanceSqr(v1, v2))); }

	inline Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) { return Vector3(Lerp(v1.x, v2.x, t), Lerp(v1.y, v2.y, t), Lerp(v1.z, v2.z, t)); }

	inline float Dot(const Vector3& lhs, const Vector3& rhs) { return (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z); }

	inline Vector3 Cross(const Vector3& lhs, const Vector3& rhs) { return Vector3(lhs.y*rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x); }

	inline float EuclideanDistanceSquared(const Vector3& lhs, const Vector3& rhs)
	{
		return sqr(rhs.x - lhs.x) + sqr(rhs.y - lhs.y) + sqr(rhs.z - lhs.z);
	}

	inline float EuclideanDistance(const Vector3& lhs, const Vector3& rhs)
	{
		return sqrt(EuclideanDistanceSquared(lhs, rhs));
	}

	inline float ManhattenDistance(const Vector3& lhs, const Vector3& rhs)
	{
		return abs(rhs.x - lhs.x) + abs(rhs.y - lhs.y) + abs(rhs.z - lhs.z);
	}

	inline const bool AlmostEqual(const Vector3& v1, const Vector3& v2, float episilon = 1e-06)
	{
		return (compare(v1.x, v2.x, episilon) && compare(v1.y, v2.y, episilon), compare(v1.z, v2.z, episilon)) ? true : false;
	}

	inline Vector3 Reflect(const Vector3& dir, const Vector3& normal)
	{
		float dotProd = Dot(dir, normal);
		return dir - (normal * 2.0f * dotProd);
	}
}
#endif