#ifndef INCLUDED_GRAPHICS_VERTEX_H
#define INCLUDED_GRAPHICS_VERTEX_H
#include "Common.h"

#define VF_POSITION 0x01 << 0
#define VF_NORMAL	0x01 << 1
#define VF_TANGENT  0x01 << 2
#define VF_COLOR	0x01 << 3
#define VF_UV		0x01 << 4
#define VF_BINDEX	0x01 << 5
#define VF_BWEIGHT	0x01 << 6

namespace SingularityEngine
{
	namespace Graphics
	{
		//TODO move this into a graphics base, create a better windowing system for multiple different OS, and then allow more renderer types.
		struct VertexP
		{
			static const uint32_t Format = VF_POSITION;
			Math::Vector3 position;
		};
		struct VertexPC
		{
			static const uint32_t Format = VF_POSITION | VF_COLOR;
			Math::Vector3 position{ 0.0f,0.0f,0.0f };
			Math::Color color{ 0.0f,0.0f,0.0f,1.0f };
			VertexPC()
			{

			}
			VertexPC(const Math::Vector3& pos, const Math::Color& col) : position(pos), color(col)
			{

			}
		};
		struct VertexPosColTex
		{
			static const uint32_t Format = VF_POSITION | VF_COLOR | VF_UV;
			Math::Vector3 position;
			Math::Color color{ 0.0f,0.0f,0.0f,1.0f };
			Math::Vector2 texCoord = Math::Vector2::Zero();
		};
		struct VertexPT
		{
			static const uint32_t Format = VF_POSITION | VF_TANGENT;
			Math::Vector3 position;
			Math::Vector3 tangent;
		};
		struct Vertex
		{
			static const uint32_t Format = VF_POSITION | VF_NORMAL | VF_TANGENT | VF_UV;
			Math::Vector3 position = Math::Vector3::Zero();

			Math::Vector3 normal = Math::Vector3::Zero();
			Math::Vector3 tangent = Math::Vector3::Zero();
			Math::Vector2 texCoord = Math::Vector2::Zero();
		};
		struct VertexBone
		{
			static const uint32_t Format = VF_POSITION | VF_NORMAL | VF_TANGENT | VF_UV | VF_BINDEX | VF_BWEIGHT;
			Math::Vector3 position = Math::Vector3::Zero(); // 4 *3 12

			Math::Vector3 normal = Math::Vector3::Zero(); // 4 *3 + ^ = 24
			Math::Vector3 tangent = Math::Vector3::Zero();// 4 *3 + ^ = 36
			Math::Vector2 texCoord = Math::Vector2::Zero(); // 4 *2 + ^ = 44

			uint32_t indices[4] = { 0,0,0,0 }; // 4 *4 + ^ = 60
			float weights[4] = { 0.0f,0.0f,0.0f,0.0f };
		};


	}
}


#endif // INCLUDED_GRAPHICS_VERTEX_H
