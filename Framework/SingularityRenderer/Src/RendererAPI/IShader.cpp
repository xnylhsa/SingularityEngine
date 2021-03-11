#include "Precompiled.h"
#include "RendererAPI/IShader.h"
#include "RendererAPI/Renderer.h"
#include "vulkan/Shader/VulkanShader.h"
namespace SingularityEngine::SERenderer
{

	void ShaderLibrary::add(const std::string& name, const std::shared_ptr<IShader>& shader)
	{
		ASSERT(!exists(name), "[Renderer::ShaderLibrary] Shader already exists!");
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::add(const std::shared_ptr<IShader>& shader)
	{
		auto& name = shader->getName();
		add(name, shader);
	}

	std::shared_ptr<IShader> ShaderLibrary::load(const std::string& filepath)
	{
		auto shader = IShader::Create(filepath);
		add(shader);
		return shader;
	}

	std::shared_ptr<IShader> ShaderLibrary::load(const std::string& name, const std::string& filepath)
	{
		auto shader = IShader::Create(filepath);
		add(name, shader);
		return shader;
	}

	std::shared_ptr<IShader> ShaderLibrary::get(const std::string& name)
	{
		ASSERT(exists(name), "[Renderer::ShaderLibrary] Shader not found!");
		return m_Shaders[name];
	}

	bool ShaderLibrary::exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}

	std::shared_ptr<IShader> IShader::Create(const std::string& filepath)
	{

		switch (Renderer::GetAPI())
		{
		case RenderingAPI::None:    ASSERT(false, "[Renderer::Shader] None is currently not supported!"); return nullptr;
		case RenderingAPI::Vulkan:  return std::make_shared<VulkanShader>(filepath);
		}
		//load into shader library
		ASSERT(false, "[Renderer::Shader] Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<IShader> IShader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		//get from shader library
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