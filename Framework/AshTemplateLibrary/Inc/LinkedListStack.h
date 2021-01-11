#ifndef INCLUDED_LINKEDSTACK_HEADER
#define INCLUDED_LINKEDSTACK_HEADER
#include <iostream>

template<class T>
struct StackNode
{
	T data;
	StackNode* next;
	StackNode* last;
};

template<class T>
class StackLinkedList
{
public:
	StackLinkedList();

	~StackLinkedList();




	void PrintReverse();
	void PrintForward();
	void Push(T item);
	StackNode<T>* Pop();
	StackNode<T>* Peek();
private:
	int32_t size;
	StackNode<T>* listStart;
	StackNode<T>* endOfList;

};

template<class T>
void StackLinkedList<T>::PrintReverse()
{
	bool end = false;
	StackNode<T>* currentNode = endOfList;
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
void StackLinkedList<T>::PrintForward()
{
	bool end = false;
	StackNode<T>* currentNode = listStart;
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
StackLinkedList<T>::StackLinkedList()
{
	size = 0;
}
template<class T>
StackLinkedList<T>::~StackLinkedList()
{

}

template<class T>
void StackLinkedList<T>::Push(T item)
{
	if (size == 0)
	{
		listStart = new StackNode<T>;
		listStart->data = item;
		listStart->last = nullptr;
		listStart->next = nullptr;
		size++;
	}
	else if (size == 1)
	{
		endOfList = listStart;
		listStart = new StackNode<T>();
		listStart->data = item;
		endOfList->last = listStart;
		listStart->next = endOfList;
		size++;
	}
	else
	{
		StackNode<T>* newNode = new StackNode<T>();
		newNode->data = item;
		listStart->last = newNode;
		newNode->next = listStart;
		listStart = newNode;
		size++;
	}
}

template<class T>
StackNode<T>* StackLinkedList<T>::Pop()
{
	if (size > 0)
	{

		StackNode<T>* temp;
		temp = listStart;
		if (listStart->next != nullptr)
		{
			listStart = listStart->next;
			listStart->next = nullptr;
		}
		else
		{
			listStart = nullptr;
		}
		size--;
		return temp;
	}
	return nullptr;
}


template<class T>
StackNode<T>* StackLinkedList<T>::Peek()
{
	if (size > 0)
	{

		StackNode<T>* temp;
		temp = listStart;
		return temp;
	}
	return nullptr;
}
#endif

