#ifndef INCLUDED_VECTOR_MATRIX_MATH_H
#define INCLUDED_VECTOR_MATRIX_MATH_H

#include "Common.h"
#include "SimpleFloatingPointMath.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Color.h"
#include "Quaternion.h"
#include "Matrix3x3.h"
#include "Matrix4x4.h"


namespace SingularityEngine {

	namespace Math
	{

		inline Vector2& Vector2::operator=(const Vector3& rhs) { x = rhs.x; y = rhs.y; return *this; }


		inline Vector3::Vector3(const Vector2& v2D, float Z) :x(v2D.x), y(v2D.y), z(Z) {}

		inline Vector4::Vector4(const Vector3& v3D, float W) : x(v3D.x), y(v3D.y), z(v3D.z), w(W) {}

		inline Color::Color(const Vector3& v3D, float A) : r(v3D.x), g(v3D.y), b(v3D.z), a(A) {}

		inline Quaternion::Quaternion(const Vector3& axisAngle, float rotationAngle /*degrees*/)
		{
			float theta = rotationAngle * Math::kDegtoRad;
			theta = theta / 2.0f;
			w = std::cosf(theta);
			float rotationScale = std::sinf(theta);
			x = axisAngle.x * rotationScale;
			y = axisAngle.y * rotationScale;
			z = axisAngle.z * rotationScale;
			*this = Math::Normalize(*this);
		}

		inline Matrix4x4 Matrix4x4::Translation(const Vector3& translation) { return Matrix4x4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, translation.x, translation.y, translation.z, 1.0f); };

		inline Matrix4x4 Matrix4x4::RotationFromAxis(const Vector3& up, const Vector3& right, const Vector3 forward)
		{
			return Matrix4x4(
				up.x, right.x, forward.x, 0.0f,
				up.y, right.y, forward.y, 0.0f,
				up.z, right.z, forward.z, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			);
		}

		inline Quaternion RotationMatrixToQuaternion(const Matrix4x4& matrix)
		{
			Quaternion output;
			output.w = (sqrt(1.0f + matrix.a1 + matrix.b2 + matrix.c3) / 2.0f);
			float w4 = 1.0f / (4.0f * output.w);
			output.x = (matrix.b3 - matrix.c2) * w4;
			output.y = (matrix.c1 - matrix.a3) * w4;
			output.z = (matrix.a2 - matrix.b1) * w4;
			return output;
		}

		inline Matrix4x4 Matrix4x4::RotationQuaternion(Quaternion q)
		{
			q.Normalize();
			//avoid redundancies
			float xSq = sqr(q.x);
			float ySq = sqr(q.y);
			float zSq = sqr(q.z);
			float twoX = 2.0f * q.x;
			float twoY = 2.0f * q.y;
			float twoW = 2.0f * q.w;
			float xy = twoX * q.y;
			float xz = twoX * q.z;
			float yz = twoY * q.z;
			float wx = twoW * q.x;
			float wy = twoW * q.y;
			float wz = twoW * q.z;
			return Matrix4x4(
				1 - 2 * ySq - 2 * zSq, xy + wz, xz - wy, 0.0f,
				xy - wz, 1 - 2 * xSq - 2 * zSq, yz + wx, 0.0f,
				xz + wy, yz - wx, 1 - 2 * xSq - 2 * ySq, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f);

			//1 - 2 * ySq - 2 * zSq, xy - wz, xz + wy, 0.0f,
			//xy + wz, 1 - 2 * xSq - 2 * zSq, yz - wx, 0.0f,
			//xz - wy, yz + wx, 1 - 2 * xSq - 2 * ySq, 0.0f,
			//0.0f, 0.0f, 0.0f, 1.0f);
		}

		inline Quaternion EulerAnglesToQuaternion(float yaw, float roll, float pitch)
		{
			yaw *= kDegtoRad;
			pitch *= kDegtoRad;
			roll *= kDegtoRad;
			// Assuming the angles are in radians.
			float cYaw = Math::Cos(yaw * 0.5f);
			float sYaw = Math::Sin(yaw * 0.5f);
			float cRoll = Math::Cos(roll * 0.5f);
			float sRoll = Math::Sin(roll * 0.5f);
			float cPitch = Math::Cos(pitch * 0.5f);
			float sPitch = Math::Sin(pitch * 0.5f);
			Quaternion output;
			output.w = cYaw * cRoll * cPitch - sYaw * sRoll * sPitch;
			output.x = cYaw * cRoll * sPitch + sYaw * sRoll * cPitch;
			output.y = sYaw * cRoll * cPitch + cYaw * sRoll * sPitch;
			output.z = cYaw * sRoll * cPitch - sYaw * cRoll * sPitch;
			return output;
		}

