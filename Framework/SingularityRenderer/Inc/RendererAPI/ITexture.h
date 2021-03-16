#ifndef SINGULARITY_ENGINE_RENDERER_TEXTURE
#define SINGULARITY_ENGINE_RENDERER_TEXTURE
#include "Common.h"


namespace SingularityEngine::SERenderer
{
	enum class TextureFormat
	{
		none = 0,
		RGB,
		RGBA,
		Float16,
		Float34,
		Max
	};

	enum class TextureWrapMode
	{
		none = 0,
		Clamp,
		Repeat,
		Max
	};

	class ITexture
	{
	public:
		virtual ~ITexture() = default;

		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;
		virtual uint32_t getMipLevelCount() const = 0;

		virtual TextureFormat getFormat() const = 0;

		virtual void bind(uint32_t slot = 0) const = 0;

		static uint32_t CalculateMipMapCount(uint32_t width, uint32_t height);
		virtual uint64_t getHash() const = 0;

		virtual bool operator==(const ITexture& other) const = 0;
	};

	class ITexture2D : public ITexture
	{
	public:
		static std::shared_ptr<ITexture2D> Create(TextureFormat format, uint32_t width, uint32_t height, const void* data, TextureWrapMode wrapMode);
		static std::shared_ptr<ITexture2D> Create(const std::string& path);

		virtual void lock() = 0;
		virtual void unlock() = 0;

		virtual void resize(uint32_t width, uint32_t height) = 0;
		virtual Core::Buffer getWriteableBuffer() = 0;

		virtual bool loaded() const = 0;
		virtual const std::string& GetPath() const = 0;

	};

	class ITextureCube : public ITexture
	{
	public:
		static std::shared_ptr<ITextureCube> Create(TextureFormat format, uint32_t width, uint32_t height);
		static std::shared_ptr<ITextureCube> Create(const std::string& path);
		virtual const std::string& GetPath() const = 0;

	};
}

#endif 
