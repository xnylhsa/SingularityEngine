#include "GameApp.h"
#include "assimp/types.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/config.h"
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
	Assimp::Importer importer;
	const uint32_t flags = aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_TransformUVCoords;
	const aiScene* scene = importer.ReadFile("F:\\git_repos\\SingularityEngine\\Assets\\models\\Knuckles\\Knuckles.fbx", flags);
	std::vector<SERenderer::VertexPC> verticies;
	std::vector<uint32_t> indicies;
	if (scene && scene->HasMeshes())
	{
		for (size_t meshIndex = 0; meshIndex < scene->mNumMeshes; meshIndex++)
		{
			const aiMesh* inputMesh = scene->mMeshes[meshIndex];
			const int numIndicies = inputMesh->mNumFaces * 3;
			for (size_t i = 0; i < inputMesh->mNumVertices; i++)
			{
				SERenderer::VertexPC vertex;
				vertex.position = { inputMesh->mVertices[i].x, inputMesh->mVertices[i].y, inputMesh->mVertices[i].z };
				vertex.color = { 1.0f, 0.0f, 0.0f, 1.0f };
				verticies.push_back(vertex);
			}
			for (uint32_t face = 0, index = 0; face < inputMesh->mNumFaces; ++face, index += 3)
			{
				indicies.push_back(inputMesh->mFaces[face].mIndices[0]);
				indicies.push_back(inputMesh->mFaces[face].mIndices[1]);
				indicies.push_back(inputMesh->mFaces[face].mIndices[2]);
			}
		}
	}
	auto path = startPath.parent_path().parent_path() / (p);
	path.make_preferred();
	mShaderLibrary.load("triangle", path.string(), false);
	pipelineSpec.shader = mShaderLibrary.get("triangle");
	pipelineSpec.vertexFormat = SERenderer::VertexPC::Format;
	mPipeline = SERenderer::IPipeline::Create(pipelineSpec);
	float fov = 80.0f ;
	fov *= 0.5f;
	fov *= Math::kDegtoRad;
	const float farPlane = 100.f;
	const float nearPlane = 0.1f;
	const float aspectRatio = (float)getWindow()->getWidth() / (float)getWindow()->getHeight();
	const float h = -(1.0f / tan(fov));
	const float w = -h/aspectRatio;
	const float q = -(farPlane + nearPlane) / (farPlane - nearPlane );
	const float b = -(2 * farPlane * nearPlane) / (farPlane - nearPlane);
	Math::Matrix4x4 proj
	(
		w, 0.0f, 0.0f, 0.0f,
		0.0f, h, 0.0f, 0.0f,
		0.0f, 0.0f, q, -1.0f,
		0.0f, 0.0f, b, 0.0f
	);
	Math::Vector3 yAxis = Math::Vector3::AxisY();
	Math::Vector3 pos(3.0f, 3.0f, 3.0f);
	Math::Vector3 target(0.0f, 0.0f, 0.0f);
	Math::Vector3 look = Math::Normalize(pos - target);
	Math::Quaternion lookQuaternion = Math::QuaternionFromLookDirection(look);
	const Math::Vector3 R = Math::Rotate(lookQuaternion, Math::Vector3::AxisX());
	const Math::Vector3 U = Math::Rotate(lookQuaternion, Math::Vector3::AxisY());
	const Math::Vector3 L = Math::Rotate(lookQuaternion, Math::Vector3::AxisZ());
	const float dx = -Math::Dot(R, pos);
	const float dy = -Math::Dot(U, pos);
	const float dz = -Math::Dot(L, pos);

	Math::Matrix4x4 view
	{
		R.x, U.x, L.x,  0.0f,
		R.y, U.y, L.y,  0.0f,
		R.z, U.z, L.z,  0.0f,
		dx, dy, dz, 1.0f
	};

	Math::Matrix4x4 transform = Math::Matrix4x4::Identity();
	camera.proj = proj;
	camera.view = view;
	camera.transform = transform;
	mVertexBuffer = SERenderer::IVertexBuffer::Create(verticies.data(), sizeof(SERenderer::VertexPC) * verticies.size(), SERenderer::VertexBufferUsage::Static);
	mIndexBuffer = SERenderer::IIndexBuffer::Create(indicies.data(), sizeof(uint32_t) * indicies.size());
	prepared = true;
}

void GameApp::onTerminate()
{
	mVertexBuffer.reset();
	mIndexBuffer.reset();
	mPipeline.reset();
	mShaderLibrary.cleanup();
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
		std::shared_ptr<SERenderer::VulkanPipeline> pipeline = std::dynamic_pointer_cast<SERenderer::VulkanPipeline>(mPipeline);
		pipeline->pushConstant(VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(Math::Matrix4x4) * 3, &camera);
		mVertexBuffer->bind();
		mIndexBuffer->bind();
		SERenderer::Renderer::Get()->endScene();
	}
}
