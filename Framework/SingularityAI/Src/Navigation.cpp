#include "Precompiled.h"
#include "Navigation.h"
#include "NavigationTypes.h"
#include "NavigationContext.h"
#include "NavigationNode.h"
#include "NavigationGraph.h"
using namespace SingularityEngine;
using namespace AI;
using namespace Navigation;

void SingularityEngine::AI::Navigation::RunBFS(Graph* graph, size_t startIndex, size_t endIndex, float allowedRunDuration, SearchContext& currentPath)
{

	INode* startingNode = graph->GetNode(startIndex);
	startingNode->SetIsClosed(true);
	INode* endingNode = graph->GetNode(endIndex);
	if (currentPath.mSearchState == State::SEARCH_NOT_STARTED)
	{
		currentPath.mSearchState = State::SEARCH_INCOMPLETE;
		graph->ResetGraphForNewSearch();
		startingNode->SetIsOpen(true);
		startingNode->SetPathBack(nullptr);
		currentPath.mOpenList.push_back(startingNode);
	}
	SingularityEngine::Core::Timer functionTimer;
	functionTimer.Initialize();
	while (currentPath.mOpenList.size() > 0 )
	{
		if (functionTimer.GetTotalTime() >= allowedRunDuration)
		{
			break;
		}
		INode* currentNode = currentPath.mOpenList.front();
		currentPath.mOpenList.pop_front();
		if (currentNode == endingNode)
		{
			currentPath.mSearchState = State::PATH_FOUND;
			currentPath.mClosedList.push_back(currentNode);
			while (currentPath.mOpenList.size() > 0)
			{
				currentPath.mOpenList.pop_front();

			}
			currentNode = endingNode;
			while (currentNode != nullptr)
			{
				currentPath.mTotalPath.push_back(currentNode);
				currentNode = currentNode->GetPathBack();

			}
			return;
		}
		for (auto it = currentNode->GetNeighbors().begin(); it != currentNode->GetNeighbors().end(); it++)
		{
			bool isBlocked = (*it)->IsBlocked();
			bool isClosed = (*it)->IsClosed();
			bool isOpen = (*it)->IsOpen();

			if (!isBlocked && !isClosed && !isOpen )
			{
				(*it)->SetPathBack(currentNode);
				(*it)->SetIsOpen(true);
				currentPath.mOpenList.push_back(*it);
			}
		}
		currentNode->SetIsClosed(true);
		currentPath.mClosedList.push_back(currentNode);
		functionTimer.Update();

	}
	if (currentPath.mSearchState != State::PATH_FOUND)
	{
		if (functionTimer.GetTotalTime() >= allowedRunDuration)
		{
			currentPath.mSearchState = SEARCH_INCOMPLETE;
		}
		else
		{
			currentPath.mSearchState = State::PATH_NOT_FOUND;
		}
	}
}
void SingularityEngine::AI::Navigation::RunDFS( Graph* graph, size_t startIndex, size_t endIndex, float allowedRunDuration, SearchContext& currentPath)
{
	INode* startingNode = graph->GetNode(startIndex);

	INode* endingNode = graph->GetNode(endIndex);
	if (currentPath.mSearchState == SEARCH_NOT_STARTED)
	{
		currentPath.mSearchState = SEARCH_INCOMPLETE;
		graph->ResetGraphForNewSearch();

		currentPath.mOpenList.push_back(startingNode);
	}
	SingularityEngine::Core::Timer functionTimer;
	functionTimer.Initialize();
	while (currentPath.mOpenList.size() > 0)
	{
		if (functionTimer.GetTotalTime() >= allowedRunDuration)
		{
			break;
		}
		INode* currentNode = currentPath.mOpenList.front();
		currentPath.mOpenList.pop_front();
		if (currentNode == endingNode)
		{
			currentPath.mClosedList.push_back(currentNode);
			while(currentPath.mOpenList.size() > 0)
			{
				currentPath.mOpenList.pop_front();

			}
			currentNode = endingNode;
			while (currentNode != nullptr)
			{
				currentPath.mTotalPath.push_back(currentNode);
				currentNode = currentNode->GetPathBack();

			}
			currentPath.mSearchState = PATH_FOUND;

			break;
		}
		for (auto it = currentNode->GetNeighbors().begin(); it != currentNode->GetNeighbors().end(); it++)
		{
			bool isOpen = (*it)->IsOpen();

			bool isBlocked = (*it)->IsBlocked();
			bool isClosed = (*it)->IsClosed();
			if (!isClosed && !isBlocked && !isOpen)
			{
				(*it)->SetPathBack(currentNode);
				(*it)->SetIsOpen(true);
				currentPath.mOpenList.push_front(*it);
			}
		}
		currentPath.mClosedList.push_back(currentNode);
		currentNode->SetIsClosed(true);

		functionTimer.Update();


	}
	if (currentPath.mSearchState != State::PATH_FOUND)
	{
		if (functionTimer.GetTotalTime() >= allowedRunDuration)
		{
			currentPath.mSearchState = SEARCH_INCOMPLETE;
		}
		else
		{
			currentPath.mSearchState = State::PATH_NOT_FOUND;
		}
	}
}


