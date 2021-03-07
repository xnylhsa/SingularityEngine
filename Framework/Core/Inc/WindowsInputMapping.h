#ifndef SINGULARITY_ENGINE_WINDOWS_INPUT_MAPPING
#define SINGULARITY_ENGINE_WINDOWS_INPUT_MAPPING
#include "SEInputTypes.h"

namespace SingularityEngine::Core
{
	static void RegisterInputMappings(std::unordered_map<int, SingularityInputType>& converter) 
	{
		converter.emplace(0x01, SingularityInputType::SEKey_Escape);
		converter.emplace(0x3B, SingularityInputType::SEKey_F1);
		converter.emplace(0x3C, SingularityInputType::SEKey_F2);
		converter.emplace(0x3D, SingularityInputType::SEKey_F3);
		converter.emplace(0x3E, SingularityInputType::SEKey_F4);
		converter.emplace(0x3F, SingularityInputType::SEKey_F5);
		converter.emplace(0x40, SingularityInputType::SEKey_F6);
		converter.emplace(0x41, SingularityInputType::SEKey_F7);
		converter.emplace(0x42, SingularityInputType::SEKey_F8);
		converter.emplace(0x43, SingularityInputType::SEKey_F9);
		converter.emplace(0x44, SingularityInputType::SEKey_F10);
		converter.emplace(0x57, SingularityInputType::SEKey_F11);
		converter.emplace(0x58, SingularityInputType::SEKey_F12);

		converter.emplace(0x29, SingularityInputType::SEKey_Grave);
		converter.emplace(0x0C, SingularityInputType::SEKey_Minus);
		converter.emplace(0x0D, SingularityInputType::SEKey_Equals);
		converter.emplace(0x0E, SingularityInputType::SEKey_Backspace);
		converter.emplace(0x1A, SingularityInputType::SEKey_LeftBracket);
		converter.emplace(0x1B, SingularityInputType::SEKey_RightBracket);
		converter.emplace(0x27, SingularityInputType::SEKey_SemiColon);
		converter.emplace(0x28, SingularityInputType::SEKey_Apostrophe);
		converter.emplace(0x1C, SingularityInputType::SEKey_Enter);
		converter.emplace(0x33, SingularityInputType::SEKey_Comma);
		converter.emplace(0x34, SingularityInputType::SEKey_Period);
		converter.emplace(0x35, SingularityInputType::SEKey_ForwardSlash);
		converter.emplace(0x3A, SingularityInputType::SEKey_CapsLock);
		converter.emplace(0x45, SingularityInputType::SEKey_NumLock);
		converter.emplace(0x46, SingularityInputType::SEKey_ScrollLock);
		converter.emplace(0xE052, SingularityInputType::SEKey_Insert);
		converter.emplace(0xE053, SingularityInputType::SEKey_Del);
		converter.emplace(0xE047, SingularityInputType::SEKey_Home);
		converter.emplace(0xE04F, SingularityInputType::SEKey_End);
		converter.emplace(0xE049, SingularityInputType::SEKey_PageUp);
		converter.emplace(0xE051, SingularityInputType::SEKey_PageDown);

		converter.emplace(0x2A, SingularityInputType::SEKey_LeftShift);
		converter.emplace(0x36, SingularityInputType::SEKey_RightShift);
		converter.emplace(0x1D, SingularityInputType::SEKey_LeftCtrl);
		converter.emplace(0xE01D, SingularityInputType::SEKey_RightCtrl);
		converter.emplace(0x38, SingularityInputType::SEKey_LeftAlt);
		converter.emplace(0xE038, SingularityInputType::SEKEY_RightAlt);
		converter.emplace(0x39, SingularityInputType::SEKey_Space);

		converter.emplace(0xE048, SingularityInputType::SEKey_Up);
		converter.emplace(0xE050, SingularityInputType::SEKey_Down);
		converter.emplace(0xE04B, SingularityInputType::SEKey_Left);
		converter.emplace(0xE04D, SingularityInputType::SEKey_Right);

		converter.emplace(0x52, SingularityInputType::SENumpad_0);
		converter.emplace(0x4F, SingularityInputType::SENumpad_1);
		converter.emplace(0x50, SingularityInputType::SENumpad_2);
		converter.emplace(0x51, SingularityInputType::SENumpad_3);
		converter.emplace(0x4B, SingularityInputType::SENumpad_4);
		converter.emplace(0x4C, SingularityInputType::SENumpad_5);
		converter.emplace(0x4D, SingularityInputType::SENumpad_6);
		converter.emplace(0x47, SingularityInputType::SENumpad_7);
		converter.emplace(0x48, SingularityInputType::SENumpad_8);
		converter.emplace(0x49, SingularityInputType::SENumpad_9);
		converter.emplace(0x4E, SingularityInputType::SENumpad_Add);
		converter.emplace(0x4A, SingularityInputType::SENumpad_Subtract);
		converter.emplace(0x37, SingularityInputType::SENumpad_Multiply);
		converter.emplace(0xE035, SingularityInputType::SENumpad_Divide);
		converter.emplace(0xE01C, SingularityInputType::SENumpad_Enter);
		converter.emplace(0x53, SingularityInputType::SENumpad_Decimal);
		converter.emplace(0x0F, SingularityInputType::SEKey_Tab);
		converter.emplace(0x2B, SingularityInputType::SEKey_BackSlash);

		converter.emplace(0x0B, SingularityInputType::SEKey_0);
		converter.emplace(0x02, SingularityInputType::SEKey_1);
		converter.emplace(0x03, SingularityInputType::SEKey_2);
		converter.emplace(0x04, SingularityInputType::SEKey_3);
		converter.emplace(0x05, SingularityInputType::SEKey_4);
		converter.emplace(0x06, SingularityInputType::SEKey_5);
		converter.emplace(0x07, SingularityInputType::SEKey_6);
		converter.emplace(0x08, SingularityInputType::SEKey_7);
		converter.emplace(0x09, SingularityInputType::SEKey_8);
		converter.emplace(0x0A, SingularityInputType::SEKey_9);


		converter.emplace(0x10, SingularityInputType::SEKey_Q);
		converter.emplace(0x11, SingularityInputType::SEKey_W);
		converter.emplace(0x12, SingularityInputType::SEKey_E);
		converter.emplace(0x13, SingularityInputType::SEKey_R);
		converter.emplace(0x14, SingularityInputType::SEKey_T);
		converter.emplace(0x15, SingularityInputType::SEKey_Y);
		converter.emplace(0x16, SingularityInputType::SEKey_U);
		converter.emplace(0x17, SingularityInputType::SEKey_I);
		converter.emplace(0x18, SingularityInputType::SEKey_O);
		converter.emplace(0x19, SingularityInputType::SEKey_P);
		converter.emplace(0x1E, SingularityInputType::SEKey_A);
		converter.emplace(0x1F, SingularityInputType::SEKey_S);
		converter.emplace(0x20, SingularityInputType::SEKey_D);
		converter.emplace(0x21, SingularityInputType::SEKey_F);
		converter.emplace(0x22, SingularityInputType::SEKey_G);
		converter.emplace(0x23, SingularityInputType::SEKey_H);
		converter.emplace(0x24, SingularityInputType::SEKey_J);
		converter.emplace(0x25, SingularityInputType::SEKey_K);
		converter.emplace(0x26, SingularityInputType::SEKey_L);
		converter.emplace(0x2C, SingularityInputType::SEKey_Z);
		converter.emplace(0x2D, SingularityInputType::SEKey_X);
		converter.emplace(0x2E, SingularityInputType::SEKey_C);
		converter.emplace(0x2F, SingularityInputType::SEKey_V);
		converter.emplace(0x30, SingularityInputType::SEKey_B);
		converter.emplace(0x31, SingularityInputType::SEKey_N);
		converter.emplace(0x32, SingularityInputType::SEKey_M);


		converter.emplace(VK_LBUTTON, SingularityInputType::SEMouseLeft);
		converter.emplace(VK_RBUTTON, SingularityInputType::SEMouseRight);
		converter.emplace(VK_MBUTTON, SingularityInputType::SEMouseMiddle);
		converter.emplace(VK_XBUTTON1, SingularityInputType::SEMouseButton4);
		converter.emplace(VK_XBUTTON2, SingularityInputType::SEMouseButton5);

		converter.emplace(VK_GAMEPAD_A, SingularityInputType::SEGamepad_FaceButtonDown);
		converter.emplace(VK_GAMEPAD_B, SingularityInputType::SEGamepad_FaceButtonRight);
		converter.emplace(VK_GAMEPAD_X, SingularityInputType::SEGamepad_FaceButtonLeft);
		converter.emplace(VK_GAMEPAD_Y, SingularityInputType::SEGamepad_FaceButtonUp);
		converter.emplace(VK_GAMEPAD_DPAD_UP, SingularityInputType::SEGamepad_DPadUp);
		converter.emplace(VK_GAMEPAD_DPAD_DOWN, SingularityInputType::SEGamepad_DPadDown);
		converter.emplace(VK_GAMEPAD_DPAD_LEFT, SingularityInputType::SEGamepad_DPadLeft);
		converter.emplace(VK_GAMEPAD_DPAD_RIGHT, SingularityInputType::SEGamepad_DPadRight);
		converter.emplace(VK_GAMEPAD_MENU, SingularityInputType::SEGamepad_Start);
		converter.emplace(VK_GAMEPAD_VIEW, SingularityInputType::SEGamepad_Select);
		converter.emplace(VK_GAMEPAD_RIGHT_TRIGGER, SingularityInputType::SEGamepad_RightTrigger);
		converter.emplace(VK_GAMEPAD_LEFT_TRIGGER, SingularityInputType::SEGamepad_LeftTrigger);
		converter.emplace(VK_GAMEPAD_LEFT_SHOULDER, SingularityInputType::SEGamepad_RightBumper);
		converter.emplace(VK_GAMEPAD_RIGHT_SHOULDER, SingularityInputType::SEGamepad_RightBumper);
		converter.emplace(VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON, SingularityInputType::SEGamepad_LeftStickDown);
		converter.emplace(VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON, SingularityInputType::SEGamepad_RightStickDown);

	}
}



#endif