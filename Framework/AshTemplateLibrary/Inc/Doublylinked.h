#ifndef INCLUDED_DOUBLYLINKEDLIST_HEADER
#define INCLUDED_DOUBLYLINKEDLIST_HEADER
#include <iostream>

template<class T>
struct DoubleNode
{
	T data;
	DoubleNode* next;
	DoubleNode* last;
};

template<class T>
class DoublyLinked
{
public:
	DoublyLinked(T startData, T endData);

	~DoublyLinked();

	void AddToFront(T item);
	void AddToBack(T item);

	bool RemoveFromList(T item);

	void PrintReverse();
	void PrintForward();
	DoubleNode<T>* GetNode(T item);

	DoubleNode<T>* listStart;
	DoubleNode<T>* endOfList;

};

template<class T>
void DoublyLinked<T>::PrintReverse()
{
	bool end = false;
	DoubleNode<T>* currentNode = endOfList;
	while (!end)
	{
		std::cout << currentNode->data << ", ";
		if (currentNode->last == nullptr)
		{
			end = true;
		}
		else
		{
			currentNode = currentNode->last;
		}
	}
	std::cout << std::endl;
}
template<class T>
void DoublyLinked<T>::PrintForward()
{
	bool end = false;
	DoubleNode<T>* currentNode = listStart;
	while (!end)
	{
		std::cout << currentNode->data << ", ";
		if (currentNode->next == nullptr)
		{
			end = true;
		}
		else
		{
			currentNode = currentNode->next;
		}
	}
	std::cout << std::endl;

}


template<class T>
DoublyLinked<T>::DoublyLinked(T startData, T endData)
{
	listStart = new DoubleNode<T>();
	endOfList = new DoubleNode<T>();
	listStart->next = endOfList;
	listStart->last = nullptr;
	listStart->data = startData;
	endOfList->data = endData;
	endOfList->last = listStart;
	endOfList->next = nullptr;
}
template<class T>
DoublyLinked<T>::~DoublyLinked()
{

}



template<class T>
DoubleNode<T>* DoublyLinked<T>::GetNode(T item)
{
	bool foundItem = false;
	DoubleNode<T>* lastNode;
	DoubleNode<T>* currentNode;
	lastNode = listStart;
	currentNode = listStart;
	if (currentNode->data == item)
	{
		return currentNode;

	}
	if (currentNode->next == nullptr)
	{
		return nullptr;
	}
	currentNode = currentNode->next;
	while (!foundItem)
	{
		if (currentNode->data == item)
		{
			return currentNode;
		}
		else
		{
			lastNode = currentNode;
		}


		if ((currentNode->next != nullptr))
		{
			currentNode = currentNode->next;

		}
		else
		{
			return nullptr;
		}
	}
	return nullptr;
}

template<class T>
bool DoublyLinked<T>::RemoveFromList(T item)
{
	bool foundItem = false;
	DoubleNode<T>* lastNode;
	DoubleNode<T>* currentNode;
	lastNode = listStart;
	currentNode = listStart;
	if (currentNode->data == item)
	{
		if (currentNode->next != nullptr)
		{
			DoubleNode<T>* temp = currentNode->next;
			delete(currentNode);
			currentNode = nullptr;
			listStart = temp;
			listStart->last = nullptr;
			if (listStart == endOfList)
			{
				temp = new DoubleNode<T>();
				temp->data = endOfList->data;
				endOfList = temp;
				endOfList->next = nullptr;
				listStart->next = endOfList;
				endOfList->last = listStart;
				
			}
			return true;
		}

	}
	if (currentNode->next == nullptr)
	{
		return false;
	}
	currentNode = currentNode->next;
	while (!foundItem)
	{
		if (currentNode->data == item)
		{
			foundItem = true;
			if (currentNode->next == nullptr)
			{
				lastNode->next = nullptr;
				endOfList = lastNode;
			}
			else
			{
				currentNode->next->last = lastNode;
				lastNode->next = currentNode->next;
			}
			delete currentNode;
			currentNode = nullptr;
			return true;
		}
		else
		{
			lastNode = currentNode;
		}


		if ((currentNode->next != nullptr))
		{
			currentNode = currentNode->next;

		}
		else
		{
			return false;
		}
	}

}

template<class T>
void DoublyLinked<T>::AddToBack(T item)
{
	DoubleNode<T>* newNode = new DoubleNode<T>();
	newNode->data = item;
	endOfList->next = newNode;
	newNode->last = endOfList;
	endOfList = newNode;
}

template<class T>
void DoublyLinked<T>::AddToFront(T item)
{
	DoubleNode<T>* newNode = new DoubleNode<T>();
	newNode->data = item;
	newNode->next = listStart;
	listStart->last = newNode;
	newNode->last = nullptr;
	listStart = newNode;
}

#endif

