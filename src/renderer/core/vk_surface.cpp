/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_surface.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 18:58:49 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/10 21:55:21 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render_core.h"
#include <platform/window.h>
#include <renderer/renderer.h>
#include <SDL2/SDL_vulkan.h>
#include <SDL2/SDL.h>
#include <algorithm>

namespace mlx
{
	void Surface::create(Renderer& renderer)
	{
		if(SDL_Vulkan_CreateSurface(renderer.getWindow()->getNativeWindow(), Render_Core::get().getInstance().get(), &_surface) != SDL_TRUE)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to create a surface : %s", SDL_GetError());
		#ifdef DEBUG
			core::error::report(e_kind::message, "Vulkan : created new surface");
		#endif
	}

	VkSurfaceFormatKHR Surface::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
	{
		auto it = std::find_if(availableFormats.begin(), availableFormats.end(), [](VkSurfaceFormatKHR format)
		{
			return format.format == VK_FORMAT_R8G8B8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
		});

		return (it == availableFormats.end() ? availableFormats[0] : *it);
	}

	void Surface::destroy() noexcept
	{
		vkDestroySurfaceKHR(Render_Core::get().getInstance().get(), _surface, nullptr);
		_surface = VK_NULL_HANDLE;
		#ifdef DEBUG
			core::error::report(e_kind::message, "Vulkan : destroyed a surface");
		#endif
	}
}
