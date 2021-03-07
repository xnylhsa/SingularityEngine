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
	void OnInitialize() override;

	void OnTerminate() override;

	bool OnResize(Core::Event& e);
	void OnUpdate() override;

private:
	Core::Timer timer;
};

