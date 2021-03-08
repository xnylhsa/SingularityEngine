#ifndef SINGULARITY_INPUT_MANAGER_GLFW
#define SINGULARITY_INPUT_MANAGER_GLFW
#include "Common.h"
#include "InputManager.h"
struct GLFWcursor;
namespace SingularityEngine::Core
{

	class InputManagerGLFW : public InputManager
	{
	public:
	protected:

		void registerKeyMappings() override;


		void registerEventBindings() override;


		void setLockedMousePlatformImpl(bool locked) override;


		void setShowSystemCursorPlatformImpl(bool showCursor) override;


		void setMouseClipToWindowPlatformImpl(bool clip) override;


		void setRawInputPlatformImpl(bool useRawInput) override;


		bool onInitalize() override;


		bool onTeardown() override;


		bool handleKeyPressedEvent(Event& e) override;


		bool handleKeyReleasedEvent(Event& e) override;


		bool handleMouseButtonPressedEvent(Event& e) override;


		bool handleMouseButtonReleasedEvent(Event& e) override;


		bool handleMouseMovedEvent(Event& e) override;


		bool handleMouseScrolledEvent(Event& e) override;


		bool handleGamepadButtonPressedEvent(Event& e) override;


		bool handleGamepadButtonReleasedEvent(Event& e) override;


		bool handleGamepadJoystickMovedEvent(Event& e) override;


		void onPollInput() override;

	private:
		GLFWcursor* mCursor = nullptr;
	};
}

#endif