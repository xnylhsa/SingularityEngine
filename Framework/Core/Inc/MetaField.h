#pragma once


namespace SingularityEngine::Core {

class MetaType;

class MetaField
{
public:
	MetaField(const char* const name, const MetaType* const type, size_t offset);

	const char* GetName() const { return mName; }
	const MetaType* GetMetaType() const { return mType; }
	size_t GetOffset() const { return mOffset; }

private:
	const char* const mName{ nullptr };
	const MetaType* const mType{ nullptr };
	size_t mOffset{ 0 };

};

}