#ifndef ASH_ORDERED_ARRAY_HEADER
#define ASH_ORDERED_ARRAY_HEADER

template <class T>
class OrderedArray
{

public:
	OrderedArray(int size, int _growBy)
	{
		values = new T[size];
		currentMax = size;
		centerIndex = 0;
		currentNum = 0;
		growBy = _growBy;
	}
	OrderedArray()
	{
		int i = 0;
	}
	~OrderedArray()
	{
		delete(values);
	}
	void AddOrdered(T itemToAdd)
	{
		if (currentMax == currentNum)
		{
			DoubleArray();
		}
		values[currentNum] = itemToAdd;
		currentNum++;
		QuickSort(0, currentNum - 1);
		centerIndex = (int)(((currentNum / 2) - 1) + 0.5);
	}
	void RemoveAtIndex(int index)
	{

		for (int i = index; i < currentNum - 1; i++)
		{
			if (i < currentNum - 2)
			{
				values[i] = values[i + 1];

			}
			else
			{
				currentNum--;
			}
		}
		QuickSort(0, currentNum - 1);
	}

	void RemoveFirst(T val)
	{
		int index = FindFirst(val);
		RemoveAtIndex(index);
	}
	void RemoveAll(T val)
	{
		UnorderedArray<int> foundIndicies = FindAll(val);
		for (int i = 0; i < foundIndicies.Num(); i++)
		{
			int index = foundIndicies[i];
			RemoveAtIndex(index);
			for (int X = i + 1; X < foundIndicies.Num(); X++)
			{
				foundIndicies[X] -= 1;
			}
		}
	}

	int BinarySearchRecursive(T val, int l, int r )
	{
		if (r >= l)
		{
			float temp = ((l + (r - 1)) / 2.0f);
			int i = (int) (temp + 0.5);

			if (values[i] == val)
			{
				return i;
			}
			else if (values[i] < val)
			{
				return BinarySearchRecursive(val, i + 1, r);
			}
			else if (values[i] > val)
			{
				return BinarySearchRecursive(val, l, i - 1);
			}
		}

		return -1;
	}

	T GetAtIndex(int index)
	{
		return values[index];
	}

	T& operator[](int index)
	{
		return values[index];
	}

	int Num()
	{
		return currentNum;
	}

	//returns first value found
	int FindFirst(T val)
	{
		bool foundIndex = false;
		int currentIndex = centerIndex;
		return BinarySearchRecursive(val, 0, currentNum - 1);
	}

	OrderedArray<int>& FindAll(T val)
	{
		OrderedArray<int>* indexArray = new UnorderedArray<int>(10, 10);
		for (int i = 0; i < currentNum; i++)
		{
			if (val == values[i])
			{
				indexArray->AddToBack(i);
			}
		}
		return *indexArray;
	}

	OrderedArray<T>& operator =(UnorderedArray<T>& right)
	{
		growBy = right.growBy;
		currentMax = right.currentMax;
		currentNum = right.currentNum;
		values = new T[currentMax];
		for (int i = 0; i < currentNum; i++)
		{
			values[i] = right.values[i];
		}
	}

	T * values;


private:
	void QuickSort(int left, int right)
	{
		if (right - left <= 0)
			return;
		else
		{
			T pivot = values[right];
			int partition = Partition(pivot, left, right);
			QuickSort(left, partition - 1);
			QuickSort(partition, right);
		}

	}
	int Partition(T pivot, int left, int right)
	{
		int leftPtr = left - 1;
		int rightPtr = right + 1;

		while (true)
		{
			while (values[++leftPtr] < pivot)
			{
				//do nothing
			}
			while (values[--rightPtr] > pivot)
			{
				//do nothing
			}
			if (leftPtr >= rightPtr)
				break;
			else
				Swap(leftPtr, rightPtr);
		}
		Swap(leftPtr, rightPtr);
		return leftPtr;
	}
	void Swap(int index1, int index2)
	{
		T temp = values[index1];
		values[index1] = values[index2];
		values[index2] = temp;
	}
	void DoubleArray()
	{
		T* temp = values;
		int oldMax = currentMax;
		currentMax += growBy;
		values = new T[currentMax];
		for (int i = 0; i < oldMax; i++)
		{
			values[i] = temp[i];
		}
	}
	int growBy;
	int currentMax = 0;
	int centerIndex = 0;
	int currentNum = 0;
};
#endif // !Ash_Array