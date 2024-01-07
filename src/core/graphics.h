/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 14:49:49 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/07 01:27:09 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_GRAPHICS__
#define __MLX_GRAPHICS__

#include <memory>
#include <unordered_set>
#include <filesystem>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <platform/window.h>
#include <renderer/renderer.h>
#include <renderer/pixel_put.h>
#include <renderer/text_pipeline.h>
#include <utils/non_copyable.h>
#include <renderer/images/texture.h>
#include <mlx_profile.h>

namespace mlx
{
	class GraphicsSupport : public non_copyable
	{
		public:
			GraphicsSupport(std::size_t w, std::size_t h, Texture* render_target, int id);
			GraphicsSupport(std::size_t w, std::size_t h, std::string title, int id);

			inline int& getID() noexcept;
			inline std::shared_ptr<MLX_Window> getWindow();

			void render() noexcept;

			inline void clearRenderData() noexcept;
			inline void pixelPut(int x, int y, uint32_t color) noexcept;
			inline void stringPut(int x, int y, int color, std::string str);
			inline void texturePut(Texture* texture, int x, int y);
			inline void loadFont(const std::filesystem::path& filepath, float scale);

			inline Renderer& getRenderer() { return *_renderer; }

			~GraphicsSupport();

		private:
			std::vector<TextureRenderData> _textures_to_render;
			PixelPutPipeline _pixel_put_pipeline;
			glm::mat4 _proj = glm::mat4(1.0);
			std::shared_ptr<MLX_Window> _window;
			std::unique_ptr<TextPutPipeline> _text_put_pipeline; // unique_ptr because of the size of the class
			std::unique_ptr<Renderer> _renderer;
			std::size_t _width = 0;
			std::size_t _height = 0;
			int _id;
	};
}

#include <core/graphics.inl>

#endif
