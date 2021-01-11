#ifndef INCLUDED_CORE_DELETEUTIL_H
#define INCLUDED_CORE_DELETEUTIL_H

template <class T>
inline void SafeDelete(T*& ptr)
{
	delete ptr;
	ptr = nullptr;
}

template <class T>
inline void SafeDeleteArray(T*& ptr)
{
	delete[] ptr;
	ptr = nullptr;
}

template <class T>
inline void SafeRelease(T*& ptr)
{
	if (ptr)
	{
		ptr->Release();
		ptr = nullptr;
	}
}


#endif // INCLUDED_CORE_DELETEUTIL_H