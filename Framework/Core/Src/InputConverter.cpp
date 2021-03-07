#include "precompiled.h"
#include "InputConverter.h"
#include "Debug.h"
#if defined SINGULARITY_PLATFORM_WINDOWS
#include "WindowsInputMapping.h"
#endif
#include "Common.h"
namespace SingularityEngine::Core
{

	void InputConverter::initialize()
	{
		RegisterInputMappings(mRegisteredInputConversions);
	}

	SingularityInputType InputConverter::convert(int i)
	{
		LOG("[Core::Input] converting input: %04x", i);
		const auto& inputConversion = mRegisteredInputConversions.find(i);
		if (inputConversion != mRegisteredInputConversions.end())
			return inputConversion->second;
		LOG("[Core::Input] Failed to convert specified input: %04x", i);
		return SingularityInputType::SEINPUTMAX;
	}

}