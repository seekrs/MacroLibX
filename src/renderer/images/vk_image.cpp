/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_image.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 11:59:07 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/01 16:05:03 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vk_image.h"
#include <renderer/core/render_core.h>
#include <renderer/buffers/vk_buffer.h>
#include <renderer/command/vk_cmd_pool.h>

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
		return VK_FORMAT_R8G8B8A8_UNORM; // to avoid warning;
	}

	void Image::create(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties)
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
	}

	void Image::createImageView(VkImageViewType type, VkImageAspectFlags aspectFlags) noexcept
	{
		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = _image;
		viewInfo.viewType = type;
		viewInfo.format = _format;
		viewInfo.subresourceRange.aspectMask = aspectFlags;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		if(vkCreateImageView(Render_Core::get().getDevice().get(), &viewInfo, nullptr, &_image_view) != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to create an image view");
	}

	void Image::createSampler() noexcept
	{
		VkSamplerCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		info.magFilter = VK_FILTER_LINEAR;
		info.minFilter = VK_FILTER_LINEAR;
		info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		info.minLod = -1000;
		info.maxLod = 1000;
		info.anisotropyEnable = VK_FALSE;
		info.maxAnisotropy = 1.0f;

		if(vkCreateSampler(Render_Core::get().getDevice().get(), &info, nullptr, &_sampler) != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to create an image");
	}

	void Image::copyBuffer(Buffer& buffer)
	{
		CmdPool cmdpool;
		cmdpool.init();
		auto device = Render_Core::get().getDevice().get();

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = cmdpool.get();
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer cmdBuffer;
		vkAllocateCommandBuffers(device, &allocInfo, &cmdBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(cmdBuffer, &beginInfo);

		VkImageMemoryBarrier copy_barrier = {};
		copy_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		copy_barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		copy_barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		copy_barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		copy_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		copy_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		copy_barrier.image = _image;
		copy_barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		copy_barrier.subresourceRange.levelCount = 1;
		copy_barrier.subresourceRange.layerCount = 1;
		vkCmdPipelineBarrier(cmdBuffer, VK_PIPELINE_STAGE_HOST_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &copy_barrier);

		VkBufferImageCopy region = {};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;
		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;
		region.imageOffset = {0, 0, 0};
		region.imageExtent = {
			_width,
			_height,
			1
		};
		vkCmdCopyBufferToImage(cmdBuffer, buffer.get(), _image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

		VkImageMemoryBarrier use_barrier = {};
		use_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		use_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		use_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		use_barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		use_barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		use_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		use_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		use_barrier.image = _image;
		use_barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		use_barrier.subresourceRange.levelCount = 1;
		use_barrier.subresourceRange.layerCount = 1;
		vkCmdPipelineBarrier(cmdBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &use_barrier);

		vkEndCommandBuffer(cmdBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &cmdBuffer;

		auto graphicsQueue = Render_Core::get().getQueue().getGraphic();

		vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(graphicsQueue);

		cmdpool.destroy();
	}

	void Image::destroy() noexcept
	{
		if(_sampler != VK_NULL_HANDLE)
			vkDestroySampler(Render_Core::get().getDevice().get(), _sampler, nullptr);
		if(_image_view != VK_NULL_HANDLE)
			vkDestroyImageView(Render_Core::get().getDevice().get(), _image_view, nullptr);

		vkFreeMemory(Render_Core::get().getDevice().get(), _memory, nullptr);
		vkDestroyImage(Render_Core::get().getDevice().get(), _image, nullptr);
	}
}
