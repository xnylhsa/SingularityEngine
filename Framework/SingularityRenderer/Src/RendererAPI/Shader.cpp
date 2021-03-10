#include "Precompiled.h"
#include "RendererAPI/Shader.h"
#include "RendererAPI/Renderer.h"
#include "External/SPIRV-Cross/Inc/spirv_cross_c.h"

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
		shaderc::Compiler compiler;
		shaderc::CompileOptions compileOptions;
		compileOptions.AddMacroDefinition("MY_DEFINE","1");
		compileOptions.SetOptimizationLevel(shaderc_optimization_level_size);
		std::string shaderCode = "// Author:\
\
		// Title:\
\
#ifdef GL_ES\
		precision mediump float;\
#endif\
\
	uniform vec2 u_resolution;\
	uniform vec2 u_mouse;\
	uniform float u_time;\
\
	void main() {\
		vec2 st = gl_FragCoord.xy / u_resolution.xy;\
		st.x *= u_resolution.x / u_resolution.y;\
\
		vec3 color = vec3(0.);\
		color = vec3(st.x, st.y, abs(sin(u_time)));\
\
		gl_FragColor = vec4(color, 1.0);\
	}";
		shaderc::AssemblyCompilationResult result = compiler.CompileGlslToSpvAssembly(shaderCode, shaderc_glsl_default_vertex_shader, "myvert", compileOptions);


		switch (Renderer::GetAPI())
		{
		case RenderingAPI::None:    ASSERT(false, "[Renderer::Shader] None is currently not supported!"); return nullptr;
		case RenderingAPI::Vulkan:  ASSERT(false, "[Renderer::Shader] Vulkan is currently not supported!"); return nullptr;
		}
		//load into shader library
		UNREFERENCED_PARAMETER(filepath);
		ASSERT(false, "[Renderer::Shader] Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
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