		inline Quaternion EulerAnglesToQuaternion(const Vector3& eulerAngles)
		{
			return EulerAnglesToQuaternion(eulerAngles.y, eulerAngles.z, eulerAngles.x);
		}

		inline float GetRoll(const Quaternion& q) //X Axis
		{
			float sinr = 2.0f * (q.w * q.x + q.y * q.z);
			float cosr = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
			return atan2(sinr, cosr);
		}
		inline float GetYaw(const Quaternion& q) // Y Axis
		{
			float sinp = 2.0f * (q.w * q.y - q.z * q.x);
			if (fabs(sinp) >= 1)
				return copysign(kPi / 2.0f, sinp); // use 90 degrees if out of range
			else
				return asin(sinp);
		}
		inline float GetPitch(const Quaternion& q) // Z Axis
		{
			float siny = 2.0f * (q.w * q.z + q.x * q.y);
			float cosy = 1.0f - 2.0f * (q.y * q.y + q.z * q.w);
			return atan2(siny, cosy);
		}



		inline Math::Vector3 QuaternionToEulerAngles(const Quaternion& q)
		{
			return Vector3(GetRoll(q), GetYaw(q), GetPitch(q));
		}

		inline Quaternion QuaternionFromLookDirection(const Vector3& lookDir, const Vector3 upDir = Vector3::AxisY())
		{
			Vector3 sideVector = Math::Normalize(Math::Cross(upDir, lookDir));
			Vector3 upVec = Math::Normalize(Math::Cross(lookDir, sideVector));
			float m00 = sideVector.x;
			float m01 = sideVector.y;
			float m02 = sideVector.z;
			float m10 = upVec.x;
			float m11 = upVec.y;
			float m12 = upVec.z;
			float m20 = lookDir.x;
			float m21 = lookDir.y;
			float m22 = lookDir.z;

			float num8 = (m00 + m11) + m22;
			Quaternion quaternion;
			if (num8 > 0.0f)
			{
				float num = Math::sqrt(num8 + 1.0f);
				quaternion.w = num * 0.5f;
				num = 0.5f / num;
				quaternion.x = (m12 - m21) * num;
				quaternion.y = (m20 - m02) * num;
				quaternion.z = (m01 - m10) * num;
				return quaternion;
			}
			if ((m00 >= m11) && (m00 >= m22))
			{
				float num7 = Math::sqrt(((1.0f + m00) - m11) - m22);
				float num4 = 0.5f / num7;
				quaternion.x = 0.5f * num7;
				quaternion.y = (m01 + m10) * num4;
				quaternion.z = (m02 + m20) * num4;
				quaternion.w = (m12 - m21) * num4;
				return quaternion;
			}
			if (m11 > m22)
			{
				float num6 = Math::sqrt(((1.0f + m11) - m00) - m22);
				float num3 = 0.5f / num6;
				quaternion.x = (m10 + m01) * num3;
				quaternion.y = 0.5f * num6;
				quaternion.z = (m21 + m12) * num3;
				quaternion.w = (m20 - m02) * num3;
				return quaternion;
			}
			float num5 = Math::sqrt(((1.0f + m22) - m00) - m11);
			float num2 = 0.5f / num5;
			quaternion.x = (m20 + m02) * num2;
			quaternion.y = (m21 + m12) * num2;
			quaternion.z = 0.5f * num5;
			quaternion.w = (m01 - m10) * num2;
			return quaternion;
		}

		inline Quaternion ShortestQuaternionRotation(const Vector3& from, const Vector3& to)
		{
			Quaternion q;
			Vector3 crossProd = Cross(from, to);
			q.x = crossProd.x;
			q.y = crossProd.y;
			q.z = crossProd.z;
			q.w = sqrt(sqr(from.Magnitude()) * sqr(to.Magnitude())) + Dot(from, to);
			q.Normalize();
			return q;
		}

