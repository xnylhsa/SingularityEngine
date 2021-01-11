#pragma once
template <class T>
class Stack
{
public:
	Stack(int startingSize, int _growBy);
	~Stack();
	void Push(T item);
	T Pop();
	T Peek();
	int GetSize();

	void GrowStack();
private:
	int head;
	int size;
	int MaxLength;
	T* mStack;
	int growBy;
};

template<class T>
Stack<T>::Stack(int startingSize, int _growBy)
{

	mStack = new T[startingSize];
	MaxLength = startingSize;
	head = 0;
	growBy = _growBy;
	size = 0;
}

template<class T>
Stack<T>::~Stack()
{
	delete(mStack);
	mStack = nullptr;
}

template<class T>
int Stack<T>::GetSize()
{
	return size;
}

template<class T>
T Stack<T>::Peek()
{
	return mStack[head];
}

template<class T>
T Stack<T>::Pop()
{
	T temp = mStack[head];
	mStack[head] = T();
	head--;
	size--;
	return temp;
}

template<class T>
void Stack<T>::Push(T item)
{
	if (size + 1 >= MaxLength)
	{
		GrowStack();
	}
	if (size == 0)
	{
		head = head;
	}
	else
	{
		head++;
	}
	mStack[head] = item;
	size++;

}


template<class T>
void Stack<T>::GrowStack()
{
	T* temp = mStack;
	mStack = new T[MaxLength + growBy];
	int y = 0;
	for (int i = 0; i <= head; i++)
	{
		mStack[y] = temp[i];
		y++;

	}
	MaxLength = MaxLength + growBy;
	size = head + 1;
}