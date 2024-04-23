/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Surface.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 18:58:49 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/23 18:56:56 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pre_compiled.h>
#include "render_core.h"
#include <platform/window.h>
#include <renderer/renderer.h>

namespace mlx
{
	void Surface::Create(Renderer& renderer)
	{
		if(glfwCreateWindowSurface(RenderCore::Get().GetInstance().Get(), renderer.GetWindow()->GetNativeWindow(), NULL, &m_surface) != VK_SUCCESS)
			FatalError("Vulkan : failed to create a surface");
		DebugLog("Vulkan : created new surface");
	}

	VkSurfaceFormatKHR Surface::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& available_formats)
	{
		auto it = std::find_if(available_formats.begin(), available_formats.end(), [](VkSurfaceFormatKHR format)
		{
			return format.format == VK_FORMAT_R8G8B8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
		});

		return (it == available_formats.end() ? available_formats[0] : *it);
	}

	void Surface::Destroy() noexcept
	{
		vkDestroySurfaceKHR(RenderCore::Get().GetInstance().Get(), m_surface, nullptr);
		m_surface = VK_NULL_HANDLE;
		DebugLog("Vulkan : destroyed a surface");
	}
}
