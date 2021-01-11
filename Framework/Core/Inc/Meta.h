#pragma once

#include "MetaType.h"
#include "MetaUtil.h"
#include "MetaClass.h"
// stop compiler from generating stuff
#define META_TYPE_DECLARE(Type)\
template <> const SingularityEngine::Core::MetaType* SingularityEngine::Core::GetMetaType<Type>();

#define META_TYPE_DEFINE(Type, Name)\
	template <> const SingularityEngine::Core::MetaType* SingularityEngine::Core::GetMetaType<Type>()\
	{\
		static SingularityEngine::Core::MetaType sMetaType(\
			Name,\
			SingularityEngine::Core::MetaType::Category::Primitive,\
			sizeof(Type),\
			[] { return new Type; },\
			[](void* data) { delete static_cast<Type*>(data);}\
			);\
		return &sMetaType;\
	}

#define META_TYPE_CLASS_DEFINE(Type, Name)\
template <> const SingularityEngine::Core::MetaType* SingularityEngine::Core::GetMetaType<Type>()\
{\
	static SingularityEngine::Core::MetaType sMetaType(\
		Name,\
		SingularityEngine::Core::MetaType::Category::Class,\
		sizeof(Type),\
		[] { return new Type; },\
		[](void* data) { delete static_cast<Type*>(data);}\
		);\
	return &sMetaType;\
}

#define META_CLASS_DECLARE\
	static const SingularityEngine::Core::MetaClass* StaticGetMetaClass();\
	virtual const SingularityEngine::Core::MetaClass* GetMetaClass() const { return StaticGetMetaClass(); }\


#define META_CLASS_BEGIN_INTERNAL(ClassType)\
template<> const SingularityEngine::Core::MetaClass* SingularityEngine::Core::GetMetaClass<ClassType>()\
{\
	return ClassType::StaticGetMetaClass();\
}\
const SingularityEngine::Core::MetaClass* ClassType::StaticGetMetaClass()\
{\
	using LocalType = ClassType; \
	const char* const className = #ClassType;


#define META_CLASS_BEGIN(ClassType)\
		META_CLASS_BEGIN_INTERNAL(ClassType)\
		const SingularityEngine::Core::MetaClass* parentMetaClass = nullptr;

#define META_DERIVED_CLASS_BEGIN(ClassType, ParentType)\
		META_CLASS_BEGIN_INTERNAL\
		const SingularityEngine::Core::MetaClass* parentMetaClass = nullptr;

#define META_FIELD_BEGIN \
static const SingularityEngine::Core::MetaField fields[] { 


#define META_FIELD(Var,Name)\
		SingularityEngine::Core::MetaField(Name, SingularityEngine::Core::GetMetaType(&LocalType::Var), SingularityEngine::Core::GetFieldOffset(&LocalType::Var)),

#define META_FIELD_END \
};\
static const size_t numFields = std::size(fields);

#define META_NO_FIELD\
	static const SingularityEngine::Core::MetaField* fields = nullptr;\
	static const size_t numFields = 0;

#define META_CLASS_END\
	static const SingularityEngine::Core::MetaClass sMetaClass(className, sizeof(LocalType), parentMetaClass, fields, numFields\
	, [] { return new LocalType();}\
	, [](void* data) { delete static_cast<LocalType*>(data);});\
	return &sMetaClass;\
	}

//META_TYPE_DECLARE(int)
//META_TYPE_DECLARE(float)
//META_TYPE_DECLARE(bool)
//META_TYPE_DECLARE(std::string)
