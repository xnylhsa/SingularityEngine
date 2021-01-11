#ifndef INCLUDED_CORE_META_REGISTRY_HEADER_
#define  INCLUDED_CORE_META_REGISTRY_HEADER_

namespace SingularityEngine::Core
{
	class MetaClass;
	namespace MetaRegistry
	{
		void Register(const MetaClass* metaClass);
		const MetaClass* FindMetaClass(const char* className);
	}
}



#endif // 
