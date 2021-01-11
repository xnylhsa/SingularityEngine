#ifndef INCLUDED_MATH_MATRIX_4X4_H
#define INCLUDED_MATH_MATRIX_4X4_H

#include "Common.h"
#include "SimpleFloatingPointMath.h"
#include "MathForwards.h"

namespace SingularityEngine::Math
{
	struct Matrix4x4
	{
		float a1, a2, a3, a4;
		float b1, b2, b3, b4;
		float c1, c2, c3, c4;
		float d1, d2, d3, d4;

		Matrix4x4()
			:a1(1.0f), a2(0.0f), a3(0.0f), a4(0.0f),
			b1(0.0f), b2(1.0f), b3(0.0f), b4(0.0f),
			c1(0.0f), c2(0.0f), c3(1.0f), c4(0.0f),
			d1(0.0f), d2(0.0f), d3(0.0f), d4(1.0f)
		{}


		Matrix4x4(
			float _11, float _12, float _13, float _14,
			float _21, float _22, float _23, float _24,
			float _31, float _32, float _33, float _34,
			float _41, float _42, float _43, float _44)
			:a1(_11), a2(_12), a3(_13), a4(_14),
			b1(_21), b2(_22), b3(_23), b4(_24),
			c1(_31), c2(_32), c3(_33), c4(_34),
			d1(_41), d2(_42), d3(_43), d4(_44)
		{}

		template<typename Stream>
		bool Serialize(Stream& stream);

		template<typename Stream>
		bool Serialize_Compressed(Stream& stream, float min, float max, float precision);

