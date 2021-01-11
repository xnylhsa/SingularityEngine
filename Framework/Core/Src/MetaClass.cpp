#include "Precompiled.h"
#include "MetaClass.h"
#include "MetaField.h"
#include "MetaRegistry.h"
#include "Debug.h"
using namespace SingularityEngine::Core;

MetaClass::MetaClass(const char* const name, size_t size,
	const MetaClass* const parent,
	const MetaField* const fields, size_t numFields, CreateFunc create, DestroyFunc destroy)
	: MetaType(name, MetaType::Category::Class, size, std::move(create), std::move(destroy))
	, mParent(parent)
	, mFields(fields)
	, mNumFields(numFields)
{
	MetaRegistry::Register(this);
}

const MetaClass* MetaClass::GetParent() const
{
	return mParent;
}
const MetaField* MetaClass::FindField(const char* name) const
{
	const MetaField* field = nullptr;
	for (size_t i = 0; i < mNumFields; i++)
	{
		const MetaField& f = mFields[i];
		if (strcmp(f.GetName(), name) == 0)
		{
			field = &f;
			break;
		}
	}

	// If the field is not found, try the parent
	if (field == nullptr && mParent != nullptr)
	{
		field = mParent->FindField(name);
	}
	return field;
}


const void MetaClass::Dump() const
{
	LOG("Class [%s]", GetName());
	for (size_t i = 0; i < mNumFields; ++i)
	{
		auto& field = mFields[i];
		LOG("has field [%s] of type [%s] at %zu ", field.GetName(), field.GetMetaType()->GetName(), field.GetOffset());
	}
	if(mParent)
	{
		LOG("Is derived from [%s]", mParent->GetName());
	}
}



/*
What we are about to build will this stuff for us
class Car
{
public:
	virtual MetaClass* GetMetaCass() {return GetMetaClass}
private:
	int position;
	float fuel;

}


*/

// MetaField fields[] = 
//{                       
//	MetaField("position", GetMetaType<int>(), 0)
//	MetaField("fuel", GetMetaType<float>(), sizeof(int))
//}
// MetaClass carMetaClass("Car", sizeof(Car), nullptr, fields, sizeof(fields))
// nullptr if you don't have a parent           ^^^^

/*
void SetPosition(void* Instance, MetaClass* metaClass, int pos)
{
	const MetaField* field = metaClass->FindField("position");
	if(field)
	{
	*(int*)(instance + field->GetOffset()) =  pos; // walking memory
	}
}

MetaSystem can be dangerous if not used properly

*/