void SingularityEngine::AI::Navigation::RunAStar( Graph* graph, size_t startIndex, size_t endIndex, IHeuristicFunctor* heuristicFunction, float allowedRunDuration, SearchContext& currentPath)
{
	INode* startingNode = graph->GetNode(startIndex);
	INode* endingNode = graph->GetNode(endIndex);
	if (currentPath.mSearchState == SEARCH_NOT_STARTED)
	{
		currentPath.mSearchState = SEARCH_INCOMPLETE;
		startingNode->SetPathBack(nullptr);
		graph->ResetGraphForNewSearch();
		currentPath.mOpenList.push_back(startingNode);
		startingNode->SetIsOpen(true);
		startingNode->SetGCost(0.0f);
		startingNode->SetPathTravelCost(0.0f);
	}
	SingularityEngine::Core::Timer functionTimer;
	functionTimer.Initialize();
	while (currentPath.mOpenList.size() > 0)
	{
		if (functionTimer.GetTotalTime() >= allowedRunDuration)
		{
			break;
		}

		currentPath.mOpenList.sort([](INode* a, INode* b) {
			return *a < *b; });
		while (currentPath.mOpenList.size() > 20)
		{
			currentPath.mOpenList.pop_back();
		}

		auto currentNode = currentPath.mOpenList.front();
		currentPath.mOpenList.pop_front();
		currentPath.mClosedList.push_back(currentNode);

		if (currentNode == endingNode)
		{
			INode* pathNode = endingNode;

			while (pathNode != nullptr)
			{
				currentPath.mTotalPath.push_back(pathNode);
				pathNode = pathNode->GetPathBack();
			}
			currentPath.mSearchState = PATH_FOUND;
			return;
		}
		float currentTotalCost = currentNode->GetGCost();
		for (auto it = currentNode->GetNeighbors().begin(); it != currentNode->GetNeighbors().end(); it++)
		{
			float heuristicCost = (*heuristicFunction)((*it), endingNode);
			float gCost = currentTotalCost + currentNode->CalculateTravelCost((*it));
			float TravelCost = gCost + heuristicCost;
			if (!(*it)->IsClosed())
			{
				if (gCost < (*it)->GetGCost())
				{
					(*it)->SetGCost(gCost);
					(*it)->SetPathTravelCost(TravelCost);
					(*it)->SetPathBack(currentNode);
				}
				if (!(*it)->IsOpen())
				{
					(*it)->SetIsOpen(true);
					currentPath.mOpenList.push_back((*it));
				}
			}

		}
		//std::sort(currentPath.mOpenList.begin(), currentPath.mOpenList.end(), std::greater<INode>());
		currentNode->SetIsClosed(true);
		functionTimer.Update();
	}
	if (currentPath.mSearchState != State::PATH_FOUND)
	{
		if (functionTimer.GetTotalTime() >= allowedRunDuration)
		{
			currentPath.mSearchState = State::SEARCH_INCOMPLETE;
		}
		else
		{
			currentPath.mSearchState = State::PATH_NOT_FOUND;
		}
	}
}

void SingularityEngine::AI::Navigation::RunDjikstra( Graph* graph, size_t startIndex, size_t endIndex, float allowedRunDuration, SearchContext& currentPath)
{
	INode* startingNode = graph->GetNode(startIndex);
	INode* endingNode = graph->GetNode(endIndex);
	if (currentPath.mSearchState == SEARCH_NOT_STARTED)
	{
		currentPath.mSearchState = SEARCH_INCOMPLETE;
		graph->ResetGraphForNewSearch();
		startingNode->SetPathBack(nullptr);
		startingNode->SetIsOpen(true);
		startingNode->SetGCost(0.0f);
		startingNode->SetPathTravelCost(0.0f);

		currentPath.mOpenList.push_back(startingNode);
	}
	SingularityEngine::Core::Timer functionTimer;
	functionTimer.Initialize();
	while (currentPath.mOpenList.size() > 0)
	{
		if (functionTimer.GetTotalTime() >= allowedRunDuration)
		{
			break;
		}
		currentPath.mOpenList.sort([](INode* a, INode* b) {
			return *a < *b; });

		auto currentNode = currentPath.mOpenList.front();
		currentPath.mOpenList.pop_front();
		currentPath.mClosedList.push_back(currentNode);

		if (currentNode == endingNode)
		{
			INode* pathNode = endingNode;
			while (pathNode != nullptr)
			{
				currentPath.mTotalPath.push_back(pathNode);
				pathNode = pathNode->GetPathBack();
			}
			currentPath.mSearchState = PATH_FOUND;
			return;
		}
		float currentTotalCost = currentNode->GetGCost();
		for (auto it = currentNode->GetNeighbors().begin(); it != currentNode->GetNeighbors().end(); it++)
		{
			float gCost = currentTotalCost + currentNode->CalculateTravelCost((*it));
			if (!(*it)->IsClosed())
			{
				if (gCost < (*it)->GetGCost())
				{
					(*it)->SetGCost(gCost);
					(*it)->SetPathTravelCost(gCost);
					(*it)->SetPathBack(currentNode);
				}
				if (!(*it)->IsOpen())
				{
					(*it)->SetIsOpen(true);
					currentPath.mOpenList.push_back((*it));
				}
			}

		}
		currentNode->SetIsClosed(true);

		functionTimer.Update();
	}
	if (currentPath.mSearchState != State::PATH_FOUND)
	{
		if (functionTimer.GetTotalTime() >= allowedRunDuration)
		{
			currentPath.mSearchState = State::SEARCH_INCOMPLETE;
		}
		else
		{
			currentPath.mSearchState = State::PATH_NOT_FOUND;
		}
	}
}

