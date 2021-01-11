#ifndef SINGULARITY_AI_NAVIGATION_NODE_
#define SINGULARITY_AI_NAVIGATION_NODE_
#include "Common.h"

namespace SingularityEngine::AI::Navigation
{
	class INode
	{
	public:
		void AddNeighbor(INode* newNeighbor);
		virtual float CalculateTravelCost(INode* otherNode) = 0;
		INode* GetNeighbor(size_t index);
		std::vector<INode*>& GetNeighbors();
		virtual float GetNodeWeight() = 0;
		size_t GetGraphIndex();
		INode* GetPathBack();
		float GetPathTravelCost();
		float GetGCost();

		bool IsBlocked();
		bool IsClosed();
		bool IsOpen();

		bool operator==(const INode& rhs);
		bool operator <(const INode& rhs);
		bool operator >(const INode& rhs);

		void RemoveNeighbor(INode* neighborToRemove);
		void RemoveNeighbor(size_t neighborToRemove);
		void ResetSearchParameters();
		void SetGraphIndex(size_t index);
		void SetIsBlocked(bool isBlocked);
		void SetIsClosed(bool isClosed);
		void SetIsOpen(bool isOpen);
		void SetPathTravelCost(float travelCost);
		void SetGCost(float gCost);
		void SetPathBack(INode* pathBack);
	private:
		INode * mPathBack;
		float mGCost = FLT_MAX;
		float mPathTravelCost = FLT_MAX;
		bool mIsClosed = false;
		bool mIsOpen = false;
		size_t mGraphIndex;
		bool mIsBlocked = false;
		std::vector<INode*> mNeighbors;
	};

	class IVolumeNode : public INode
	{
	public:
		IVolumeNode(Math::Vector3 pos) : mPosition(pos)
		{

		}
		virtual float CalculateTravelCost(INode* otherNode) override
		{
			IVolumeNode* other = static_cast<IVolumeNode*>(otherNode);
			float mixVal = Math::Lerp(otherNode->GetNodeWeight(), mNodeTravelWeight, 0.5f);

			return EuclideanDistanceSquared(mPosition, other->mPosition) * mixVal;
		}


		virtual float GetNodeWeight() override
		{
			return mNodeTravelWeight;
		}
	private:
		Math::Vector3 mPosition;
		float mNodeTravelWeight = 1.0f;
	};

}



#endif