#ifndef CORE_META_REGISTRATION_
#define CORE_META_REGISTRATION_
#include "Meta.h"



META_TYPE_DECLARE(int);
META_TYPE_DECLARE(float);
META_TYPE_DECLARE(bool);
META_TYPE_DECLARE(std::string);

namespace SingularityEngine::Core
{
	void MetaRegistration();
}


#endif // 
