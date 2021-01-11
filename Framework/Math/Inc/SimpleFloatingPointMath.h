#ifndef INCLUDED_FLOATING_POINT_MATH_H
#define INCLUDED_FLOATING_POINT_MATH_H
#include "Common.h"
#include "MathConstants.h"
namespace SingularityEngine::Math
{
	template<typename T>
	inline T Min(T left, T right) { return(left < right ? left : right); }
	template<typename T>
	inline T Max(T left, T right) { return (left > right ? left : right); }
	template<typename T>
	inline T Clamp(T value, T min, T max) { return Max(Min(value, max), min); }
			
	template<typename T>
	inline T Snap(T value, T min, T max) { return Min(Max( value,max),min); }

	inline float abs(float val) { return (val < 0 ? -val : val); }
	inline float Sign(float val) { return (val >= 0.0f) ? 1.0f : -1.0f; }
	inline float sqr(float val) { return  val * val; }
	inline float sqrt(float val) { return sqrtf(val); }
	const float kGoldenRatio = (1.0f + Math::sqrt(5.0f)) * 0.5f;
	inline int abs(int val) { return (val < 0 ? -val : val); }
	inline int sqr(int val) { return  val * val; }
	inline float Sin(float rad) { return std::sinf(rad); }
	inline float Cos(float rad) { return std::cosf(rad); }
	inline bool compare(float a, float b, float epislon = FLT_MIN) { return (abs(a - b) < epislon); }
	inline bool IsZero(float val) { return compare(val, 0.0f); }
	inline float Lerp(float v1, float v2, float t) { return (((v1 * (1.0f - t))) + v2 * t); }

}


#endif