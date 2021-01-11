#ifndef INCLUDED_MATH_QUATERNION_H
#define INCLUDED_MATH_QUATERNION_H

#include "Common.h"
#include "SimpleFloatingPointMath.h"
#include "MathForwards.h"

namespace SingularityEngine::Math
{
	struct Quaternion
	{
	public:
		float x, y, z, w;
		inline Quaternion() { x = 0.0f; y = 0.0f; z = 0.0f; w = 1.0f; }
		inline Quaternion(float X, float Y, float Z, float W) : x(X), y(Y), z(Z), w(W) { }

		inline Quaternion(const Vector3& axisAngle, float rotationAngle /*degrees*/);
		inline Quaternion(const Quaternion& q) { x = q.x; y = q.y; z = q.z; w = q.w; }


		const static Quaternion Zero() { return Quaternion(0.0f, 0.0f, 0.0f, 0.0f); }
		const static Quaternion Identity() { return Quaternion(0.0f, 0.0f, 0.0f, 1.0f); }
		inline Quaternion Conjugate() const { return Quaternion(-x, -y, -z, w); }
		inline const float Magnitude() const { return sqrt(sqr(x) + sqr(y) + sqr(z) + sqr(w)); }

		inline const float Norm() const
		{
			float mag = Magnitude();
			return abs(mag);
		}

		inline Quaternion Inverse() const { return Conjugate() * (1 / Norm()); }
		inline void Normalize()
		{
			float n = Norm();
			float inverseN = 1 / n;
			x *= inverseN; y *= inverseN; z *= inverseN; w *= inverseN;
		}
		inline Quaternion operator -() const { return Quaternion(-x, -y, -z, w); }

		inline Quaternion operator +(const Quaternion& right) const { return Quaternion(x + right.x, y + right.y, z + right.z, w + right.w); }
		inline Quaternion operator -(const Quaternion& right) const { return Quaternion(x - right.x, y - right.y, z - right.z, w - right.w); }
		inline Quaternion operator *(const float scalar) const { return Quaternion(x * scalar, y * scalar, z * scalar, w * scalar); }
		inline Quaternion operator /(const float scalar) const { ASSERT(scalar != 0, "[MATH::Quaternion] Error Divide By Zero"); float invScale = 1.0f / scalar; return Quaternion(x * invScale, y * invScale, z  * invScale, w  * invScale); }
		inline Quaternion operator *(const Quaternion& rhd) const;
		inline Quaternion& operator +=(const Quaternion& right) { x += right.x; y += right.y; z += right.z; w += right.w; return *this; }
		inline Quaternion& operator -=(const Quaternion& right) { x -= right.x; y -= right.y; z -= right.z; w -= right.w; return *this; }
		inline bool operator ==(const Quaternion& right) { return (x == right.x && y == right.y && z == right.z && w == right.w) ? true : false; }
		inline bool operator !=(const Quaternion& right) { return (x != right.x || y != right.y || z != right.z || w != right.w) ? true : false; }

		template<typename Stream>
		bool Serialize(Stream& stream);
	};
	template<typename Stream>
	inline bool Quaternion::Serialize(Stream& stream)
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

	inline Quaternion Normalize(const Quaternion& val) { float n = abs(val.Magnitude()); return Quaternion(val.x / n, val.y / n, val.z / n, val.w / n); }

	inline float MagnitudeSqr(const Quaternion& val) { return sqr(val.x) + sqr(val.y) + sqr(val.z) + sqr(val.w); }

	inline float Magnitude(const Quaternion& val) { return sqrt(MagnitudeSqr(val)); }

	inline Quaternion Lerp(const Quaternion& v1, const Quaternion& v2, float t) { return Quaternion(Lerp(v1.x, v2.x, t), Lerp(v1.y, v2.y, t), Lerp(v1.z, v2.z, t), Lerp(v1.w, v2.w, t)); }

	inline Quaternion SLerp(const Quaternion& v1, const Quaternion& v2, float t)
	{
		Quaternion qm;
		float cosHalfTheta = v1.w * v2.w + v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
		if (Math::abs(cosHalfTheta) >= 1.0f) {
			qm.w = v1.w; qm.x = v1.x; qm.y = v1.y; qm.z = v1.z;
			return qm;
		}
		float halfTheta = acos(cosHalfTheta);
		float sinHalfTheta = Math::sqrt(1.0f - cosHalfTheta * cosHalfTheta);

		if (fabs(sinHalfTheta) < 0.001f) {
			qm.w = (v1.w * 0.5f + v2.w * 0.5f);
			qm.x = (v1.x * 0.5f + v2.x * 0.5f);
			qm.y = (v1.y * 0.5f + v2.y * 0.5f);
			qm.z = (v1.z * 0.5f + v2.z * 0.5f);
			qm.Normalize();
			return qm;
		}
		float ratioA = sin((1.0f - t) * halfTheta) / sinHalfTheta;
		float ratioB = sin(t * halfTheta) / sinHalfTheta;
		qm.w = (v1.w * ratioA + v2.w * ratioB);
		qm.x = (v1.x * ratioA + v2.x * ratioB);
		qm.y = (v1.y * ratioA + v2.y * ratioB);
		qm.z = (v1.z * ratioA + v2.z * ratioB);
		qm.Normalize();
		return qm;
	}

	inline float Dot(const Quaternion& lhs, const Quaternion& rhs) { return (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z) + lhs.w * rhs.w; }

	inline Quaternion Quaternion::operator*(const Quaternion& rhd) const
	{
		Quaternion q;
		q.w = w * rhd.w - x * rhd.x - y * rhd.y - z * rhd.z;
		q.x = x * rhd.w + rhd.x * w + y * rhd.z - z * rhd.y;
		q.y = y * rhd.w + w * rhd.y + z * rhd.x - x * rhd.z;
		q.z = z * rhd.w + w * rhd.z + x * rhd.y - y * rhd.x;
		q.Normalize();
		return q;
	}
}
#endif