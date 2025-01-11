#ifndef __MLX_FONT__
#define __MLX_FONT__

#include <Renderer/Image.h>

namespace mlx
{
	class Font
	{
		public:
			Font(const std::filesystem::path& path, float scale) : m_build_data(path), m_name(path.string()), m_scale(scale) {}
			Font(const std::string& name, const std::vector<std::uint8_t>& ttf_data, float scale) : m_build_data(ttf_data), m_name(name), m_scale(scale) {}

			void BuildFont();
			void Destroy();

			inline const std::string& GetName() const { return m_name; }
			inline float GetScale() const noexcept { return m_scale; }
			inline const std::array<stbtt_packedchar, 96>& GetCharData() const { return m_cdata; }
			inline const Texture& GetTexture() const noexcept { return m_atlas; }
			inline bool operator==(const Font& rhs) const { return rhs.m_name == m_name && rhs.m_scale == m_scale; }
			inline bool operator!=(const Font& rhs) const { return rhs.m_name != m_name || rhs.m_scale != m_scale; }

			inline ~Font() { Destroy(); }

		private:
			std::array<stbtt_packedchar, 96> m_cdata;
			Texture m_atlas;
			std::variant<std::filesystem::path, std::vector<std::uint8_t>> m_build_data;
			std::string m_name;
			float m_scale;
	};

	class FontRegistry
	{
		public:
			FontRegistry() = default;

			inline void RegisterFont(std::shared_ptr<Font> font);
			inline void UnregisterFont(std::shared_ptr<Font> font);
			inline std::shared_ptr<Font> GetFont(const std::filesystem::path& name, float scale);
			inline void Reset();

			~FontRegistry() = default;

		private:
			std::unordered_set<std::shared_ptr<Font>> m_fonts_registry;
	};
}

#include <Graphics/Font.inl>

#endif
