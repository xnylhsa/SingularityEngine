#ifndef INCLUDED_SINGULARITY_2DPHYSICSMATH_H_
#define INCLUDED_SINGULARITY_2DPHYSICSMATH_H_
#include "Common.h"

namespace SingularityEngine::Math
{
	// ********************************************************************
	// ****                Contains Point Checks 2D                    ****
	// ********************************************************************
	inline bool ContainsPoint(const Vector2& point, const Line2D& line)
	{
		float dy = (line.end.y - line.start.y);
		float dx = (line.end.x - line.start.x);
		float slope = dy / dx;
		float B = line.start.y - slope * line.start.x;
		return compare(point.y, slope * point.x + B);
	}
	inline bool ContainsPoint(const Line2D& line, const Vector2& point) { return ContainsPoint(point, line); }

	inline bool ContainsPoint(const Vector2& point, const Circle& circle)
	{
		Line2D line(point, circle.position);
		//check length of line is less or equal to radius
		return (LengthSq(line) < circle.radius * circle.radius);
	}
	inline bool ContainsPoint(const Circle& circle, const Vector2& point) { return ContainsPoint(point, circle); }

	inline bool ContainsPoint(const Vector2& point, const Rectangle& rect)
	{
		Vector2 min = GetMin(rect);
		Vector2 max = GetMax(rect);
		bool withinYBounds = min.y <= point.y && point.y <= max.y;
		bool withinXBounds = min.x <= point.x && point.x <= max.x;
		return withinYBounds && withinXBounds;
	}
	inline bool ContainsPoint(const Rectangle& rect, const Vector2& point) { return ContainsPoint(point, rect); }

	inline bool ContainsPoint(const Vector2& point, const OrientedRectangle& rect)
	{
		Vector2 rotVector = point - rect.position;
		float theta = -rect.rotationInDegrees * Math::kDegtoRad;
		Math::Matrix3x3 rotMat = Math::Matrix3x3::Rotation(theta);
		ApplyTransformation(rotVector, rotMat);

		Rectangle localRectangle(Vector2(),
			rect.halfExtent * 2.0f);
		Vector2 localPoint = rotVector + rect.halfExtent;
		return ContainsPoint(localPoint, localRectangle);
	}
	inline bool ContainsPoint(const OrientedRectangle& rect, const Vector2& point) { return ContainsPoint(point, rect); }

	// ********************************************************************
	// ****                Line Intersection Tests 2D                  ****
	// ********************************************************************

	inline bool Intersects(const Line2D& line, const Circle& circle)
	{
		Vector2 ab = line.end - line.start;
		float t = Dot(circle.position - line.start, ab) / Dot(ab, ab);
		if (t < 0.0f || t > 1.0f) return false;
		Vector2 closestPoint = line.start + ab * t;
		Line2D circleToClosest(circle.position, closestPoint);
		return LengthSq(circleToClosest) < circle.radius * circle.radius;
	}
	inline bool Intersects(const Circle& circle, const Line2D& line) { return Intersects(line, circle); }

	inline bool Intersects(const Line2D& line, const Rectangle& rect)
	{
		if (ContainsPoint(line.start, rect) || ContainsPoint(line.end, rect))
		{
			return true;
		}

		Vector2 norm = Normalize(line.end - line.start);
		norm.x = (norm.x != 0.0f) ? (1.0f / norm.x) : 0.0f;
		norm.y = (norm.y != 0.0f) ? (1.0f / norm.y) : 0.0f;
		Vector2 min = (GetMin(rect) - line.start) * norm;
		Vector2 max = (GetMax(rect) - line.start) * norm;

		float tMin = Max<float>(Min<float>(min.x, max.x), Min<float>(min.y, max.y));
		float tMax = Min<float>(Max<float>(min.x, max.x), Max<float>(min.y, max.y));
		if (tMax < 0 || tMin > tMax)
		{
			return false;
		}
		float t = (tMin < 0.0f) ? tMax : tMin;
		return t > 0.0f && t * t < LengthSq(line);
	}
	inline bool Intersects(const Rectangle& rect, const Line2D& line) { return Intersects(line, rect); }

