/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_image.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 11:59:07 by maldavid          #+#    #+#             */
/*   Updated: 2023/01/25 12:01:03 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vk_image.h"
#include <renderer/core/render_core.h>

namespace mlx
{
	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
	{
		for(VkFormat format : candidates)
		{
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(Render_Core::get().getDevice().getPhysicalDevice(), format, &props);
			if(tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
				return format;
			else if(tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
				return format;
		}
		core::error::report(e_kind::fatal_error, "Vulkan : failed to find image format");
	}

	void Image::create(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImageAspectFlags aspectFlags)
	{
		_width = width;
		_height = height;
		_format = format;

		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = width;
		imageInfo.extent.height = height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = format;
		imageInfo.tiling = tiling;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = usage;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if(vkCreateImage(Render_Core::get().getDevice().get(), &imageInfo, nullptr, &_image) != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to create an image");

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(Render_Core::get().getDevice().get(), _image, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = RCore::findMemoryType(memRequirements.memoryTypeBits, properties);

		if(vkAllocateMemory(Render_Core::get().getDevice().get(), &allocInfo, nullptr, &_memory) != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to allocate memory for an image");

		vkBindImageMemory(Render_Core::get().getDevice().get(), _image, _memory, 0);

		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = _image;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = format;
		viewInfo.subresourceRange.aspectMask = aspectFlags;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		if(vkCreateImageView(Render_Core::get().getDevice().get(), &viewInfo, nullptr, &_image_view) != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to create an image view");
	}

	void Image::destroy() noexcept
	{
		vkDestroyImage(Render_Core::get().getDevice().get(), _image, nullptr);
	    vkFreeMemory(Render_Core::get().getDevice().get(), _memory, nullptr);
		vkDestroyImageView(Render_Core::get().getDevice().get(), _image_view, nullptr);
	}

}
