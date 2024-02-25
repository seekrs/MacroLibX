/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_ubo.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:45:52 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/10 18:30:57 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vk_ubo.h"
#include <cstring>
#include <renderer/renderer.h>
#include <core/profiler.h>

namespace mlx
{
	void UBO::create(Renderer* renderer, std::uint32_t size, [[maybe_unused]] const char* name)
	{
		MLX_PROFILE_FUNCTION();
		_renderer = renderer;

		for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			#ifdef DEBUG
				std::string name_frame = name;
				name_frame.append(std::to_string(i));
				_buffers[i].create(Buffer::kind::uniform, size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, name_frame.c_str());
			#else
				_buffers[i].create(Buffer::kind::uniform, size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, nullptr);
			#endif
			_buffers[i].mapMem(&_maps[i]);
			if(_maps[i] == nullptr)
				core::error::report(e_kind::fatal_error, "Vulkan : unable to map a uniform buffer");
		}
	}

	void UBO::setData(std::uint32_t size, const void* data)
	{
		MLX_PROFILE_FUNCTION();
		std::memcpy(_maps[_renderer->getActiveImageIndex()], data, static_cast<std::size_t>(size));
	}

	void UBO::setDynamicData(std::uint32_t size, const void* data)
	{
		MLX_PROFILE_FUNCTION();
		std::memcpy(_maps[_renderer->getActiveImageIndex()], data, static_cast<std::size_t>(size));
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