	inline bool Intersects(const Line2D& line, const OrientedRectangle& rect)
	{
		float theta = -Math::kDegtoRad * rect.rotationInDegrees;
		Math::Matrix3x3 rotMat = Math::Matrix3x3::Rotation(theta);
		Line2D localLine;
		Vector2 rotVector = line.start - rect.position;
		ApplyTransformation(rotVector, rotMat);
		localLine.start = rotVector + rect.halfExtent;
		rotVector = line.end - rect.position;
		ApplyTransformation(rotVector, rotMat);
		localLine.end = rotVector + rect.halfExtent;
		Rectangle localRect(Vector2(), rect.halfExtent*2.0f);
		return Intersects(localLine, localRect);
	}
	inline bool Intersects(const OrientedRectangle& rect, const Line2D& line) { return Intersects(line, rect); }


	// ********************************************************************
	// ****                      Collision Checks                      ****
	// ********************************************************************


	inline bool IsColliding(const Circle& circleA, const Circle& circleB)
	{
		float radii = circleA.radius + circleB.radius;
		return DistanceSqr(circleA.position, circleB.position) < radii * radii;
	}

	inline bool IsColliding(const Circle& circle, const Rectangle& rect)
	{
		Vector2 min = GetMin(rect);
		Vector2 max = GetMax(rect);
		Vector2 closestPoint(Clamp(circle.position.x, min.x, max.x), Clamp(circle.position.y, min.y, max.y));
		return DistanceSqr(closestPoint, circle.position) < circle.radius * circle.radius;
	}
	inline bool IsColliding(const Rectangle& rect, const Circle& circle) { return IsColliding(circle, rect); }

	inline bool IsColliding(const Circle& circle, const OrientedRectangle& rect)
	{
		Vector2 rotationVec = circle.position - rect.position;
		float theta = -rect.rotationInDegrees * kDegtoRad;
		Matrix3x3 localTransform = Matrix3x3::Rotation(theta);
		Circle localCircle(ApplyTransformation(rotationVec, localTransform) + rect.halfExtent, circle.radius);
		Rectangle localRect(Vector2(), rect.halfExtent * 2.0f);
		return IsColliding(localCircle, localRect);
	}
	inline bool IsColliding(const OrientedRectangle& rect, const Circle& circle){ return IsColliding(circle, rect); }



	// ********************************************************************
	// ****                    Seperating Axis Theory                  ****
	// ********************************************************************

	//======================
	//====   Intervals  ====
	//======================

	struct Interval2D
	{
		float min;
		float max;
	};

	inline Interval2D GetInterval(const Rectangle& rect, const Vector2& axis)
	{
		Interval2D result;

		Vector2 min = GetMin(rect);
		Vector2 max = GetMax(rect);
		Vector2 vertices[4] = { Math::Vector2(min.x, min.y), Math::Vector2(min.x, max.y) , Math::Vector2(max.x, max.y) , Math::Vector2(max.x, min.y) };
		result.min = result.max = Dot(axis, vertices[0]);
		for (size_t i = 1; i < 4; i++)
		{
			float projection = Dot(axis, vertices[i]);
			result.min = (projection < result.min) ? projection : result.min;
			result.max = (projection > result.max) ? projection : result.max;
		}
		return result;
	}

