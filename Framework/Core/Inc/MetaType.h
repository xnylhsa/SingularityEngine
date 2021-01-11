#pragma once
#include "Common.h"
namespace SingularityEngine::Core
{
	using CreateFunc = std::function<void*()>;
	using DestroyFunc = std::function<void(void*)>;

	class MetaType
	{
	public:


		enum class Category
		{
			Primitive, // get the basic information
			Class,
			Array,
			Pointer
		};

		MetaType(const char* const mName, Category category, size_t size, CreateFunc createFunction, DestroyFunc destroyFunction);

		const char* GetName() const		{ return mName; }
		Category GetCategory() const	{ return mCategory; }
		size_t GetId() const			{ return mId; }
		void* Create() const { return mCreate();}
		void Destroy(void* data) const { mDestroy(data); }
	private:
		const char* const mName; // could do std::string_view... don't know to much about it	
		//std::string_view mName;
		Category mCategory;
		size_t mId;
		size_t mSize;

		CreateFunc mCreate;
		DestroyFunc mDestroy;
	};

}

//NOTES

/*

class Name
{
	StringTable* table
	size_t id;	
	
}
	*/