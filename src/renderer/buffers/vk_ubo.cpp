/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_ubo.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:45:52 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/22 19:51:56 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vk_ubo.h"
#include <cstring>
#include <renderer/renderer.h>

namespace mlx
{
	void UBO::create(Renderer* renderer, uint32_t size)
	{
		_renderer = renderer;

		for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			_buffers[i].create(Buffer::kind::uniform, size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
			_buffers[i].mapMem(&_maps[i]);
			if(_maps[i] == nullptr)
				core::error::report(e_kind::fatal_error, "Vulkan : unable to map a uniform buffer");
		}
	}

	void UBO::setData(uint32_t size, const void* data)
	{
		std::memcpy(_maps[_renderer->getActiveImageIndex()], data, static_cast<size_t>(size));
	}

	void UBO::setDynamicData(uint32_t size, const void* data)
	{
		std::memcpy(_maps[_renderer->getActiveImageIndex()], data, static_cast<size_t>(size));
		_buffers[_renderer->getActiveImageIndex()].flush();
	}

	unsigned int UBO::getSize() noexcept
	{
		return _buffers[_renderer->getActiveImageIndex()].getSize();
	}

	unsigned int UBO::getOffset() noexcept
	{
		return _buffers[_renderer->getActiveImageIndex()].getOffset();
	}

	VkDeviceMemory UBO::getDeviceMemory() noexcept
	{
		return _buffers[_renderer->getActiveImageIndex()].getDeviceMemory();
	}

	VkBuffer& UBO::operator()() noexcept
	{
		return _buffers[_renderer->getActiveImageIndex()].get();
	}

	VkBuffer& UBO::get() noexcept
	{
		return _buffers[_renderer->getActiveImageIndex()].get();
	}

	void UBO::destroy() noexcept
	{
		for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			_buffers[i].destroy();
	}
}
