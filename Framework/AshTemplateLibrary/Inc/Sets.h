#ifndef INCLUDED_SETS_HEADER_
#define INCLUDED_SETS_HEADER_
#include <vector>
template<class T>
class DataSet
{
public:
	DataSet() {};
	void Add(T entity)
	{
		items.emplace_back(entity);
	}
	void AddRange(std::vector<T> entitites)
	{
		for (int i = 0; i < entitites.size(); i++)
		{
			Add(entitites);
		}
	}
	void AddNoDupe(T entity)
	{
		bool CopyFound = Contains(entity);
		if (!CopyFound)
			items.emplace_back(entity);
	}
	void AddRangeNoDupe(std::vector<T> entitites)
	{
		for (int i = 0; i < entitites.size(); i++)
		{
			AddNoDupe(entitites[i]);
		}
	}

	bool Contains(T entity)
	{
		for (int i = 0; i < items.size(); i++)
		{
			if (items[i] == entity)
				return true;
		}
		return false;
	}

	DataSet<T> Union(DataSet<T> otherset)
	{
		DataSet<T> unionSet;

		unionSet.AddRangeNoDupe(items);
		unionSet.AddRangeNoDupe(otherset.GetItems());
		return unionSet;
	}
	std::vector<T> GetItems()
	{
		return items;
	}


private:
	std::vector<T> items;

};


#endif