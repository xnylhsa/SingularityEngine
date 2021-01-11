#ifndef INCLUDED_MATH_MATRIX_3X3_H
#define INCLUDED_MATH_MATRIX_3X3_H

#include "Common.h"
#include "SimpleFloatingPointMath.h"
#include "MathForwards.h"

namespace SingularityEngine::Math
{
	struct Matrix3x3
	{
		float _11, _12, _13;
		float _21, _22, _23;
		float _31, _32, _33;

		Matrix3x3()
			: _11(1.0f), _12(0.0f), _13(0.0f),
			_21(0.0f), _22(1.0f), _23(0.0f),
			_31(0.0f), _32(0.0f), _33(1.0f)
		{}


		Matrix3x3(
			float _11, float _12, float _13,
			float _21, float _22, float _23,
			float _31, float _32, float _33)
			:_11(_11), _12(_12), _13(_13),
			_21(_21), _22(_22), _23(_23),
			_31(_31), _32(_32), _33(_33)
		{}


		static Matrix3x3 Zero() { return Matrix3x3(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f); }
		static Matrix3x3 Identity() { return Matrix3x3(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f); };
		static Matrix3x3 Translation(float x, float y) { return Matrix3x3(1.0f, 0.0f, x, 0.0f, 1.0f, y, 0.0f, 0.0f, 1.0f); };
		static Matrix3x3 Rotation(float rad) {
			return Matrix3x3
			(
			cosf(rad), -sinf(rad), 0.0f,
			sinf(rad), cosf(rad), 0.0f,
			0.0f, 0.0f, 1.0f
			)
				;
		}
		static Matrix3x3 Shear(float a) { return Matrix3x3(1.0f, a, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f); }
		static Matrix3x3 Scale(float sx, float sy) { return Matrix3x3(sx, 0.0f, 0.0f, 0.0f, sy, 0.0f, 0.0f, 0.0f, 1.0f); }
		static Matrix3x3 Scale(float s) { return Matrix3x3(s, 0.0f, 0.0f, 0.0f, s, 0.0f, 0.0f, 0.0f, 1.0f); }

		inline Matrix3x3 operator-();

		inline Matrix3x3 operator+(const Matrix3x3& right) const;

		inline Matrix3x3 operator-(const Matrix3x3& right) const;

		inline Matrix3x3 operator*(float scaler) const;

		inline Matrix3x3 operator*(const Matrix3x3& right) const;

		inline Matrix3x3 operator/(float scaler) const;

		inline Matrix3x3& operator+=(const Matrix3x3& right);
	};

	inline Matrix3x3 Matrix3x3::operator-()
	{
		return Matrix3x3(
			-(_11), -(_12), -(_13),
			-(_21), -(_22), -(_23),
			-(_31), -(_32), -(_33));
	}
	inline Matrix3x3 Matrix3x3::operator+(const Matrix3x3& right) const
	{
		return Matrix3x3(
			_11 + right._11, _12 + right._12, _13 + right._13,
			_21 + right._21, _22 + right._22, _23 + right._23,
			_31 + right._31, _32 + right._32, _33 + right._33
		);
	}
	inline Matrix3x3 Matrix3x3::operator-(const Matrix3x3& right) const
	{
		return Matrix3x3(
			_11 - right._11, _12 - right._12, _13 - right._13,
			_21 - right._21, _22 - right._22, _23 - right._23,
			_31 - right._31, _32 - right._32, _33 - right._33
		);
	}
	inline Matrix3x3 Matrix3x3::operator*(float scaler) const
	{
		return Matrix3x3(
			_11 * scaler, _12 * scaler, _13 * scaler,
			_21 * scaler, _22 * scaler, _23 * scaler,
			_31 * scaler, _32 * scaler, _33 * scaler
		);
	}

	inline Matrix3x3 Matrix3x3::operator*(const Matrix3x3& right) const
	{
		return Matrix3x3(

			_11 * right._11 + _12 * right._21 + _13 * right._31,
			_11 * right._12 + _12 * right._22 + _13 * right._32,
			_11 * right._13 + _12 * right._23 + _13 * right._33,

			_21 * right._11 + _22 * right._21 + _23 * right._31,
			_22 * right._12 + _22 * right._22 + _23 * right._32,
			_23 * right._13 + _22 * right._23 + _23 * right._33,

			_31 * right._11 + _32 * right._21 + _33 * right._31,
			_32 * right._12 + _32 * right._22 + _33 * right._32,
			_33 * right._13 + _32 * right._23 + _33 * right._33
		);
	}

	inline Matrix3x3 Matrix3x3::operator/(float scaler) const
	{
		ASSERT(scaler != 0, "[MATH::Matrix3x3] Error Divide By Zero");
		return Matrix3x3(
			_11 / scaler, _12 / scaler, _13 / scaler,
			_21 / scaler, _22 / scaler, _23 / scaler,
			_31 / scaler, _32 / scaler, _33 / scaler
		);
	}

	inline Matrix3x3& Matrix3x3::operator+=(const Matrix3x3& right)
	{
		_11 += right._11; _12 += right._12; _13 += right._13;
		_21 += right._21; _22 += right._22; _23 += right._23;
		_31 += right._31; _32 += right._32; _33 += right._33;
		return *this;
	}
}

#endif
