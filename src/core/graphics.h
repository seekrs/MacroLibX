/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 14:49:49 by maldavid          #+#    #+#             */
/*   Updated: 2023/12/11 19:47:03 by kbz_8            ###   ########.fr       */
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
			GraphicsSupport(std::size_t w, std::size_t h, const std::string& title, int id);

			inline int& getID() noexcept;
			inline std::shared_ptr<MLX_Window> getWindow();

			inline void beginRender() noexcept;
			void endRender() noexcept;

			inline void clearRenderData() noexcept;
			inline void pixelPut(int x, int y, uint32_t color) noexcept;
			inline void stringPut(int x, int y, int color, std::string str);
			inline void texturePut(Texture* texture, int x, int y);
			inline void loadFont(const std::filesystem::path& filepath, float scale);

			~GraphicsSupport();

		private:
			std::vector<TextureRenderData> _textures_to_render;
			PixelPutPipeline _pixel_put_pipeline;
			glm::mat4 _proj = glm::mat4(1.0);
			std::shared_ptr<MLX_Window> _window;
			std::unique_ptr<TextPutPipeline> _text_put_pipeline; // unique_ptr because of the size of the class
			std::unique_ptr<Renderer> _renderer;
			int _id;
	};
}

#include <core/graphics.inl>

#endif
