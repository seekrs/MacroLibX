/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 15:13:55 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/08 00:19:18 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <core/graphics.h>

namespace mlx
{
	GraphicsSupport::GraphicsSupport(std::size_t w, std::size_t h, std::string title, int id) :
		_window(std::make_shared<MLX_Window>(w, h, std::move(title))),
		_renderer(std::make_unique<Renderer>()), _text_put_pipeline(std::make_unique<TextPutPipeline>()),
		_id(id)
	{
		_renderer->setWindow(_window.get());
		_renderer->init();
		_pixel_put_pipeline.init(w, h, *_renderer);
		_text_put_pipeline->init(_renderer.get());
	}

	void GraphicsSupport::endRender() noexcept
	{
		auto cmd_buff = _renderer->getActiveCmdBuffer().get();

		std::vector<VkDescriptorSet> sets;
		sets.push_back(_renderer->getVertDescriptorSet().get());

		for(auto& data : _textures_to_render)
		{
			if(data.texture->getSet() == VK_NULL_HANDLE)
				data.texture->setDescriptor(_renderer->getFragDescriptorSet().duplicate());
			if(!data.texture->hasBeenUpdated())
				data.texture->updateSet(0);
			sets.push_back(data.texture->getSet());
			vkCmdBindDescriptorSets(cmd_buff, VK_PIPELINE_BIND_POINT_GRAPHICS, _renderer->getPipeline().getPipelineLayout(), 0, sets.size(), sets.data(), 0, nullptr);
			data.texture->render(*_renderer, data.x, data.y);
			sets.pop_back();
		}

		_pixel_put_pipeline.present();

		sets.push_back(_pixel_put_pipeline.getDescriptorSet());
		vkCmdBindDescriptorSets(cmd_buff, VK_PIPELINE_BIND_POINT_GRAPHICS, _renderer->getPipeline().getPipelineLayout(), 0, sets.size(), sets.data(), 0, nullptr);
		_pixel_put_pipeline.render(*_renderer);
		sets.pop_back();

		sets.push_back(_text_put_pipeline->getDescriptorSet());
		vkCmdBindDescriptorSets(cmd_buff, VK_PIPELINE_BIND_POINT_GRAPHICS, _renderer->getPipeline().getPipelineLayout(), 0, sets.size(), sets.data(), 0, nullptr);
		_text_put_pipeline->render();
		
		_renderer->endFrame();

		for(auto& data : _textures_to_render)
			data.texture->resetUpdate();
	}

	GraphicsSupport::~GraphicsSupport()
	{
        vkDeviceWaitIdle(Render_Core::get().getDevice().get());
		_pixel_put_pipeline.destroy();
		_text_put_pipeline->destroy();
		_renderer->destroy();
	}
}
