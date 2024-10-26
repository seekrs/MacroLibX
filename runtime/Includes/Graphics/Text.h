#ifndef __MLX_TEXT__
#define __MLX_TEXT__

#include <Graphics/Font.h>
#include <Graphics/Mesh.h>
#include <Graphics/Drawable.h>

namespace mlx
{
	class Text : public Drawable
	{
		friend class Render2DPass;

		public:
			Text(const std::string& text, std::shared_ptr<Font> font);
			inline Text(const std::string& text, std::shared_ptr<Font> font, std::shared_ptr<Mesh> mesh) : Drawable(DrawableType::Text) { Init(text, font, mesh); }

			[[nodiscard]] inline const std::string& GetText() const { return m_text; }
			[[nodiscard]] inline std::shared_ptr<Font> GetFont() const { return p_font; }

			virtual ~Text() = default;

		private:
			void Init(const std::string& text, std::shared_ptr<Font> font, std::shared_ptr<Mesh> mesh);

			inline void Bind(std::size_t frame_index, VkCommandBuffer cmd) override
			{
				if(!p_set)
					return;
				p_set->SetImage(frame_index, 0, const_cast<Texture&>(p_font->GetTexture()));
				p_set->Update(frame_index, cmd);
			}

		private:
			std::shared_ptr<Font> p_font;
			std::string m_text;
	};
}

#endif
