#ifndef SINGULARITY_AI_NAVIGATION_GRID_
#define SINGULARITY_AI_NAVIGATION_GRID_
#include "Common.h"
#include "NavigationGraph.h"
#include "NavigationNode.h"
namespace SingularityEngine::AI::Navigation
{
	struct GridCoord
	{
		size_t x;
		size_t y;
		inline GridCoord operator +(const GridCoord& rhs)
		{
			return { x + rhs.x, y + rhs.y };
		}
	};

	class Grid : public Graph
	{
	public:

		Grid();
		~Grid();

		void Initialize(GridCoord gridSize, Math::Vector2 topLeft, float cellSize, bool allowsDiagonalMovement = true);
		void Terminate();
		const size_t GetIndexByCoord(const GridCoord coord) const;
		GridCoord GetGridSize() const { return mGridSize; }
			virtual INode * GetNode(const size_t nodeIndex) const override;
		protected:
			virtual size_t AddNode(INode* newNode) override;
			virtual void RemoveNode(uint32_t nodeIndex) override;
	private:
		GridCoord mGridSize;
	};
	class IGridNode : public INode
	{
	public:
		IGridNode(Math::Vector2 pos, GridCoord gridSize) : mPosition(pos), gridSize(gridSize)
		{

		}
		virtual float CalculateTravelCost(INode* otherNode) override
		{
			auto other = static_cast<AI::Navigation::IGridNode*>(otherNode);
			auto currentPos = GetPosition();
			auto otherPos = other->GetPosition();

			size_t currentIndex = GetGraphIndex();
			AI::Navigation::GridCoord currentCoord;

			currentCoord.y = currentIndex % gridSize.y;
			currentCoord.x = currentIndex / gridSize.y;
			size_t goalIndex = other->GetGraphIndex();
			AI::Navigation::GridCoord goalCoord;
			goalCoord.y = goalIndex % gridSize.y;
			goalCoord.x = goalIndex / gridSize.y;
			float dx = (float)goalCoord.x - (float)currentCoord.x;
			float dy = (float)goalCoord.y - (float)currentCoord.y;
			dx = Math::abs(dx);
			dy = Math::abs(dy);
			return Math::Max(dx,dy) + 0.41f * Math::Min(dx, dy);
		}

		const Math::Vector2 GetPosition() const { return mPosition; };
		virtual float GetNodeWeight() override
		{
			return mNodeTravelWeight;
		}
	private:
		GridCoord gridSize;

		Math::Vector2 mPosition;
		float mNodeTravelWeight = 1.0f;
	};

}


#endif 