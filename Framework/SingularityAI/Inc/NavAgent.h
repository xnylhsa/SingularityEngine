#ifndef SINGULARITY_AI_NAV_AGENT__
#define SINGULARITY_AI_NAV_AGENT__
#include "Common.h"
#include "AIEntity.h"
namespace SingularityEngine::AI
{

	class NavAgent : public AI::AIEntity
	{
	public:
		NavAgent(std::string name) : AIEntity(name)
		virtual ~NavAgent() {}
	private:

	};
}


#endif // BASICE_FINITE_STATE_MACHINE_
