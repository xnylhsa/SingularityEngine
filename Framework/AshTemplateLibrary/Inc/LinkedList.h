#ifndef INCLUDED_LINKEDLIST_HEADER
#define INCLUDED_LINKEDLIST_HEADER

template<class T>
struct Node
{
	T data;
	Node* next;
};

template<class T>
class LinkedList
{
public:
	LinkedList();
	LinkedList(T startData, T endData);

	~LinkedList();

	void AddToFront(T item);
	void AddToBack(T item);
	LinkedList<T> Reverse();
	bool RemoveFromList(T item);

	Node<T>* GetNode(T item);

	Node<T>* listStart;
	Node<T>* endOfList;

};

template<class T>
LinkedList<T>::LinkedList(T startData, T endData)
{
	listStart = new Node<T>();
	endOfList = new Node<T>();
	listStart->next = endOfList;
	listStart->data = startData;
	endOfList->data = endData;
	endOfList->next = nullptr;
}
template<class T>
LinkedList<T>::~LinkedList()
{

}
template<class T>
LinkedList<T>::LinkedList()
{
	listStart = new Node<T>();
	endOfList = new Node<T>();
	listStart->next = endOfList;
	endOfList->next = nullptr;
}


template<class T>
Node<T>* LinkedList<T>::GetNode(T item)
{
	bool foundItem = false;
	Node<T>* lastNode;
	Node<T>* currentNode;
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
bool LinkedList<T>::RemoveFromList(T item)
{
	bool foundItem = false;
	Node<T>* lastNode;
	Node<T>* currentNode;
	lastNode = listStart;
	currentNode = listStart;
	if (currentNode->data == item)
	{
		if (currentNode->next != nullptr)
		{
			Node<T>* temp = currentNode->next;
			delete(currentNode);
			currentNode = nullptr;
			listStart = temp;
			if (listStart == endOfList)
			{
				temp = new Node<T>();
				temp->data = endOfList->data;
				endOfList = temp;
				listStart->next = endOfList;
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
void LinkedList<T>::AddToBack(T item)
{
	Node<T>* newNode = new Node<T>();
	newNode->data = item;
	endOfList->next = newNode;
	endOfList = newNode;
}

template<class T>
void LinkedList<T>::AddToFront(T item)
{
	Node<T>* newNode = new Node<T>();
	newNode->data = item;
	newNode->next = listStart;
	listStart = newNode;
}

template<class T> LinkedList<T> LinkedList<T>::Reverse()
{	
	LinkedList<T> reverse(listStart->next->data, listStart->data);
	//reversed.endOfList->data = listStart->data;
	//reversed.listStart->data = listStart->next->data;
	//reversed.listStart->next = reversed.endOfList;

	Node<T>* currentNode = listStart->next;

	while (currentNode->next != nullptr)
	{
		currentNode = currentNode->next;
		reverse.AddToFront(currentNode->data);
	}
	return reverse;

}
#endif

