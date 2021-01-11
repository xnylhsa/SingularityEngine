#ifndef INCLUDED_SINGULARITY_3DPHYSICSMATH_H_
#define INCLUDED_SINGULARITY_3DPHYSICSMATH_H_
#include "Common.h"


namespace SingularityEngine::Math
{

	inline OBB::OBB(const Vector3& p, const Vector3& s, const Vector3& eulerOrientation) : center(p), size(s)
	{
		orientation = EulerAnglesToQuaternion(eulerOrientation);
	}

	inline bool ContainsPoint(const Vector3& point, const AABB& box)
	{
		Vector3 min = box.GetMin();
		Vector3 max = box.GetMax();
		if (point.x < min.x || point.y < min.y || point.z < min.z)
		{
			return false;
		}
		if (point.x > max.x || point.y > max.y || point.z > max.z)
		{
			return false;
		}
		return true;
	}
	inline bool ContainsPoint(const Vector3& point, const OBB& box)
	{
		Vector3 Dir = point - box.center;

		Vector3 xAxis = Rotate(box.orientation, Vector3::AxisX());
		Vector3 yAxis = Rotate(box.orientation, Vector3::AxisY());
		Vector3 zAxis = Rotate(box.orientation, Vector3::AxisZ());
		float distanceX = Dot(Dir, xAxis);
		if (distanceX > box.size.x)
		{
			return false;
		}
		if (distanceX < -box.size.x)
		{
			return false;
		}

		float distanceY = Dot(Dir, yAxis);
		if (distanceY > box.size.y)
		{
			return false;
		}
		if (distanceY < -box.size.y)
		{
			return false;
		}

		float distanceZ = Dot(Dir, zAxis);
		if (distanceZ > box.size.z)
		{
			return false;
		}
		if (distanceZ < -box.size.z)
		{
			return false;
		}
	}


	inline bool ContainsPoint(const Vector3& pos, const Sphere& sphere)
	{
		return DistanceSqr(pos, sphere.position) <= sphere.radius * sphere.radius;
	}

	inline bool ContainsPoint(const Vector3& pos, const Plane& plane)
	{
		float dotProduct = Dot(pos, plane.normal);
		return compare(dotProduct - plane.distance, 0.0f, 0.1f);
	}
	inline Vector3 ClosestPoint(const Vector3& point, const AABB& box)
	{
		Vector3 min = box.GetMin();
		Vector3 max = box.GetMax();
		Vector3 closestPoint;
		closestPoint.x = Clamp(point.x, min.x, max.x);
		closestPoint.y = Clamp(point.y, min.y, max.y);	
		closestPoint.z = Clamp(point.z, min.z, max.z);
		return closestPoint;
	}
	inline Vector3 ClosestPoint(const Vector3& pos, const Plane& plane)
	{
		float dotProduct = Dot(pos, plane.normal);
		float dist = dotProduct - plane.distance;
		return pos - plane.normal * dist;
	}
	inline Vector3 ClosestPoint(const Vector3& pos, const OBB& box)
	{
		Vector3 Dir = pos - box.center;
		return Dir;
	}
	inline Vector3 ClosestPoint(const Vector3& pos, const Sphere& sphere)
	{
		Vector3 dir = sphere.position - pos;
		return pos + dir * sphere.radius;
	}

	inline Vector3 ClosestPoint(const Line& line, const Vector3& pos)
	{
		Vector3 lDir = line.end - line.start;
		float t = Dot(pos - line.start, lDir) / Dot(lDir, lDir);
		t = Clamp(t, 0.0f, 1.0f);
		return Lerp(line.start, line.end, t);
	}

	inline Vector3 ContainsPoint(const Vector3& pos, const Line& line)
	{
		Vector3 closest = ClosestPoint(line, pos);
		float dist = MagnitudeSqr(closest - pos);
		return dist == 0.0f;
	}

	inline bool Linetest(const Sphere& sphere, const Line& line)
	{
		Vector3 closestPoint = ClosestPoint(line, sphere.position);
		float distSq = MagnitudeSqr(sphere.position - closestPoint);
		return distSq <= (sphere.radius * sphere.radius);
	}

