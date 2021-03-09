#include "Precompiled.h"
#include "RendererAPI/Shader.h"
#include "RendererAPI/Renderer.h"


namespace SingularityEngine::SERenderer
{

	void ShaderLibrary::Add(const std::string& name, const std::shared_ptr<Shader>& shader)
	{
		ASSERT(!Exists(name), "[Renderer::ShaderLibrary] Shader already exists!");
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::Add(const std::shared_ptr<Shader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}

	std::shared_ptr<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	std::shared_ptr<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}

	std::shared_ptr<Shader> ShaderLibrary::Get(const std::string& name)
	{
		ASSERT(Exists(name), "[Renderer::ShaderLibrary] Shader not found!");
		return m_Shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}

	std::shared_ptr<Shader> Shader::Create(const std::string& filepath)
	{

		switch (Renderer::GetAPI())
		{
		case RenderingAPI::None:    ASSERT(false, "[Renderer::Shader] None is currently not supported!"); return nullptr;
		case RenderingAPI::Vulkan:  ASSERT(false, "[Renderer::Shader] Vulkan is currently not supported!"); return nullptr;
		}

		UNREFERENCED_PARAMETER(filepath);
		ASSERT(false, "[Renderer::Shader] Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{

		switch (Renderer::GetAPI())
		{
		case RenderingAPI::None:    ASSERT(false, "[Renderer::Shader] None is currently not supported!"); return nullptr;
		case RenderingAPI::Vulkan:  ASSERT(false, "[Renderer::Shader] Vulkan is currently not supported!"); return nullptr;
		}

		UNREFERENCED_PARAMETER(name);
		UNREFERENCED_PARAMETER(vertexSrc);
		UNREFERENCED_PARAMETER(fragmentSrc);
		ASSERT(false, "[Renderer::Shader] Unknown RendererAPI!");
		return nullptr;
	}

}