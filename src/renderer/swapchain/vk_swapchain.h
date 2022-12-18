/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_swapchain.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:23:27 by maldavid          #+#    #+#             */
/*   Updated: 2022/12/18 22:08:36 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_VK_SWAPCHAIN__
#define __MLX_VK_SWAPCHAIN__

#include <vector>
#include <volk.h>
#include "vk_imageview.h"
#include "vk_framebuffer.h"

namespace mlx
{
    class SwapChain
    {
        friend class FrameBuffer;
        friend class ImageView;
        friend class GraphicPipeline;
        friend class RenderPass;
		friend class Renderer;

        public:
            struct SwapChainSupportDetails
            {
                VkSurfaceCapabilitiesKHR capabilities;
                std::vector<VkSurfaceFormatKHR> formats;
            };

            void init(class Renderer* renderer);
            void initFB();
            void destroy() noexcept;
            void destroyFB() noexcept;

            void recreate();

            SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
            VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

            inline SwapChainSupportDetails getSupport() noexcept { return _swapChainSupport; }
            inline VkSwapchainKHR& operator()() noexcept { return _swapChain; }
            inline VkSwapchainKHR& get() noexcept { return _swapChain; }
            inline FrameBuffer& getFrameBuffer(int i) { return _framebuffers[i]; }
            inline size_t getImagesNumber() const noexcept { return _swapChainImages.size(); }

        private:
            SwapChainSupportDetails _swapChainSupport;
            std::vector<VkImage> _swapChainImages;
            std::vector<FrameBuffer> _framebuffers;
            std::vector<ImageView> _imageViews;
            VkSwapchainKHR _swapChain;
            VkFormat _swapChainImageFormat;
            VkExtent2D _swapChainExtent;
			class Renderer* _renderer = nullptr;
    };
}

#endif // __MLX_VK_SWAPCHAIN__
