/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Swapchain.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:23:27 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/28 22:18:15 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_VK_SWAPCHAIN__
#define __MLX_VK_SWAPCHAIN__

#include <Renderer/Images/Image.h>

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

			void Init(NonOwningPtr<class Renderer> renderer);
			void Recreate();
			void Destroy() noexcept;

			SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
			VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
			VkPresentModeKHR ChooseSwapPresentMode([[maybe_unused]] const std::vector<VkPresentModeKHR> &available_present_modes);

			inline VkSwapchainKHR Get() noexcept { return m_swapchain; }
			inline VkSwapchainKHR operator()() noexcept { return m_swapchain; }
			inline std::size_t GetImagesNumber() const noexcept { return m_images.size(); }
			inline Image& GetImage(std::size_t i) noexcept { return m_images[i]; }
			inline SwapChainSupportDetails GetSupport() noexcept { return m_swapchain_support; }
			inline VkExtent2D GetExtent() noexcept { return m_extent; }
			inline VkFormat GetImagesFormat() const noexcept { return m_swapchain_image_format; }

			~SwapChain() = default;

		private:
			SwapChainSupportDetails m_swapchain_support;
			VkSwapchainKHR m_swapchain;
			std::vector<Image> m_images;
			VkFormat m_swapchain_image_format;
			VkExtent2D m_extent;
			NonOwningPtr<class Renderer> m_renderer;
	};
}

#endif // __MLX_VK_SWAPCHAIN__
