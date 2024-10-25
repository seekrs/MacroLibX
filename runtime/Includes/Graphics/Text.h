#ifndef __MLX_TEXT__
#define __MLX_TEXT__

#include <Graphics/Font.h>

namespace mlx
{
	class Text
	{
		public:
			Text(const std::string& text, std::shared_ptr<Font> font);

			[[nodiscard]] inline const std::string& GetText() const { return m_text; }
			[[nodiscard]] inline std::shared_ptr<Font> GetFont() const { return p_font; }
			[[nodiscard]] MLX_FORCEINLINE std::uint32_t GetColor() const noexcept { return m_color; }

			~Text();

		private:
			std::shared_ptr<Font> p_font;
			std::string m_text;
			std::uint32_t m_color;
	};
}

#endif
