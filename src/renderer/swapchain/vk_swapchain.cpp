/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_swapchain.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:22:28 by maldavid          #+#    #+#             */
/*   Updated: 2023/01/25 11:39:01 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <renderer/core/render_core.h>
#include <renderer/renderer.h>
#include <platform/window.h>
#include <SDL2/SDL_vulkan.h>
#include <algorithm>

namespace mlx
{
    void SwapChain::init(Renderer* renderer)
    {
		_renderer = renderer;

        _swapChainSupport = querySwapChainSupport(Render_Core::get().getDevice().getPhysicalDevice());

        VkSurfaceFormatKHR surfaceFormat = renderer->getSurface().chooseSwapSurfaceFormat(_swapChainSupport.formats);
        VkPresentModeKHR presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR; // change this to set vsync (if the driver supports it)
        VkExtent2D extent = chooseSwapExtent(_swapChainSupport.capabilities);

        uint32_t imageCount = _swapChainSupport.capabilities.minImageCount + 1;
        if(_swapChainSupport.capabilities.maxImageCount > 0 && imageCount > _swapChainSupport.capabilities.maxImageCount)
            imageCount = _swapChainSupport.capabilities.maxImageCount;

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = renderer->getSurface().get();

        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        Queues::QueueFamilyIndices indices = Render_Core::get().getQueue().findQueueFamilies(Render_Core::get().getDevice().getPhysicalDevice(), renderer->getSurface().get());
        uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

        if(indices.graphicsFamily != indices.presentFamily)
		{
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
		else
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

        createInfo.preTransform = _swapChainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;

        createInfo.oldSwapchain = VK_NULL_HANDLE;

        VkDevice device = Render_Core::get().getDevice().get();

        if(vkCreateSwapchainKHR(device, &createInfo, nullptr, &_swapChain) != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to create swapchain");

        vkGetSwapchainImagesKHR(device, _swapChain, &imageCount, nullptr);
        _swapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(device, _swapChain, &imageCount, _swapChainImages.data());

        _swapChainImageFormat = surfaceFormat.format;
        _swapChainExtent = extent;

        _imageViews.resize(_swapChainImages.size());

        for(size_t i = 0; i < _swapChainImages.size(); i++)
            _imageViews[i].init(*this, _swapChainImages[i]);
    }

    void SwapChain::initFB()
    {
        _framebuffers.resize(_imageViews.size());

        for(size_t i = 0; i < _imageViews.size(); i++)
            _framebuffers[i].init(*_renderer, _imageViews[i]);
    }

    SwapChain::SwapChainSupportDetails SwapChain::querySwapChainSupport(VkPhysicalDevice device)
    {
        SwapChain::SwapChainSupportDetails details;
        VkSurfaceKHR surface = _renderer->getSurface().get();

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

        uint32_t formatCount = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

        if(formatCount != 0)
        {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
        }
        return details;
    }

    VkExtent2D SwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
    {
        if(capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
            return capabilities.currentExtent;

        int width, height;
        SDL_Vulkan_GetDrawableSize(_renderer->getWindow()->getNativeWindow(), &width, &height);

        VkExtent2D actualExtent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }

    void SwapChain::recreate()
    {
        destroyFB();
        destroy();
        _renderer->getRenderPass().destroy();

        init(_renderer);
        _renderer->getRenderPass().init(_renderer);
        initFB();
    }

    void SwapChain::destroyFB() noexcept
    {
        vkDeviceWaitIdle(Render_Core::get().getDevice().get());
            
        for(size_t i = 0; i < _framebuffers.size(); i++)
            _framebuffers[i].destroy();
    }

    void SwapChain::destroy() noexcept
    {
        vkDeviceWaitIdle(Render_Core::get().getDevice().get());

        for(size_t i = 0; i < _imageViews.size(); i++)
            _imageViews[i].destroy();

        if(_swapChain != VK_NULL_HANDLE)
            vkDestroySwapchainKHR(Render_Core::get().getDevice().get(), _swapChain, nullptr);
        _swapChain = VK_NULL_HANDLE;
    }
}
