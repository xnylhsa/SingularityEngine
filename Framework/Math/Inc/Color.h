#ifndef INCLUDED_MATH_COLOR_H
#define INCLUDED_MATH_COLOR_H

#include "Common.h"
#include "SimpleFloatingPointMath.h"
#include "MathForwards.h"

namespace SingularityEngine::Math
{
	struct Color
	{
		float r, g, b, a;
		Color() : r(0.0f), g(0.0f), b(0.0f), a(0.0f) { }
		Color(float R, float G, float B, float A) : r(R), g(G), b(B), a(A) {}
		Color(const Vector3& v3D, float A);

		template<typename Stream>
		bool Serialize(Stream& stream);
		static const Color Zero() { return Color(0.0f, 0.0f, 0.0f, 0.0f); }
		static const Color One() { return Color(1.0f, 1.0f, 1.0f, 1.0f); }
		static const Color Black() { return Color(0.0f, 0.0f, 0.0f, 1.0f); }
		static const Color Gray() { return Color(0.5f, 0.5f, 0.5f, 1.0f); }
		static const Color DarkGray() { return Color(0.25f, 0.25f, 0.25f, 1.0f); }
		static const Color White() { return Color(1.0f, 1.0f, 1.0f, 1.0f); }
		static const Color LightRed() { return Color(1.0f, 0.5f, 0.5f, 1.0f); }
		static const Color Red() { return Color(1.0f, 0.0f, 0.0f, 1.0f); }
		static const Color Green() { return Color(0.0f, 1.0f, 0.0f, 1.0f); }
		static const Color Blue() { return Color(0.0f, 0.0f, 1.0f, 1.0f); }
		static const Color Yellow() { return Color(1.0f, 1.0f, 0.0f, 1.0f); }
		static const Color Orange() { return Color(1.0f, 0.5f, 0.0f, 1.0f); }
		static const Color Cyan() { return Color(0.0f, 1.0f, 1.0f, 1.0f); }
		static const Color Magenta() { return Color(1.0f, 0.0f, 1.0f, 1.0f); }
		static Color Random() { return Color(((float)rand() / (float)(RAND_MAX / 1.0f)), ((float)rand() / (float)(RAND_MAX / 1.0f)), ((float)rand() / (float)(RAND_MAX / 1.0f)), 1.0f); }

		inline Color operator +(const Color& right) const { return Color(r + right.r, g + right.g, b + right.b, a + right.a); }
		inline Color operator -(const Color& right) const { return Color(r - right.r, g + right.g, b + right.b, a + right.a); }
		inline Color operator *(int right) const { return Color(r * right, g * right, b * right, a); }
		inline Color operator *(float right) const { return Color(r * right, g * right, b * right, a); }

		inline Color operator /(int right) const { ASSERT(right != 0, "[MATH::Vector4] Error Divide By Zero"); return Color(r / right, g / right, b / right, a / right); }
		inline Color operator /(float right) const { ASSERT(right != 0, "[MATH::Vector4] Error Divide By Zero"); return Color(r / right, g / right, b / right, a / right); }

		inline Color& operator +=(const Color& right) { r += right.r; g += right.g; b += right.b; a += right.a; return *this; }
		inline Color& operator -=(const Color& right) { r -= right.r; g -= right.g; b -= right.b; a -= right.a; return *this; }
		inline Color& operator *=(int right) { r *= right; g *= right; b *= right; a *= right; return *this; }
		inline Color& operator *=(float right) { r *= right; g *= right; b *= right; a *= right; return *this; }
		inline Color& operator /=(int right) { ASSERT(right != 0, "[MATH::Vector4] Error Divide By Zero"); r /= right; g /= right; b /= right; a /= right; return *this; }
		inline Color& operator /=(float right) { ASSERT(right != 0, "[MATH::Vector4] Error Divide By Zero"); r /= right; g /= right; b /= right; a /= right; return *this; }
	};

	template<typename Stream>
	bool Color::Serialize(Stream& stream)
	{
		if (!Core::Serialize_Float(stream, r))
		{
			return false;
		}
		if (!Core::Serialize_Float(stream, g))
		{
			return false;
		}
		if (!Core::Serialize_Float(stream, b))
		{
			return false;
		}
		if (!Core::Serialize_Float(stream, a))
		{
			return false;
		}
		return true;
	}

	inline Color Lerp(const Color& v1, const Color& v2, float t) { return Color(Lerp(v1.r, v2.r, t), Lerp(v1.g, v2.g, t), Lerp(v1.b, v2.b, t), Lerp(v1.a, v2.a, t)); }

}
#endif