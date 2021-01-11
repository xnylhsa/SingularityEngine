#ifndef SINGULARITY_ECS
#define SINGULARITY_ECS
#include "Common.h"


namespace SingularityEngine::ECS
{
	using Entity = uint32_t;

	const Entity MAX_ENTITIES = 5000;

	using ComponentType = uint16_t;
	const ComponentType MAX_COMPONENTS = 256;

	using Signature = std::bitset<MAX_COMPONENTS>;

}

#endif // SINGULARITY_ECS
