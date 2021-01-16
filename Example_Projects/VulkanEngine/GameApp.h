#pragma once
#include <Framework/Core/Inc/Core.h>
#include <Framework/Math/Inc/EngineMath.h>
#include <Framework/PhysicsMath/Inc/PhysicsMath.h>
#include <Framework/VulkanRenderer/Inc/VulkanRenderer.h>
#include <Framework/Input/Inc/Input.h>
#include <External/vulkan/vulkan_win32.h>
using namespace SingularityEngine;

class GameApp : public Core::Application
{
public:
	GameApp();
	~GameApp() override;

protected:
	void OnInitialize(uint32_t w, uint32_t h) override;


	void OnTerminate() override;


	void OnUpdate() override;

private:
	Core::Timer timer;
	Core::Window mWindow;
	uint32_t wWidth;
	uint32_t wHeight;
};

