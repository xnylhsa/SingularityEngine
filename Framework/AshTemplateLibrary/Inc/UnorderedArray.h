#ifndef ASH_ARRAY_HEADER
#define ASH_ARRAY_HEADER

template <class T>
class UnorderedArray
{

public:
	UnorderedArray(int size, int _growBy)
	{
		values = new T[size];
		currentMax = size;
		currentNum = 0;
		growBy = _growBy;
	}
	UnorderedArray()
	{
		int i = 0;
	}
	~UnorderedArray()
	{
		delete(values);
	}
	void AddToBack(T itemToAdd)
	{
		if (currentMax == currentNum)
		{
			DoubleArray();
		}
		values[currentNum] = itemToAdd;
		currentNum++;
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
		for (int i = 0; i < currentNum; i++)
		{
			if (val == values[i])
			{
				return i;
			}
		}
		return -1;
	}

	UnorderedArray<int>& FindAll(T val)
	{
		UnorderedArray<int>* indexArray = new UnorderedArray<int>(10, 10);
		for (int i = 0; i < currentNum; i++)
		{
			if (val == values[i])
			{
				indexArray->AddToBack(i);
			}
		}
		return *indexArray;
	}

	UnorderedArray<T>& operator =(UnorderedArray<T>& right)
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
	int currentNum = 0;
};
#endif // !Ash_Array
