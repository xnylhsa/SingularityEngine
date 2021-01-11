#include "Precompiled.h"
#include "NavigationNode.h"

using namespace SingularityEngine::AI::Navigation;


void INode::AddNeighbor(INode* newNeighbor)
{
	mNeighbors.push_back(newNeighbor);
}
INode* INode::GetNeighbor(size_t index)
{
	ASSERT(index >= 0 && index < mNeighbors.size(), "[AI::NavigationNode] index was larger than the size of the vector");
	return mNeighbors[index];
}

std::vector<INode*>& INode::GetNeighbors()
{
	return mNeighbors;
}

float INode::GetGCost()
{
	return mGCost;
}

size_t INode::GetGraphIndex()
{
	return mGraphIndex;
}

INode* INode::GetPathBack()
{
	return mPathBack;
}

float INode::GetPathTravelCost()
{
	return mPathTravelCost;
}

bool INode::IsBlocked()
{
	return mIsBlocked;
}

bool INode::IsClosed()
{
	return mIsClosed;
}
bool INode::IsOpen()
{
	return mIsOpen;
}

bool INode::operator==(const INode& rhs)
{
	return mGraphIndex == rhs.mGraphIndex;
}
bool INode::operator<(const INode& rhs)
{
	return mPathTravelCost < rhs.mPathTravelCost;
}
bool INode::operator>(const INode& rhs)
{
	return mPathTravelCost > rhs.mPathTravelCost;
}


void INode::RemoveNeighbor(INode* neighborToRemove)
{
	for (auto neighbor = mNeighbors.begin(); neighbor != mNeighbors.end(); neighbor++)
	{
		if ((*neighbor)->mGraphIndex == neighborToRemove->mGraphIndex)
		{
			mNeighbors.erase(neighbor);
			return;
		}
	}
}
void INode::RemoveNeighbor(size_t neighborToRemove)
{
	ASSERT(neighborToRemove >= 0 && neighborToRemove < mNeighbors.size(), "[AI::NavigationNode] index fo removal was outside the range of the vector");

	mNeighbors.erase(mNeighbors.begin() + neighborToRemove);
}

void INode::ResetSearchParameters()
{
	mPathBack = nullptr;
	mGCost = FLT_MAX;
	mPathTravelCost = FLT_MAX;
	mIsClosed = false;
	mIsOpen = false;
}

void INode::SetGCost(float gCost)
{
	mGCost = gCost;
}
void INode::SetGraphIndex(size_t index)
{
	mGraphIndex = index;
}

void INode::SetIsBlocked(bool isBlocked)
{
	mIsBlocked = isBlocked;
}

void INode::SetIsClosed(bool isClosed)
{
	mIsClosed = isClosed;
}
void INode::SetIsOpen(bool isOpen)
{
	mIsOpen = isOpen;
}

void INode::SetPathTravelCost(float travelCost)
{
	mPathTravelCost = travelCost;
}

void INode::SetPathBack(INode* pathBack)
{
	mPathBack = pathBack;
}

