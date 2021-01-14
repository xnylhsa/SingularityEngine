#ifndef INCLUDED_MATH_VECTOR_4_H
#define INCLUDED_MATH_VECTOR_4_H

#include "Common.h"
#include "SimpleFloatingPointMath.h"
#include "MathForwards.h"

namespace SingularityEngine::Math
{
	struct Vector4
	{
		float x, y, z, w;
		inline Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
		inline Vector4(float X, float Y, float Z, float W) : x(X), y(Y), z(Z), w(W) {}
		inline Vector4(const Vector3& v3D, float W);

		template<typename Stream>
		inline bool Serialize(Stream& stream);

		static Vector4 Zero() { return Vector4(0.0f, 0.0f, 0.0f, 0.0f); }
		static Vector4 One() { return Vector4(1.0f, 1.0f, 1.0f, 1.0f); }

		inline Vector4 operator +(const Vector4& right) const { return Vector4(x + right.x, y + right.y, z + right.z, w + right.w); }
		inline Vector4 operator -(const Vector4& right) const { return Vector4(x - right.x, y + right.y, z + right.z, w + right.w); }
		inline Vector4 operator *(int right) const { return Vector4(x * right, y * right, z * right, w); }
		inline Vector4 operator *(float right) const { return Vector4(x * right, y * right, z * right, w); }

		inline Vector4 operator /(int right) const { ASSERT(right != 0, "[MATH::Vector4] Error Divide By Zero"); return Vector4(x / right, y / right, z / right, w / right); }
		inline Vector4 operator /(float right) const { ASSERT(right != 0, "[MATH::Vector4] Error Divide By Zero"); return Vector4(x / right, y / right, z / right, w / right); }

		inline Vector4& operator +=(const Vector4& right) { x += right.x; y += right.y; z += right.z; w += right.w; return *this; }
		inline Vector4& operator -=(const Vector4& right) { x -= right.x; y -= right.y; z -= right.z; w -= right.w; return *this; }
		inline Vector4& operator *=(int right) { x *= right; y *= right; z *= right; w *= right; return *this; }
		inline Vector4& operator *=(float right) { x *= right; y *= right; z *= right; w *= right; return *this; }
		inline Vector4& operator /=(int right) { ASSERT(right != 0, "[MATH::Vector4] Error Divide By Zero"); x /= right; y /= right; z /= right; w /= right; return *this; }
		inline Vector4& operator /=(float right) { ASSERT(right != 0, "[MATH::Vector4] Error Divide By Zero"); x /= right; y /= right; z /= right; w /= right; return *this; }
	};
	template<typename Stream>
	inline bool Vector4::Serialize(Stream& stream)
	{
		if (!Core::Serialize_Float(stream, x))
		{
			return false;
		}
		if (!Core::Serialize_Float(stream, y))
		{
			return false;
		}
		if (!Core::Serialize_Float(stream, z))
		{
			return false;
		}
		if (!Core::Serialize_Float(stream, w))
		{
			return false;
		}
		return true;
	}
}
#endif