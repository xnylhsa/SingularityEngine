#ifndef SINGULARITY_ENGINE_RENDER_RENDERER_CONTEXT
#define SINGULARITY_ENGINE_RENDER_RENDERER_CONTEXT
#include "Common.h"

namespace SingularityEngine::SERenderer
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetInt(const std::string& name, int value) = 0;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) = 0;
		virtual void SetFloat(const std::string& name, float value) = 0;
		virtual void SetFloat2(const std::string& name, const Math::Vector2& value) = 0;
		virtual void SetFloat3(const std::string& name, const Math::Vector2& value) = 0;
		virtual void SetFloat4(const std::string& name, const Math::Vector2& value) = 0;
		virtual void SetMat4(const std::string& name, const Math::Matrix4x4& value) = 0;

		virtual const std::string& GetName() const = 0;

		static std::shared_ptr<Shader> Create(const std::string& filepath);
		static std::shared_ptr<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
	};

	class ShaderLibrary
	{
	public:
		void Add(const std::string& name, const std::shared_ptr<Shader>& shader);
		void Add(const std::shared_ptr<Shader>& shader);
		std::shared_ptr<Shader> Load(const std::string& filepath);
		std::shared_ptr<Shader> Load(const std::string& name, const std::string& filepath);

		std::shared_ptr<Shader> Get(const std::string& name);

		bool Exists(const std::string& name) const;
	private:
		std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;
	};
}

#endif 
