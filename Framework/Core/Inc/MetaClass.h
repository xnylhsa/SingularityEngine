#pragma once
#include "MetaType.h"

namespace SingularityEngine::Core
{

	class MetaField;

	class MetaClass : public MetaType
	{
	public:
		MetaClass(const char* const name, size_t size,
			const MetaClass* const parent,
			const MetaField* const fields, size_t numFields, CreateFunc create, DestroyFunc destroy);

		const MetaClass* GetParent() const;
		const MetaField* FindField(const char* name) const;
		const void Dump() const;

	private:
		const MetaClass* const mParent{nullptr};
		const MetaField* const mFields{nullptr};
		const size_t mNumFields{0};
	};

}