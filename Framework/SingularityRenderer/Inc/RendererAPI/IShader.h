#ifndef SINGULARITY_ENGINE_RENDERER_SHADER
#define SINGULARITY_ENGINE_RENDERER_SHADER
#include "Common.h"

namespace SingularityEngine::SERenderer
{
	class IShader
	{
	public:
		virtual ~IShader() = default;

		virtual void reload() = 0;
		virtual void bind() = 0;
		virtual void unbind() const = 0;
		virtual void setUniformBuffer(const std::string& name, const void* data, uint32_t size) = 0;
		virtual void setUniform(const std::string& fullname, float value) = 0;
		virtual void setUniform(const std::string& fullname, int value) = 0;
		virtual void setUniform(const std::string& fullname, Math::Vector2 value) = 0;
		virtual void setUniform(const std::string& fullname, Math::Vector3 value) = 0;
		virtual void setUniform(const std::string& fullname, Math::Vector4 value) = 0;
		virtual void setUniform(const std::string& fullname, Math::Matrix3x3 value) = 0;
		virtual void setUniform(const std::string& fullname, Math::Matrix4x4 value) = 0;

		virtual void setInt(const std::string& name, int value) = 0;
		virtual void setIntArray(const std::string& name, int* values, uint32_t count) = 0;
		virtual void setFloat(const std::string& name, float value) = 0;
		virtual void setFloat2(const std::string& name, const Math::Vector2& value) = 0;
		virtual void setFloat3(const std::string& name, const Math::Vector2& value) = 0;
		virtual void setFloat4(const std::string& name, const Math::Vector2& value) = 0;
		virtual void setMat4(const std::string& name, const Math::Matrix4x4& value) = 0;

		virtual const std::string& getName() const = 0;

		static std::shared_ptr<IShader> Create(const std::string& filepath);
		static std::shared_ptr<IShader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
	};

	class ShaderLibrary
	{
	public:
		void add(const std::string& name, const std::shared_ptr<IShader>& shader);
		void add(const std::shared_ptr<IShader>& shader);
		std::shared_ptr<IShader> load(const std::string& filepath);
		std::shared_ptr<IShader> load(const std::string& name, const std::string& filepath);

		std::shared_ptr<IShader> get(const std::string& name);

		bool exists(const std::string& name) const;
	private:
		std::unordered_map<std::string, std::shared_ptr<IShader>> m_Shaders;
	};
}

#endif 