	inline Interval2D GetInterval(const OrientedRectangle& oRect, const Vector2& axis)
	{
		Rectangle rect(Vector2(oRect.position - oRect.halfExtent), oRect.halfExtent * 2.0f);
		Vector2 min = GetMin(rect);
		Vector2 max = GetMax(rect);
		Vector2 vertices[4] = { min, Math::Vector2(min.x, max.y) , max, Math::Vector2(max.x, min.y) };

		float theta = kDegtoRad * oRect.rotationInDegrees;
		Matrix3x3 rotationMat = Matrix3x3::Rotation(theta);
		for (size_t i = 0; i < 4; i++)
		{
			Vector2 rotatedV = vertices[i] - rect.position;

			rotatedV = ApplyTransformation(rotatedV, rotationMat);
			vertices[i] = rotatedV + rect.position;
		}
		Interval2D result;
		result.min = result.max = Dot(axis, vertices[0]);
		for (size_t i = 1; i < 4; i++)
		{
			float projection = Dot(axis, vertices[i]);
			result.min = (projection < result.min) ? projection : result.min;
			result.max = (projection > result.max) ? projection : result.max;
		}
		return result;
	}


	//===================================
	//====   Overlap on Axis Checks  ====
	//===================================
	inline bool OverlapOnAxis(const Rectangle& rectA, const Rectangle& rectB, const Vector2& axis)
	{
		Interval2D a = GetInterval(rectA, axis);
		Interval2D b = GetInterval(rectB, axis);
		bool isAMinLeftOfBMax = a.min <= b.max;
		bool isAMaxRightOfBMin = b.min <= a.max;
		return isAMinLeftOfBMax && isAMaxRightOfBMin;
	}
	inline bool OverlapOnAxis(const OrientedRectangle& rectA, const Rectangle& rectB, const Vector2& axis)
	{
		Interval2D a = GetInterval(rectA, axis);
		Interval2D b = GetInterval(rectB, axis);
		bool isAMinLeftOfBMax = a.min <= b.max;
		bool isAMaxRightOfBMin = b.min <= a.max;
		return isAMinLeftOfBMax && isAMaxRightOfBMin;
	}

	//===============================================
	//====    Seperating Axis Theory Collision   ====
	//===============================================

	inline bool IsColliding(const Rectangle& rectA, const Rectangle& rectB)
	{
		if (!OverlapOnAxis(rectA, rectB, Vector2::AxisX()))
			return false;
		if (!OverlapOnAxis(rectA, rectB, Vector2::AxisY()))
			return false;
		return true;
	}

	inline bool IsColliding(const OrientedRectangle& rectA, const Rectangle& rectB)
	{
		float theta = kDegtoRad * rectA.rotationInDegrees;
		Matrix3x3 rotationMat = Matrix3x3::Rotation(theta);
		Vector2 testAxis[4] = { Vector2::AxisX(),Vector2::AxisY(), Normalize(ApplyTransformation(Vector2::AxisX(), rotationMat)),Normalize(ApplyTransformation(Vector2::AxisY(), rotationMat)) };
		for (size_t i = 0; i < 4; i++)
		{
			if (!OverlapOnAxis(rectA, rectB, testAxis[i]))
				return false;
		}
		return true;
	}
	inline bool IsColliding(const Rectangle& rectA, const OrientedRectangle& rectB) { return IsColliding(rectB, rectA); }

	inline bool IsColliding(const OrientedRectangle& oRectA, const OrientedRectangle& oRectB)
	{
		Rectangle localA(Vector2::Zero(), oRectA.halfExtent * 2.0f);
		Vector2 rotation = oRectB.position - oRectA.position;
		OrientedRectangle localORectB(oRectB.position, oRectB.halfExtent, oRectB.rotationInDegrees);
		localORectB.rotationInDegrees = oRectB.rotationInDegrees - oRectA.rotationInDegrees;
		float theta = kDegtoRad * oRectA.rotationInDegrees;
		Matrix3x3 rotationMat = Matrix3x3::Rotation(-theta);
		ApplyTransformation(rotation, rotationMat);
		localORectB.position = rotation + oRectA.halfExtent;
		return IsColliding(localORectB, localA);
	}

	//===============================================
	//====        Constraining Volume 2D         ====
	//===============================================

