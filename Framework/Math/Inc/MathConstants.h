#ifndef INCLUDED_MATH_CONSTANTS_H
#define INCLUDED_MATH_CONSTANTS_H
#include "Common.h"

namespace SingularityEngine::Math
{
	const float kOneThird = 1 / 3;
	const float kPi = 3.14159265358979323846f;
	const float kDegtoRad = kPi / 180.0f;
	const float kPimul2 = 6.28318530718f;  // pi*2
	const float kPidiv2 = 1.57079632679f;  // pi/2
	const float kPimul3div2 = 2.09439510239f;   // 3*pi/2
	const float kPirev = 0.31830988618f; // 1/pi
	const float kRadToDeg = 180.0f / kPi;

}

#endif