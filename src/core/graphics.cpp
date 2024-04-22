/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 15:13:55 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/22 17:34:27 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <core/graphics.h>

namespace mlx
{
	GraphicsSupport::GraphicsSupport(std::size_t w, std::size_t h, Texture* render_target, int id) :
		_window(nullptr),
		_renderer(std::make_unique<Renderer>()),
		_width(w),
		_height(h),
		_id(id),
		_has_window(false)
	{
		MLX_PROFILE_FUNCTION();
		_renderer->setWindow(nullptr);
		_renderer->init(render_target);
		_pixel_put_pipeline.init(w, h, *_renderer);
		_text_manager.init(*_renderer);
	}

	GraphicsSupport::GraphicsSupport(std::size_t w, std::size_t h, std::string title, int id) :
		_window(std::make_shared<MLX_Window>(w, h, title)),
		_renderer(std::make_unique<Renderer>()), 
		_width(w),
		_height(h),
		_id(id),
		_has_window(true)
	{
		MLX_PROFILE_FUNCTION();
		_renderer->setWindow(_window.get());
		_renderer->init(nullptr);
		_pixel_put_pipeline.init(w, h, *_renderer);
		_text_manager.init(*_renderer);
	}

	void GraphicsSupport::render() noexcept
	{
		MLX_PROFILE_FUNCTION();
		if(!_renderer->beginFrame())
			return;
		_proj = glm::ortho<float>(0, _width, 0, _height);
		_renderer->getUniformBuffer()->setData(sizeof(_proj), &_proj);

		std::array<VkDescriptorSet, 2> sets = {
			_renderer->getVertDescriptorSet().get(), 
			VK_NULL_HANDLE
		};

		for(auto& data : _drawlist)
			data->render(sets, *_renderer);

		_pixel_put_pipeline.render(sets, *_renderer);

		_renderer->endFrame();

		for(auto& data : _drawlist)
			data->resetUpdate();

		#ifdef GRAPHICS_MEMORY_DUMP
			// dump memory to file every two seconds
			static std::uint64_t timer = SDL_GetTicks64();
			if(SDL_GetTicks64() - timer > 2000)
			{
				Render_Core::get().getAllocator().dumpMemoryToJson();
				timer += 2000;
			}
		#endif
	}

	GraphicsSupport::~GraphicsSupport()
	{
		MLX_PROFILE_FUNCTION();
		vkDeviceWaitIdle(Render_Core::get().getDevice().get());
		_text_manager.destroy();
		_pixel_put_pipeline.destroy();
		_renderer->destroy();
		if(_window)
			_window->destroy();
	}
}
