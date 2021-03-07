#ifndef SINGULARITY_INPUT_CONVERTER
#define SINGULARITY_INPUT_CONVERTER
#include "Common.h"
#include "SEInputTypes.h"
namespace SingularityEngine::Core
{
	class InputConverter
	{
	public:
		InputConverter() = default;
		~InputConverter() = default;
		void initialize();
		SingularityInputType convert(int i);
	protected:
		
		std::unordered_map<int, SingularityInputType> mRegisteredInputConversions;
	};
}

#endif