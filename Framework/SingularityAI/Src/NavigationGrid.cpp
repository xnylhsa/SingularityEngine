#include "Precompiled.h"
#include "NavigationGrid.h"
#include "NavigationNode.h"
using namespace SingularityEngine;
using namespace AI::Navigation;




Grid::Grid()
{

}

void Grid::Initialize(GridCoord gridSize, Math::Vector2 topLeft, float cellSize, bool allowsDiagonalMovement)
{
	Math::Vector2 bottomRight(topLeft.x + (gridSize.x -1)* cellSize, topLeft.y + (gridSize.y - 1)*cellSize);
	mGridSize = gridSize;
	for (float x = topLeft.x; x <= bottomRight.x; x += cellSize)
	{
		for (float z = topLeft.y; z <= bottomRight.y; z += cellSize)
		{
			IGridNode* addedNode = new IGridNode(Math::Vector2(x, z), gridSize);
			mNavigationNodes.push_back(addedNode);
		}
	}
	for (size_t x = 0; x < gridSize.x; x++)
	{
		for (size_t z = 0; z < gridSize.y; z++)
		{
			GridCoord thisNodeCoord = { x,z };
			size_t index = GetIndexByCoord(thisNodeCoord);
			INode* currentNode = GetNode(index);
			currentNode->SetGraphIndex(index);
			if (allowsDiagonalMovement)
			{
				std::vector<size_t> indices = { 0 ,1 ,2, 3, 4, 5, 6, 7 };
				std::vector<int> Xoffsets = { -1,0,1,1,1,0,-1,-1 };
				std::vector<int> Yoffsets = { 1,1,1,0,-1,-1,-1,0 };
				std::random_device rd;
				std::mt19937 g(rd());

				//std::shuffle(indices.begin(), indices.end(), g);

				for (int i = 0; i < 8; i++)
				{
					GridCoord connectedNodeCoord = { thisNodeCoord.x + Xoffsets[indices[i]], thisNodeCoord.y + Yoffsets[indices[i]] };
					if (connectedNodeCoord.x >= 0 && connectedNodeCoord.x < gridSize.x && connectedNodeCoord.y >= 0 && connectedNodeCoord.y < gridSize.y)
					{
						size_t connectedNodeIndex = GetIndexByCoord(connectedNodeCoord);
						INode* connectedNode = GetNode(connectedNodeIndex);
						currentNode->AddNeighbor(connectedNode);
					}
				}
			}
			else
			{
				std::vector<int> Xoffsets = { 0,0,-1,1 };
				std::vector<int> Yoffsets = { 1,-1 ,0,0};
				for (int i = 0; i < 4; i++)
				{
					GridCoord connectedNodeCoord = { thisNodeCoord.x + Xoffsets[i], thisNodeCoord.y + Yoffsets[i] };

					if (connectedNodeCoord.x >= 0 && connectedNodeCoord.x <= gridSize.x && connectedNodeCoord.y >= 0 && connectedNodeCoord.y <= gridSize.y)
					{
						size_t connectedNodeIndex = GetIndexByCoord(connectedNodeCoord);
						INode* connectedNode = GetNode(connectedNodeIndex);
						currentNode->AddNeighbor(connectedNode);
					}
				}
			}
		}
	}
	mInitialized = true;
}

void Grid::Terminate()
{
	mNavigationNodes.clear();
	mInitialized = false;
}
Grid::~Grid()
{

}

void Grid::RemoveNode(uint32_t )
{
	ASSERT(false, "[AI::Navigation] Grid should be statically sized!");

}

size_t Grid::AddNode(INode* )
{
	ASSERT(false, "[AI::Navigation] Grid should be statically sized!");
	return 0;
}

INode* Grid::GetNode(const size_t nodeIndex) const
{
	ASSERT(nodeIndex < mNavigationNodes.size(), "[AI::Navigation] Index was larger than size of grid");
	return mNavigationNodes[nodeIndex];
}

const size_t Grid::GetIndexByCoord(const GridCoord coord) const
{
	return coord.x * mGridSize.y + coord.y;
}
