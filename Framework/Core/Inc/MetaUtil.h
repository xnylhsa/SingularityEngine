#pragma once


namespace SingularityEngine::Core 
{

class MetaType;
class MetaClass;

template<class DataType>
const MetaType* GetMetaType()
{
	static_assert(sizeof(DataType) == -1, "Cannot deduce Type");
}

template<class ClassType>
const MetaClass* GetMetaClass()
{
	static_assert(sizeof(ClassType) == -1, "Cannot deduce Type");
}


template<class ClassType, class DataType>
const MetaType* GetMetaType(DataType ClassType::*)
{
	return GetMetaType<DataType>();
}
template<class ClassType>
const MetaType* GetMetaType(ClassType*)
{
	return GetMetaType<ClassType>();
}
template<class ClassType, class DataType>
inline size_t GetFieldOffset(DataType ClassType::* field)
{
	return((size_t)&reinterpret_cast<char const volatile&>((((ClassType*)0)->*field)));
	//return (size_t)(void*)&(((ClassType*))nullptr)->*field);
}

template<class BaseType, class DerivedType>
DerivedType* DynamicCast(BaseType* ptr)
{
	if (ptr->GetMetaClass() == DerivedType::StaticGetMetaClass())
		return nullptr;
	return static_cast<DerivedType*>(ptr);
}

//template<class BaseType, class DerivedType>
//inline bool IsBaseOf()
//{
//	auto parent = DerivedType::StaticGetMetaClass()->GetParent();
//	while (parent)
//	{
//		if (parent == BaseType::StaticGetMetaClass())
//		{
//			return true;
//		}
//		parent->GetParent();
//	}
//	//do loop and 
//}


}