		inline Vector3 Cross(const Vector3& lhs, const Matrix3x3& rhs) { return Vector3((lhs.x * rhs._11) + (lhs.y*rhs._21) + lhs.z * (rhs._31), (lhs.x * rhs._12) + (lhs.y * rhs._22) + (rhs._32), (lhs.x * rhs._13) + (lhs.y * rhs._23) + lhs.z * (rhs._33)); }

		inline Vector4 Cross(const Vector4& lhs, const Matrix4x4& rhs)
		{
			return Vector4(
				(lhs.x * rhs.a1) + (lhs.y * rhs.b1) + (lhs.z * rhs.c1) + lhs.w * rhs.d1,
				(lhs.x * rhs.a2) + (lhs.y * rhs.b2) + (lhs.z * rhs.c2) + lhs.w * (rhs.d2),
				(lhs.x * rhs.a3) + (lhs.y * rhs.b3) + (lhs.z * rhs.c3) + lhs.w * (rhs.d3),
				(lhs.x * rhs.a4) + (lhs.y * rhs.b4) + (lhs.z * rhs.c4) + lhs.w * (rhs.d4));
		}

		inline Vector3 ApplyTransformation(const Vector3& point, const Matrix4x4& transformation)
		{
			Vector4 temp = Cross(Vector4(point.x, point.y, point.z, 1.0f), transformation);
			return (Vector3(temp.x, temp.y, temp.z) / temp.w);
		}

		inline Vector4 ApplyTransformation(const Vector4& point, const Matrix4x4& transformation)
		{
			Vector4 temp = Cross(Vector4(point.x, point.y, point.z, 1.0f), transformation);
			Vector4 returnVec;
			returnVec = temp / temp.w;

			return returnVec;
		}

		inline Vector2 ApplyTransformation(const Vector2& point, const Matrix3x3& transformation)
		{
			Vector3 temp = Cross(Vector3(point.x, point.y, 1.0f), transformation);
			return (Vector2(temp.x, temp.y) / temp.z);
		}

		inline Vector3 ApplyTransformationNormal(const Vector3& point, const Matrix4x4& transformation)
		{
			return Vector3
			(
				point.x * transformation.a1 + point.y * transformation.b1 + point.z * transformation.c1,
				point.x * transformation.a2 + point.y * transformation.b2 + point.z * transformation.c2,
				point.x * transformation.a3 + point.y * transformation.b3 + point.z * transformation.c3
			);
		}

		inline Vector2 ApplyTransformationNormal(const Vector2& point, const Matrix3x3& transformation)
		{
			Vector2 nPoint = Normalize(point);
			Vector3 temp = Cross(Vector3(nPoint.x, nPoint.y, 0.0f), transformation);
			return Vector2(temp.x, temp.y);
		}

		inline Vector3 GetTransform(const Matrix4x4& m) { return Vector3(m.d1, m.d2, m.d3); }

		inline Vector3 GetUpVector(const Matrix4x4& m) { return Vector3(m.a1, m.b1, m.c1); }

		inline Vector3 GetRightVector(const Matrix4x4& m) { return Vector3(m.a2, m.b2, m.c2); }

		inline Vector3 GetForwardVector(const Matrix4x4& m) { return Vector3(m.a3, m.b3, m.c3); }

		inline Vector2 Rotate(const Vector2& center, float rad) { return ApplyTransformation(center, Matrix3x3::Rotation(rad)); }

		inline float Determinant(const Matrix4x4& m)
		{
			float det = 0.0f;
			det = (m.a1 * (m.b2 * (m.c3 * m.d4 - (m.d3 * m.c4)) - m.b3 * (m.c2 * m.d4 - (m.d2 * m.c4)) + m.b4 * (m.c2 * m.d3 - (m.d2 * m.c3))));
			det -= (m.a2 * (m.b1 * (m.c3 * m.d4 - (m.d3 * m.c4)) - m.b3 * (m.c1 * m.d4 - (m.d1 * m.c4)) + m.b4 * (m.c1 * m.d3 - (m.d1 * m.c3))));
			det += (m.a3 * (m.b1 * (m.c2 * m.d4 - (m.d2 * m.c4)) - m.b2 * (m.c1 * m.d4 - (m.d1 * m.c4)) + m.b4 * (m.c1 * m.d2 - (m.d1 * m.c2))));
			det -= (m.a4 * (m.b1 * (m.c2 * m.d3 - (m.d2 * m.c3)) - m.b2 * (m.c1 * m.d3 - (m.d1 * m.c3)) + m.b3 * (m.c1 * m.d2 - (m.d1 * m.c2))));
			return det;
		}

