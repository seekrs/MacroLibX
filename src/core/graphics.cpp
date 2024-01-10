/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 15:13:55 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/10 18:23:26 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <core/graphics.h>

namespace mlx
{
	GraphicsSupport::GraphicsSupport(std::size_t w, std::size_t h, Texture* render_target, int id) :
		_window(nullptr),
		_text_put_pipeline(std::make_unique<TextPutPipeline>()),
		_renderer(std::make_unique<Renderer>()),
		_width(w),
		_height(h),
		_id(id)
	{
		MLX_PROFILE_FUNCTION();
		_renderer->setWindow(nullptr);
		_renderer->init(render_target);
		_pixel_put_pipeline.init(w, h, *_renderer);
		_text_put_pipeline->init(_renderer.get());
	}

	GraphicsSupport::GraphicsSupport(std::size_t w, std::size_t h, std::string title, int id) :
		_window(std::make_shared<MLX_Window>(w, h, title)),
		_text_put_pipeline(std::make_unique<TextPutPipeline>()),
		_renderer(std::make_unique<Renderer>()), 
		_width(w),
		_height(h),
		_id(id)
	{
		MLX_PROFILE_FUNCTION();
		_renderer->setWindow(_window.get());
		_renderer->init(nullptr);
		_pixel_put_pipeline.init(w, h, *_renderer);
		_text_put_pipeline->init(_renderer.get());
	}

	void GraphicsSupport::render() noexcept
	{
		MLX_PROFILE_FUNCTION();
		if(!_renderer->beginFrame())
			return;
		_proj = glm::ortho<float>(0, _width, 0, _height);
		_renderer->getUniformBuffer()->setData(sizeof(_proj), &_proj);
		auto cmd_buff = _renderer->getActiveCmdBuffer().get();

		static std::array<VkDescriptorSet, 2> sets = {
			_renderer->getVertDescriptorSet().get(), 
			VK_NULL_HANDLE
		};

		for(auto& data : _textures_to_render)
		{
			if(!data.texture->isInit())
				continue;
			if(data.texture->getSet() == VK_NULL_HANDLE)
				data.texture->setDescriptor(_renderer->getFragDescriptorSet().duplicate());
			if(data.texture->getLayout() != VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
				data.texture->transitionLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
			if(!data.texture->hasBeenUpdated())
				data.texture->updateSet(0);
			sets[1] = data.texture->getSet();
			vkCmdBindDescriptorSets(cmd_buff, VK_PIPELINE_BIND_POINT_GRAPHICS, _renderer->getPipeline().getPipelineLayout(), 0, sets.size(), sets.data(), 0, nullptr);
			data.texture->render(*_renderer, data.x, data.y);
		}

		_pixel_put_pipeline.present();
		sets[1] = _pixel_put_pipeline.getDescriptorSet();
		vkCmdBindDescriptorSets(cmd_buff, VK_PIPELINE_BIND_POINT_GRAPHICS, _renderer->getPipeline().getPipelineLayout(), 0, sets.size(), sets.data(), 0, nullptr);
		_pixel_put_pipeline.render(*_renderer);

		_text_put_pipeline->render(sets);

		_renderer->endFrame();

		for(auto& data : _textures_to_render)
			data.texture->resetUpdate();

		#ifdef GRAPHICS_MEMORY_DUMP
			// dump memory to file every two seconds
			static uint64_t timer = SDL_GetTicks64();
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
		_text_put_pipeline->destroy();
		_pixel_put_pipeline.destroy();
		_renderer->destroy();
		if(_window)
			_window->destroy();
	}
}
