#pragma once
#include <Framework/Core/Inc/Core.h>
#include <Framework/Math/Inc/EngineMath.h>
#include <Framework/PhysicsMath/Inc/PhysicsMath.h>
#include <Framework/SingularityRenderer/Inc/SingularityRenderer.h>
#include <External/vulkan/vulkan_win32.h>
using namespace SingularityEngine;

class GameApp : public Core::Application
{
public:
	GameApp();
	~GameApp() override;
protected:
	void onInitialize() override;

	void onTerminate() override;

	bool onResize(Core::Event& e);
	void onUpdate() override;

private:
	SERenderer::ShaderLibrary mShaderLibrary;
	std::shared_ptr<SERenderer::IPipeline> mPipeline;
	Core::Timer timer;
	std::shared_ptr<SERenderer::IBuffer> mVertexBuffer;
	std::shared_ptr<SERenderer::IBuffer> mIndexBuffer;

};