	inline bool Linetest(const Plane& plane, const Line& line)
	{
		Vector3 ab = line.end - line.start;
		float nDotA = Dot(plane.normal, line.start);
		float nDotAB = Dot(plane.normal, ab);
		if (nDotAB == 0.0f)
		{
			return false;
		}
		float t = (plane.distance - nDotA) / nDotAB;
		return t >= 0.0f && t <= 1.0f;
	}

	inline float Raycast(const Ray& ray, const Plane& plane)
	{
		float nDot = Dot(ray.direction, plane.normal);
		if (nDot >= 0.0f)
			return -1.0f;

		float planeDot = Dot(ray.origin, plane.normal);
		float t = (plane.distance - planeDot) / nDot;
		if (t >= 0.0f)
			return t;

		return -1.0f;
	}

	inline float Raycast(const Ray& ray, const Sphere& sphere)
	{
		Vector3 rayToCenter = ray.origin - sphere.position;
		float distToCenter = Magnitude(rayToCenter);
		float theta = Dot(rayToCenter, ray.direction);
		float rSq = sphere.radius * sphere.radius;
		float distToCenterProjectedSqr = distToCenter + (theta * theta);
		float distToConnectionPoint = sqrtf(rSq - distToCenterProjectedSqr);
		if (rSq - (distToCenterProjectedSqr - (theta*theta)) == 0.0f)
		{
			return -1;
		}
		else if (distToCenterProjectedSqr < rSq)
		{
			return theta + distToConnectionPoint;
		}

		return theta - distToConnectionPoint;

	}

	inline bool Intersects(const Sphere& a, const Sphere& b)
	{
		float radiiSum = a.radius + b.radius;
		radiiSum *= radiiSum;
		return Math::DistanceSqr(a.position, b.position) < radiiSum;
	}
	inline bool Intersects(const Sphere& s, const Plane& p)
	{
		return (p.normal.x*s.position.x + p.normal.y * s.position.y + p.normal.z * s.position.z + p.distance) < s.radius;
	}

	inline std::vector<Vector3> FABRIKInverseKinematics(const Vector3 origin, const Vector3 goal, const std::vector<Vector3> previousPoints)
	{
		std::vector<float> lengths(previousPoints.size() - 1);
		for (size_t i = 0; i < previousPoints.size() - 1; i++)
		{
			lengths[i] = Math::Distance(previousPoints[i], previousPoints[i+1]);
		}
		std::vector<Vector3> returnPoints(previousPoints.size());

		float sum = 0;
		for (size_t i = 0; i < lengths.size(); i++)
		{
			sum += lengths[i];
		}
		const float totalLength = sum;
		Line goalLine(origin, goal);

		if (goalLine.Length() > totalLength)
		{

			Ray goalRay(goalLine);
			sum = 0;
			returnPoints[0] = origin;

			for (size_t i = 1; i < previousPoints.size(); i++)
			{
					sum += lengths[i-1];
					returnPoints[i] = goalRay.origin + goalRay.direction * sum;
			}
			return returnPoints;
		}
		std::vector<Vector3> forwardPoints(previousPoints.size());
		std::vector<Vector3> backwardPoints(previousPoints.size());
		while (true)
		{

			backwardPoints[backwardPoints.size() - 1] = goal;
			for (size_t i = previousPoints.size() - 1; i > 0; i--)
			{
				Vector3 constainPos = (i == previousPoints.size() - 1) ? backwardPoints[backwardPoints.size() - 1] : backwardPoints[i + 1];
				Ray linkRay(constainPos, Normalize((previousPoints[i] - constainPos)));

				backwardPoints[i] = linkRay.origin + linkRay.direction * lengths[i - 1];
			}


			forwardPoints[0] = origin;
			for (size_t i = 1; i < backwardPoints.size(); i++)
			{
				Vector3 constainPos = i == 0 ? forwardPoints[0] : forwardPoints[i - 1];
				Ray linkRay(constainPos, Normalize((backwardPoints[i] - constainPos)));
				forwardPoints[i] = linkRay.origin + linkRay.direction * lengths[i - 1];
			}
			if (Math::DistanceSqr(forwardPoints[forwardPoints.size() - 1], goal) <= 1.0f)
			{
				return forwardPoints;
			}
		}
		return previousPoints;
	}





}
#endif