		inline Matrix4x4 Transpose(const Matrix4x4& m)
		{
			return Matrix4x4
			(
				m.a1, m.b1, m.c1, m.d1,
				m.a2, m.b2, m.c2, m.d2,
				m.a3, m.b3, m.c3, m.d3,
				m.a4, m.b4, m.c4, m.d4
			);
		}

		static Matrix4x4 RotationAxis(const Vector3& axis, float rad)
		{
			Vector3 u = Normalize(axis);
			const float x = u.x;
			const float y = u.y;
			const float z = u.z;
			const float s = sin(rad);
			const float c = cos(rad);
			return Matrix4x4
			{
				c + (x * x * (1.0f - c)),
				x * y * (1.0f - c) + z * s,
				x * z * (1.0f - c) - y * s,
				0.0f,

				x * y * (1.0f - c) - z * s,
				c + (y * y * (1.0f - c)),
				y * z * (1.0f - c) + x * s,
				0.0f,

				x * z * (1.0f - c) + y * s,
				y * z * (1.0f - c) - x * s,
				c + (z * z * (1.0f - c)),
				0.0f,

				0.0f,
				0.0f,
				0.0f,
				1.0f
			};
		}

		inline Vector3 Rotate(const Quaternion& q, const Vector3& point)
		{
			float twoX = q.x * 2.0f;
			float twoY = q.y * 2.0f;
			float twoZ = q.z * 2.0f;
			float twoxSqr = q.x * twoX;
			float twoySqr = q.y * twoY;
			float twoZSqr = q.z * twoZ;
			float xy = q.x * twoY;
			float xz = q.x * twoZ;
			float yz = q.y * twoZ;
			float wx = q.w * twoX;
			float wy = q.w * twoY;
			float num12 = q.w * twoZ;
			Vector3 result;
			result.x = (1.0f - (twoySqr + twoZSqr)) * point.x + (xy - num12) * point.y + (xz + wy) * point.z;
			result.y = (xy + num12) * point.x + (1.0f - (twoxSqr + twoZSqr)) * point.y + (yz - wx) * point.z;
			result.z = (xz - wy) * point.x + (yz + wx) * point.y + (1.0f - (twoxSqr + twoySqr)) * point.z;
			return result;
		}