	inline Circle ContainingCircle(const Vector2* positions, size_t numPositions)
	{
		size_t sizeOf = sizeof(positions);
		ASSERT(sizeOf / sizeof(Vector2) == numPositions, "[Math::Physics] Passed in incorrect size of position array!");

		Vector2 center(0.0f,0.0f);
		for (size_t i = 0; i < numPositions; i++)
		{
			center = center + positions[i];
		}
		center = center * (1.0f / numPositions);
		Circle result;
		result.position = center;
		result.radius = MagnitudeSqr(center - positions[0]);
		for (size_t i = 1; i < numPositions; i++)
		{
			float distance = MagnitudeSqr(center - positions[i]);
			if (distance > result.radius)
				result.radius = distance;
		}
		result.radius = sqrt(result.radius);
		return result;
	}

	inline Rectangle ContainingRectangle(const Vector2* positions, size_t numPositions)
	{
		size_t sizeOf = sizeof(positions);
		ASSERT(sizeOf /sizeof(Vector2) == numPositions, "[Math::Physics] Passed in incorrect size of position array!");

		Vector2 min = positions[0];
		Vector2 max = positions[0];
		for (size_t i = 1; i < numPositions; ++i) 
		{
			min.x = positions[i].x < min.x ? positions[i].x : min.x;
			min.y = positions[i].y < min.y ? positions[i].y : min.y;

			max.x = positions[i].x > max.x ? positions[i].x : max.x;
			max.y = positions[i].y > max.y ? positions[i].y : max.y;
		}
		return FromMinMax(min, max);
	}


	// ********************************************************************
	// ****                    Collision Point                         ****
	// ********************************************************************


	inline Math::Vector2 FindCollisionPoint(const Circle& circleA, const Circle& circleB)
	{
		Math::Vector2 collisionPoint;
		collisionPoint.x = ((circleA.position.x * circleB.radius) + (circleB.position.x * circleA.radius)) / (circleA.radius + circleB.radius);
		collisionPoint.y = ((circleA.position.y * circleB.radius) + (circleB.position.y * circleA.radius)) / (circleA.radius + circleB.radius);
		return collisionPoint;
	}

	inline void ElasticCollision(const Vector2& initialVelocityA, const float massA, const Vector2& initialVelocityB, const float massB, Vector2& finalVelA, Vector2& finalVelB)
	{
		const float totalMass = massA + massB;
		const float invTotalMass = 1.0f / totalMass;

		finalVelA.x = initialVelocityA.x * ((massA - massB) * invTotalMass) + initialVelocityB.x * ((2 * massB)  * invTotalMass);
		finalVelA.y = initialVelocityA.y * ((massA - massB) * invTotalMass) + initialVelocityB.y * ((2 * massB)  * invTotalMass);

		finalVelB.x = initialVelocityA.x * ((2 * massB)  * invTotalMass) + initialVelocityB.x * ((massB - massA) * invTotalMass);
		finalVelB.y = initialVelocityA.y * ((2 * massB)  * invTotalMass) + initialVelocityB.y * ((massB - massA) * invTotalMass);
	}
	inline Vector2 OneDimensionalElasticCollison(const Vector2& initialVelocityA, const float massA, const Vector2& initialVelocityB, const float massB, const float theta)
	{
		Vector2 localInitialVelocityA = Math::Rotate(initialVelocityA, theta);
		Vector2 localInitialVelocityB = Math::Rotate(initialVelocityB, theta);
		const float totalMass = massA + massB;
		const float invTotalMass = 1.0f / totalMass;

		Vector2 finalVel;
		finalVel.x = localInitialVelocityA.x * ((massA - massB) * invTotalMass) + localInitialVelocityB.x * ((2 * massB)  * invTotalMass);
		finalVel.y = localInitialVelocityA.y;
		return Math::Rotate(finalVel, -theta);
	}

}
#endif
