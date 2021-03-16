#ifndef SINGULARITY_ENGINE_RENDERER_SHADER
#define SINGULARITY_ENGINE_RENDERER_SHADER
#include "Common.h"

namespace SingularityEngine::SERenderer
{

	enum class ShaderUniformType
	{
		None = 0,
		Bool,
		Int,
		UInt,
		Float,
		Vec2,
		Vec3,
		Vec4,
		Mat3,
		Mat4,
		Max
	};

	class ShaderUniform
	{
	public:
		ShaderUniform() = default;
		ShaderUniform(const std::string& name, ShaderUniformType type, uint32_t size, uint32_t offset, uint32_t bindingPoint, uint32_t set);

		const std::string& GetName() const { return mName; }
		ShaderUniformType GetType() const { return mType; }
		uint32_t GetSize() const { return mSize; }
		uint32_t GetOffset() const { return mOffset; }
		uint32_t GetBindingPoint() const { return mBindingPoint; }
		uint32_t GetSet() const { return mSet; }

		static const std::string UniformTypeToString(ShaderUniformType type);
	private:
		std::string mName;
		ShaderUniformType mType = ShaderUniformType::None;
		uint32_t mSize = 0;
		uint32_t mOffset = 0;
		uint32_t mBindingPoint = 0;
		uint32_t mSet = 0;

	};

	struct ShaderBuffer
	{
		std::string Name;
		uint32_t Size = 0;
		std::unordered_map<std::string, ShaderUniform> Uniforms;
	};


	class IShader
	{
	public:
		IShader() = default;
		virtual ~IShader() = default;

		virtual void reload(bool forceCompile) = 0;
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

		virtual const std::string& getName() const = 0;
		IShader(const IShader & other) = delete;
		void operator=(const IShader& other) = delete;

		static std::shared_ptr<IShader> Create(const std::string& filepath);
	};

	class ShaderLibrary
	{
	public:
		void add(const std::string& name, const std::shared_ptr<IShader>& shader);
		void add(const std::shared_ptr<IShader>& shader);
		std::shared_ptr<IShader> load(const std::string& filepath, bool forceCompilation = false);
		std::shared_ptr<IShader> load(const std::string& name, const std::string& filepath, bool forceCompilation = false);

		std::shared_ptr<IShader> get(const std::string& name);

		bool exists(const std::string& name) const;
		void cleanup() { m_Shaders.clear(); }
	private:
		std::unordered_map<std::string, std::shared_ptr<IShader>> m_Shaders;
	};
}

#endif 
