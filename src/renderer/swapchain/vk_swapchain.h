/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_swapchain.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:23:27 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/14 17:06:41 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_VK_SWAPCHAIN__
#define __MLX_VK_SWAPCHAIN__

#include <vector>
#include <mlx_profile.h>
#include <volk.h>
#include <renderer/images/vk_image.h>

namespace mlx
{
	class SwapChain
	{
		friend class GraphicPipeline;
		friend class RenderPass;
		friend class Renderer;

		public:
			struct SwapChainSupportDetails
			{
				VkSurfaceCapabilitiesKHR capabilities;
				std::vector<VkSurfaceFormatKHR> formats;
				std::vector<VkPresentModeKHR> present_modes;
			};

		public:
			SwapChain() = default;

			void init(class Renderer* renderer);
			void recreate();
			void destroy() noexcept;

			SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
			VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
			VkPresentModeKHR chooseSwapPresentMode([[maybe_unused]] const std::vector<VkPresentModeKHR> &availablePresentModes);

			inline VkSwapchainKHR get() noexcept { return _swapchain; }
			inline VkSwapchainKHR operator()() noexcept { return _swapchain; }
			inline std::size_t getImagesNumber() const noexcept { return _images.size(); }
			inline Image& getImage(std::size_t i) noexcept { return _images[i]; }
			inline SwapChainSupportDetails getSupport() noexcept { return _swapchain_support; }
			inline VkExtent2D getExtent() noexcept { return _extent; }
			inline VkFormat getImagesFormat() const noexcept { return _swapchain_image_format; }

			~SwapChain() = default;

		private:
			SwapChainSupportDetails _swapchain_support;
			VkSwapchainKHR _swapchain;
			std::vector<Image> _images;
			VkFormat _swapchain_image_format;
			VkExtent2D _extent;
			class Renderer* _renderer = nullptr;
	};
}

#endif // __MLX_VK_SWAPCHAIN__
