/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 14:49:49 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/02 15:36:49 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_GRAPHICS__
#define __MLX_GRAPHICS__

#include <memory>
#include <unordered_set>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <platform/window.h>
#include <renderer/renderer.h>
#include <utils/non_copyable.h>
#include <renderer/images/texture.h>

namespace mlx
{
	class GraphicsSupport : public non_copyable
	{
		public:
			GraphicsSupport(std::size_t w, std::size_t h, std::string title, int id);

			inline int& getID() noexcept;
			inline std::shared_ptr<MLX_Window> getWindow();

			inline void beginRender() noexcept;

			inline void clearRenderData() noexcept;
			inline void pixelPut(int x, int y, int color) noexcept;
			inline void texturePut(std::shared_ptr<Texture> texture, int x, int y);
			
			void endRender() noexcept;

			~GraphicsSupport();

		private:
			std::unordered_set<TextureRenderData> _textures_to_render;
			glm::mat4 _proj = glm::mat4(1.0);
			std::shared_ptr<MLX_Window> _window;
			PixelPutPipeline _pixel_put_pipeline;
			std::unique_ptr<Renderer> _renderer;
			int _id;
	};
}

#include <core/graphics.inl>

#endif
