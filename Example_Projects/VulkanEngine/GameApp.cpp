#include "GameApp.h"
using namespace SingularityEngine;
bool resizing = false;
bool prepared = false;

GameApp::GameApp()
{

}

GameApp::~GameApp()
{

}

void GameApp::onInitialize()
{
	registerEventFunc(Core::EventType::WindowResize, BIND_EVENT_FN(GameApp::onResize));
	SERenderer::Renderer::Initalize();
	SERenderer::Renderer::Get()->setClearColor(Math::Color(0.2f, 0.2f, 0.2f, 1.0f));
	SERenderer::PipelineSpecification pipelineSpec;
	std::filesystem::path p = "Assets\\Shaders\\GLSL\\triangle.glsl";
	std::filesystem::path startPath = std::filesystem::current_path();

	auto path = startPath.parent_path().parent_path() / (p);
	path.make_preferred();
	mShaderLibrary.load("triangle", path.string());
	pipelineSpec.shader = mShaderLibrary.get("triangle");
	pipelineSpec.shader->reload();
	pipelineSpec.vertexFormat = SERenderer::VertexPC::Format;
	mPipeline =  SERenderer::IPipeline::Create(pipelineSpec);
	mPipeline->invalidate();
	SERenderer::VertexPC vertices[3] = {
		{{ -0.5f,  -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f} },
		{{ 0.5f,  -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f} },
		{{ 0.0f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f} },
	};

	uint32_t indicies[3] = { 0,1,2 };

	mVertexBuffer = SERenderer::IBuffer::Create(&vertices, sizeof(SERenderer::VertexPC) * 3, 44, SERenderer::BufferUsageType::VertexBuffer);
	mIndexBuffer = SERenderer::IBuffer::Create(&indicies, sizeof(uint32_t) * 3, sizeof(uint32_t), SERenderer::BufferUsageType::IndexBuffer);

	prepared = true;
}

void GameApp::onTerminate()
{
	mVertexBuffer.reset();
	mIndexBuffer.reset();
	SERenderer::Renderer::Terminate();
}

bool GameApp::onResize(Core::Event& e)
{
	prepared = false;
	if (isFocused())
	{
		Core::WindowResizeEvent& event = *dynamic_cast<Core::WindowResizeEvent*>(&e);
		SERenderer::Renderer::Get()->resize((uint32_t)event.getWidth(), (uint32_t)event.getHeight());
		prepared = true;
	}
	return true;
}

void GameApp::onUpdate()
{

	if (prepared && isFocused() && isRunning())
	{

		SERenderer::Renderer::Get()->beginScene();
		mPipeline->bind();
		mVertexBuffer->bind();
		mIndexBuffer->bind();
		SERenderer::Renderer::Get()->endScene();
	}
}
