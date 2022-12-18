/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_surface.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 18:58:49 by maldavid          #+#    #+#             */
/*   Updated: 2022/12/18 22:20:57 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render_core.h"
#include <platform/window.h>
#include <renderer/renderer.h>
#include <SDL2/SDL_vulkan.h>
#include <SDL2/SDL.h>

namespace mlx
{
	void Surface::create(Renderer& renderer)
	{
		if(SDL_Vulkan_CreateSurface(renderer.getWindow()->getNativeWindow(), Render_Core::get().getInstance().get(), &_surface) != SDL_TRUE)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to create a surface : %s", SDL_GetError());
	}

	VkSurfaceFormatKHR Surface::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
	{
		for(const auto& availableFormat : availableFormats)
		{
			if(availableFormat.format == VK_FORMAT_R8G8B8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
				return availableFormat;
		}

		return availableFormats[0];
	}

	void Surface::destroy() noexcept
	{
		vkDestroySurfaceKHR(Render_Core::get().getInstance().get(), _surface, nullptr);
	}
}