		inline Matrix4x4 Adjoint(const Matrix4x4& m)
		{
			return Matrix4x4
			(
				(m.b2 * ((m.c3 * m.d4) - (m.d3 * m.c4)) - m.b3 * ((m.c2 * m.d4) - (m.d2 * m.c4)) + m.b4 * ((m.c2 * m.d3) - (m.d2 * m.c3))),
				-(m.a2 * ((m.c3 * m.d4) - (m.d3 * m.c4)) - m.a3 * ((m.c2 * m.d4) - (m.d2 * m.c4)) + m.a4 * ((m.c2 * m.d3) - (m.d2 * m.c3))),
				(m.a2 * ((m.b3 * m.d4) - (m.d3 * m.b4)) - m.a3 * ((m.b2 * m.d4) - (m.d2 * m.b4)) + m.a4 * ((m.b2 * m.d3) - (m.d2 * m.b3))),
				-(m.a2 * ((m.b3 * m.c4) - (m.c3 * m.b4)) - m.a3 * ((m.b2 * m.c4) - (m.c2 * m.b4)) + m.a4 * ((m.b2 * m.c3) - (m.c2 * m.b3))),

				-(m.b1 * ((m.c3 * m.d4) - (m.d3 * m.c4)) - m.c1 * ((m.b3 * m.d4) - (m.b4 * m.d3)) + m.d1 * ((m.b3 * m.c4) - (m.b4 * m.c3))),
				(m.a1 * ((m.c3 * m.d4) - (m.d3 * m.c4)) - m.a3 * ((m.c1 * m.d4) - (m.d1 * m.c4)) + m.a4 * ((m.c1 * m.d3) - (m.d1 * m.c3))),
				-(m.a1 * ((m.b3 * m.d4) - (m.d3 * m.b4)) - m.a3 * ((m.b1 * m.d4) - (m.d1 * m.b4)) + m.a4 * ((m.b1 * m.d3) - (m.d1 * m.b3))),
				(m.a1 * ((m.b3 * m.c4) - (m.c3 * m.b4)) - m.a3 * ((m.b1 * m.c4) - (m.c1 * m.b4)) + m.a4 * ((m.b1 * m.c3) - (m.c1 * m.b3))),

				(m.b1 * ((m.c2 * m.d4) - (m.d2 * m.c4)) - m.c1 * ((m.b2 * m.d4) - (m.d2 * m.b4)) + m.d1 * ((m.b2 * m.c4) - (m.c2 * m.b4))),
				-(m.a1 * ((m.c2 * m.d4) - (m.d2 * m.c4)) - m.c1 * ((m.a2 * m.d4) - (m.d2 * m.a4)) + m.d1 * ((m.a2 * m.c4) - (m.c2 * m.a4))),
				(m.a1 * ((m.b2 * m.d4) - (m.d2 * m.b4)) - m.a2 * ((m.b1 * m.d4) - (m.d1 * m.b4)) + m.a4 * ((m.b1 * m.d2) - (m.d1 * m.b2))),
				-(m.a1 * ((m.b2 * m.c4) - (m.c2 * m.b4)) - m.b1 * ((m.a2 * m.c4) - (m.c2 * m.a4)) + m.c1 * ((m.a2 * m.b4) - (m.b2 * m.a4))),

				-(m.b1 * ((m.c2 * m.d3) - (m.d2 * m.c3)) - m.c1 * ((m.b2 * m.d3) - (m.d2 * m.b3)) + m.d1 * ((m.b2 * m.c3) - (m.c2 * m.b3))),
				(m.a1 * ((m.c2 * m.d3) - (m.d2 * m.c3)) - m.a2 * ((m.c1 * m.d3) - (m.d1 * m.c3)) + m.a3 * ((m.c1 * m.d2) - (m.d1 * m.c2))),
				-(m.a1 * ((m.b2 * m.d3) - (m.d2 * m.b3)) - m.a2 * ((m.b1 * m.d3) - (m.d1 * m.b3)) + m.a3 * ((m.b1 * m.d2) - (m.d1 * m.b2))),
				(m.a1 * ((m.b2 * m.c3) - (m.c2 * m.b3)) - m.a2 * ((m.b1 * m.c3) - (m.c1 * m.b3)) + m.a3 * ((m.b1 * m.c2) - (m.c1 * m.b2)))
			);
		}

		inline Matrix4x4 Inverse(const Matrix4x4& m) {
			const float determinant = Determinant(m);
			const float invDet = 1.0f / determinant;
			return Adjoint(m) * invDet;
		}

		inline Vector3 GetTranslation(const Matrix4x4& m) { return Vector3(m.d1, m.d2, m.d3); }

		inline Vector3 GetScale(const Matrix4x4& m) { return Vector3((m.a1 + m.a2 + m.a3) * kOneThird, (m.b1 + m.b2 + m.b3) * kOneThird, (m.c1 + m.c2 + m.c3) * kOneThird); }

		inline Matrix3x3 GetRotationMatrix(const Matrix4x4& m)
		{
			Vector3 matrixScale = GetScale(m); matrixScale = Vector3(1 / matrixScale.x, 1 / matrixScale.y, 1 / matrixScale.z);
			return Matrix3x3(
				m.a1 * matrixScale.x, m.a2 * matrixScale.x, m.a3 * matrixScale.x,
				m.b1 * matrixScale.y, m.b2 * matrixScale.y, m.b3 * matrixScale.y,
				m.c1 * matrixScale.z, m.c2 * matrixScale.z, m.c3 * matrixScale.z);
		}

		inline void SinAndCosFromRadian(float& sin, float& cos, const float rad)
		{
			sin = Sin(rad);
			cos = Cos(rad);
		}


		inline const bool AlmostEqual(const Quaternion& q1, const Quaternion& q2, float episilon = 1e-06)
		{
			return (compare(q1.x, q2.x, episilon) && compare(q1.y, q2.y, episilon), compare(q1.z, q2.z, episilon), compare(abs(q1.w), abs(q2.w), episilon)) ? true : false;
		}



		float Get2DRotation();

	}
}


#endif