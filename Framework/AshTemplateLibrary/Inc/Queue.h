#ifndef INCLUDED_QUEUE_HEADER_
#define INCLUDED_QUEUE_HEADER_

template<class T>
class Queue
{
public:
	Queue(int startingSize, int growBy);
	~Queue();	
	void Enqueue(T itemToQueue);
	T Dequeue();
	T Peek();
	int GetSize();

	void GrowQueue();
private:
	int head;
	int tail;
	int size;
	int MaxLength;
	T* mStack;
	int growBy;
};
template<class T>
Queue<T>::Queue(int startingSize, int _growBy)
{

	mStack = new T[startingSize];
	MaxLength = startingSize;
	head = 0;
	tail = 0;
	growBy = _growBy;
		size = 0;
}

template<class T>
Queue<T>::~Queue()
{
	delete(mStack);
	mStack = nullptr;
}

template<class T>
int Queue<T>::GetSize()
{
	return size;
}

template<class T>
T Queue<T>::Peek()
{
	return mStack[head];
}

template<class T>
T Queue<T>::Dequeue()
{
	T temp = mStack[head];
	mStack[head] = T();
	if (head < MaxLength - 1)
		head++;
	else
		head = 0;
	size--;
	return temp;
}

template<class T>
void Queue<T>::Enqueue(T itemToQueue)
{
	if (size + 1 >= MaxLength)
	{
		GrowQueue();
	}
	if (size == 0)
	{
		tail = tail;
	}
	else
	{

		if (tail + 1 > MaxLength - 1)
		{
			tail = 0;
		}
		else
		{
			tail++;
		}
	}
	if (size == 0)
	{

		mStack[tail] = itemToQueue;
		size ++;

	}
	else
	{

		mStack[tail] = itemToQueue;
		size++;
	}

}


template<class T>
void Queue<T>::GrowQueue()
{
	T* temp = mStack;
	mStack = new T[MaxLength + growBy];
	int y = 0;
	if (tail < head)
	{

		for (int i = head; i != tail; i < MaxLength -1 ? i++ : i = 0 )
		{
			mStack[y] = temp[i];
			y++;
		
		}
		mStack[y] = temp[tail];
	}
	else
	{
		for (int i = head; i <= tail; i++)
		{
			mStack[y] = temp[i];
			y++;

		}
	}
	head = 0;
	tail = y-1;
	MaxLength = MaxLength + growBy;
	size = tail + 1;
}

#endif // INCLUDEDED_QUE_HEADER_

