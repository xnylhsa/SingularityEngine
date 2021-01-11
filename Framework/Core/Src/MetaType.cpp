#include "Precompiled.h"
#include "MetaType.h"

using namespace SingularityEngine::Core;

MetaType::MetaType(const char* const name, Category category, size_t size, CreateFunc createFunction, DestroyFunc destroyFunction)
	: mName(name)
	, mCategory(category)
	, mId(std::hash<std::string>()(name))
	, mSize(size)
	, mCreate(std::move(createFunction))
	, mDestroy(std::move(destroyFunction))
{

}

//MetaType sIntType("int", MetaType::Category::Primitive, sizeof(int));