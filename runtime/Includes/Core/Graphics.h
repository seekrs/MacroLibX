/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Graphics.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 14:49:49 by maldavid          #+#    #+#             */
/*   Updated: 2024/07/05 13:46:58 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_GRAPHICS__
#define __MLX_GRAPHICS__

#include <Platform/Window.h>
#include <Renderer/Renderer.h>
#include <Renderer/PixelPut.h>
#include <Renderer/Core/DrawableResource.h>
#include <Renderer/Images/TextureRegistry.h>
#include <Renderer/Texts/TextManager.h>
#include <Utils/NonCopyable.h>
#include <Renderer/Images/Texture.h>

namespace mlx
{
	class GraphicsSupport : public NonCopyable
	{
		public:
			GraphicsSupport(std::size_t w, std::size_t h, NonOwningPtr<Texture> render_target, int id);
			GraphicsSupport(std::size_t w, std::size_t h, std::string title, int id);

			inline int& GetID() noexcept;
			inline std::shared_ptr<Window> GetWindow();

			void Render() noexcept;

			inline void ClearRenderData() noexcept;
			inline void PixelPut(int x, int y, std::uint32_t color) noexcept;
			inline void StringPut(int x, int y, std::uint32_t color, std::string str);
			inline void TexturePut(NonOwningPtr<class Texture> texture, int x, int y);
			inline void LoadFont(const std::filesystem::path& filepath, float scale);
			inline void TryEraseTextureFromManager(NonOwningPtr<Texture> texture) noexcept;

			inline bool HasWindow() const noexcept  { return m_has_window; }

			inline Renderer& GetRenderer() { return m_renderer; }

			~GraphicsSupport();

		private:
			Renderer m_renderer;
			PixelPutPipeline m_pixel_put_pipeline;

			std::vector<NonOwningPtr<DrawableResource>> m_drawlist;

			TextManager m_text_manager;
			TextureRegistry m_texture_registry;

			glm::mat4 m_proj = glm::mat4(1.0);

			std::shared_ptr<Window> p_window;

			std::size_t m_width = 0;
			std::size_t m_height = 0;

			int m_id;

			bool m_has_window;
	};
}

#include <Core/Graphics.inl>

#endif
