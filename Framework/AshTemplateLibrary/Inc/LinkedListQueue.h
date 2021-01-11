#ifndef INCLUDED_LINKEDQUEUE_HEADER
#define INCLUDED_LINKEDQUEUE_HEADER
#include <iostream>

template<class T>
struct QueueNode
{
	T data;
	QueueNode* next;
	QueueNode* last;
};

template<class T>
class QueueLinkedList
{
public:
	QueueLinkedList();

	~QueueLinkedList();

	


	void PrintReverse();
	void PrintForward();
	void Enqueue(T item);
	QueueNode<T>* Dequeue();
	QueueNode<T>* Peek();
private:
	int32_t size;
	QueueNode<T>* listStart;
	QueueNode<T>* endOfList;

};

template<class T>
void QueueLinkedList<T>::PrintReverse()
{
	bool end = false;
	QueueNode<T>* currentNode = endOfList;
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
void QueueLinkedList<T>::PrintForward()
{
	bool end = false;
	QueueNode<T>* currentNode = listStart;
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
QueueLinkedList<T>::QueueLinkedList()
{
	size = 0;
}
template<class T>
QueueLinkedList<T>::~QueueLinkedList()
{

}

template<class T>
void QueueLinkedList<T>::Enqueue(T item)
{
	if (size == 0)
	{
		listStart = new QueueNode<T>;
		listStart->data = item;
		listStart->last = nullptr;
		listStart->next = nullptr;
		size++;
	}
	else if (size == 1)
	{
		endOfList = new QueueNode<T>();
		endOfList->data = item;
		endOfList->last = listStart;
		listStart->next = endOfList;
		size++;
	}
	else
	{
		QueueNode<T>* newNode = new QueueNode<T>();
		newNode->data = item;
		endOfList->next = newNode;
		newNode->last = endOfList;
		endOfList = newNode;
		size++;
	}
}

template<class T>
QueueNode<T>* QueueLinkedList<T>::Dequeue()
{
	if (size > 0)
	{

		QueueNode<T>* temp;
		temp = listStart;
		if (listStart->next != nullptr)
		{
			listStart = listStart->next;
			listStart->last = nullptr;
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
QueueNode<T>* QueueLinkedList<T>::Peek()
{
	if (size > 0)
	{

		QueueNode<T>* temp;
		temp = listStart;
		return temp;
	}
	return nullptr;
}
#endif