		static Matrix4x4 One() { return Matrix4x4(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f); }
		static Matrix4x4 Zero() { return Matrix4x4(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f); }
		static Matrix4x4 Identity() { return Matrix4x4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); }
		static Matrix4x4 Translation(float x, float y, float z) { return Matrix4x4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, x, y, z, 1.0f); }
		static Matrix4x4 Translation(const Vector3& translation);

		static Matrix4x4 RotationXAxis(float rad) { return Matrix4x4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, cosf(rad), sinf(rad), 0.0f, 0.0f, -sinf(rad), cosf(rad), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); }
		static Matrix4x4 RotationYAxis(float rad) { const float c = cosf(rad); const float s = sinf(rad); return Matrix4x4(c, 0.0f, -s, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, s, 0.0f, c, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); }
		static Matrix4x4 RotationZAxis(float rad) { return Matrix4x4(cosf(rad), sinf(rad), 0.0f, 0.0f, -sinf(rad), cosf(rad), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); }
		static Matrix4x4 RotationFromAxis(const Vector3& up, const Vector3& right, const Vector3 forward);
		static inline Matrix4x4 RotationQuaternion(Quaternion q);


		static Matrix4x4 Scale(float sx, float sy, float sz) { return Matrix4x4(sx, 0.0f, 0.0f, 0.0f, 0.0f, sy, 0.0f, 0.0f, 0.0f, 0.0f, sz, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); }
		static Matrix4x4 Scale(float s) { return Matrix4x4(s, 0.0f, 0.0f, 0.0f, 0.0f, s, 0.0f, 0.0f, 0.0f, 0.0f, s, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); }




		inline Matrix4x4 operator-();
		inline Matrix4x4 operator+(const Matrix4x4& right) const;
		inline Matrix4x4 operator-(const Matrix4x4& right) const;
		inline Matrix4x4 operator*(float scaler) const;
		inline Matrix4x4 operator*(const Matrix4x4& right) const;
		inline Matrix4x4 operator / (float scaler) const;
		inline Matrix4x4& operator+=(const Matrix4x4& right);

	};
	template<typename Stream>
	bool Matrix4x4::Serialize(Stream& stream)
	{
		if (!Core::Serialize_Float(stream, a1))
		{
			return false;
		}
		if (!Core::Serialize_Float(stream, a2))
		{
			return false;
		}
		if (!Core::Serialize_Float(stream, a3))
		{
			return false;
		}
		if (!Core::Serialize_Float(stream, a4))
		{
			return false;
		}

		if (!Core::Serialize_Float(stream, b1))
		{
			return false;
		}
		if (!Core::Serialize_Float(stream, b2))
		{
			return false;
		}
		if (!Core::Serialize_Float(stream, b3))
		{
			return false;
		}
		if (!Core::Serialize_Float(stream, b4))
		{
			return false;
		}

		if (!Core::Serialize_Float(stream, c1))
		{
			return false;
		}
		if (!Core::Serialize_Float(stream, c2))
		{
			return false;
		}
		if (!Core::Serialize_Float(stream, c3))
		{
			return false;
		}
		if (!Core::Serialize_Float(stream, c4))
		{
			return false;
		}

		if (!Core::Serialize_Float(stream, d1))
		{
			return false;
		}
		if (!Core::Serialize_Float(stream, d2))
		{
			return false;
		}
		if (!Core::Serialize_Float(stream, d3))
		{
			return false;
		}
		if (!Core::Serialize_Float(stream, d4))
		{
			return false;
		}

		return true;
	}

	template<typename Stream>
	bool Matrix4x4::Serialize_Compressed(Stream& stream, float min, float max, float precision)
	{
		if (!Core::Serialize_Compressed_Float(stream, a1, min, max, precision))
		{
			return false;
		}
		if (!Core::Serialize_Compressed_Float(stream, a2, min, max, precision))
		{
			return false;
		}
		if (!Core::Serialize_Compressed_Float(stream, a3, min, max, precision))
		{
			return false;
		}
		if (!Core::Serialize_Compressed_Float(stream, a4, min, max, precision))
		{
			return false;
		}

		if (!Core::Serialize_Compressed_Float(stream, b1, min, max, precision))
		{
			return false;
		}
		if (!Core::Serialize_Compressed_Float(stream, b2, min, max, precision))
		{
			return false;
		}
		if (!Core::Serialize_Compressed_Float(stream, b3, min, max, precision))
		{
			return false;
		}
		if (!Core::Serialize_Compressed_Float(stream, b4, min, max, precision))
		{
			return false;
		}

		if (!Core::Serialize_Compressed_Float(stream, c1, min, max, precision))
		{
			return false;
		}
		if (!Core::Serialize_Compressed_Float(stream, c2, min, max, precision))
		{
			return false;
		}
		if (!Core::Serialize_Compressed_Float(stream, c3, min, max, precision))
		{
			return false;
		}
		if (!Core::Serialize_Compressed_Float(stream, c4, min, max, precision))
		{
			return false;
		}

		if (!Core::Serialize_Compressed_Float(stream, d1, min, max, precision))
		{
			return false;
		}
		if (!Core::Serialize_Compressed_Float(stream, d2, min, max, precision))
		{
			return false;
		}
		if (!Core::Serialize_Compressed_Float(stream, d3, min, max, precision))
		{
			return false;
		}
		if (!Core::Serialize_Compressed_Float(stream, d4, min, max, precision))
		{
			return false;
		}
	}

	inline Matrix4x4 Matrix4x4::operator-()
	{
		return Matrix4x4(
			-(a1), -(a2), -(a3), -(a4),
			-(b1), -(b2), -(b3), -(b4),
			-(c1), -(c2), -(c3), -(c4),
			-(d1), -(d2), -(d3), -(d4));
	}
	inline Matrix4x4 Matrix4x4::operator+(const Matrix4x4& right) const
	{
		return Matrix4x4(
			a1 + right.a1, a2 + right.a2, a3 + right.a3, a4 + right.a4,
			b1 + right.b1, b2 + right.b2, b3 + right.b3, b4 + right.b4,
			c1 + right.c1, c2 + right.c2, c3 + right.c3, c4 + right.c4,
			d1 + right.d1, d2 + right.d2, d3 + right.d3, d4 + right.d4
		);
	}
	inline Matrix4x4 Matrix4x4::operator-(const Matrix4x4& right) const
	{
		return Matrix4x4(
			a1 - right.a1, a2 - right.a2, a3 - right.a3, a4 - right.a4,
			b1 - right.b1, b2 - right.b2, b3 - right.b3, b4 - right.b4,
			c1 - right.c1, c2 - right.c2, c3 - right.c3, c4 - right.c4,
			d1 - right.d1, d2 - right.d2, d3 - right.d3, d4 - right.d4
		);
	}
	inline Matrix4x4 Matrix4x4::operator*(float scaler) const
	{
		return Matrix4x4(
			a1 * scaler, a2 * scaler, a3 * scaler, a4 * scaler,
			b1 * scaler, b2 * scaler, b3 * scaler, b4 * scaler,
			c1 * scaler, c2 * scaler, c3 * scaler, c4 * scaler,
			d1 * scaler, d2 * scaler, d3 * scaler, d4 * scaler
		);
	}

	inline Matrix4x4 Matrix4x4::operator*(const Matrix4x4& right) const
	{
		return Matrix4x4(

			a1 * right.a1 + a2 * right.b1 + a3 * right.c1 + a4 * right.d1,
			a1 * right.a2 + a2 * right.b2 + a3 * right.c2 + a4 * right.d2,
			a1 * right.a3 + a2 * right.b3 + a3 * right.c3 + a4 * right.d3,
			a1 * right.a4 + a2 * right.b4 + a3 * right.c4 + a4 * right.d4,

			b1 * right.a1 + b2 * right.b1 + b3 * right.c1 + b4 * right.d1,
			b1 * right.a2 + b2 * right.b2 + b3 * right.c2 + b4 * right.d2,
			b1 * right.a3 + b2 * right.b3 + b3 * right.c3 + b4 * right.d3,
			b1 * right.a4 + b2 * right.b4 + b3 * right.c4 + b4 * right.d4,

			c1 * right.a1 + c2 * right.b1 + c3 * right.c1 + c4 * right.d1,
			c1 * right.a2 + c2 * right.b2 + c3 * right.c2 + c4 * right.d2,
			c1 * right.a3 + c2 * right.b3 + c3 * right.c3 + c4 * right.d3,
			c1 * right.a4 + c2 * right.b4 + c3 * right.c4 + c4 * right.d4,

			d1 * right.a1 + d2 * right.b1 + d3 * right.c1 + d4 * right.d1,
			d1 * right.a2 + d2 * right.b2 + d3 * right.c2 + d4 * right.d2,
			d1 * right.a3 + d2 * right.b3 + d3 * right.c3 + d4 * right.d3,
			d1 * right.a4 + d2 * right.b4 + d3 * right.c4 + d4 * right.d4
		);
	}

	inline Matrix4x4 Matrix4x4::operator/(float scaler) const
	{
		ASSERT(scaler != 0, "[MATH::Matrix4x4] Error Divide By Zero");
		return Matrix4x4(
			a1 / scaler, a2 / scaler, a3 / scaler, a4 / scaler,
			b1 / scaler, b2 / scaler, b3 / scaler, b4 / scaler,
			c1 / scaler, c2 / scaler, c3 / scaler, c4 / scaler,
			d1 / scaler, d2 / scaler, d3 / scaler, d4 / scaler
		);
	}
	inline Matrix4x4& Matrix4x4::operator+=(const Matrix4x4& right)
	{
		a1 += right.a1; a2 += right.a2; a3 += right.a3; a4 += right.a4;
		b1 += right.b1; b2 += right.b2; b3 += right.b3; b4 += right.b4;
		c1 += right.c1; c2 += right.c2; c3 += right.c3; c4 += right.c4;
		d1 += right.d1; d2 += right.d2; d3 += right.d3; d4 += right.d4;
		return *this;
	}
}
#endif