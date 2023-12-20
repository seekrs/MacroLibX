/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_image.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 11:59:07 by maldavid          #+#    #+#             */
/*   Updated: 2023/12/16 17:10:33 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vk_image.h"
#include <renderer/core/render_core.h>
#include <renderer/buffers/vk_buffer.h>
#include <renderer/command/vk_cmd_pool.h>
#include <renderer/core/vk_fence.h>

namespace mlx
{
	bool isStencilFormat(VkFormat format)
	{
		switch(format)
		{
			case VK_FORMAT_D32_SFLOAT_S8_UINT:
			case VK_FORMAT_D24_UNORM_S8_UINT:
				return true;

			default: return false;
		}
	}

	bool isDepthFormat(VkFormat format)
	{
		switch(format)
		{
			case VK_FORMAT_D16_UNORM:
			case VK_FORMAT_D32_SFLOAT:
			case VK_FORMAT_D32_SFLOAT_S8_UINT:
			case VK_FORMAT_D24_UNORM_S8_UINT:
			case VK_FORMAT_D16_UNORM_S8_UINT:
				return true;

			default: return false;
		}
	}

	VkFormat bitsToFormat(uint32_t bits)
	{
		switch(bits)
		{
			case 8:   return VK_FORMAT_R8_UNORM;
			case 16:  return VK_FORMAT_R8G8_UNORM;
			case 24:  return VK_FORMAT_R8G8B8_UNORM;
			case 32:  return VK_FORMAT_R8G8B8A8_UNORM;
			case 48:  return VK_FORMAT_R16G16B16_SFLOAT;
			case 64:  return VK_FORMAT_R16G16B16A16_SFLOAT;
			case 96:  return VK_FORMAT_R32G32B32_SFLOAT;
			case 128: return VK_FORMAT_R32G32B32A32_SFLOAT;

			default:
				core::error::report(e_kind::fatal_error, "Vulkan : unsupported image bit-depth");
				return VK_FORMAT_R8G8B8A8_UNORM;
		}
	}

	VkPipelineStageFlags layoutToAccessMask(VkImageLayout layout, bool isDestination)
	{
		VkPipelineStageFlags accessMask = 0;

		switch(layout)
		{
			case VK_IMAGE_LAYOUT_UNDEFINED:
				if(isDestination)
					core::error::report(e_kind::error, "Vulkan : the new layout used in a transition must not be VK_IMAGE_LAYOUT_UNDEFINED");
			break;
			case VK_IMAGE_LAYOUT_GENERAL: accessMask = VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT; break;
			case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL: accessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT; break;
			case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL: accessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT; break;
			case VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL:
				accessMask = VK_ACCESS_SHADER_READ_BIT; // VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
			break;
			case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL: accessMask = VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_INPUT_ATTACHMENT_READ_BIT; break;
			case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL: accessMask = VK_ACCESS_TRANSFER_READ_BIT; break;
			case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL: accessMask = VK_ACCESS_TRANSFER_WRITE_BIT; break;
			case VK_IMAGE_LAYOUT_PREINITIALIZED:
				if(!isDestination)
					accessMask = VK_ACCESS_HOST_WRITE_BIT;
				else
					core::error::report(e_kind::error, "Vulkan : the new layout used in a transition must not be VK_IMAGE_LAYOUT_PREINITIALIZED");
			break;
			case VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL: accessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT; break;
			case VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL: accessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT; break;
			case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR: accessMask = VK_ACCESS_MEMORY_READ_BIT; break;

			default: core::error::report(e_kind::error, "Vulkan : unexpected image layout"); break;
		}

		return accessMask;
	}

	VkPipelineStageFlags accessFlagsToPipelineStage(VkAccessFlags accessFlags, VkPipelineStageFlags stageFlags)
	{
		VkPipelineStageFlags stages = 0;

		while(accessFlags != 0)
		{
			VkAccessFlagBits AccessFlag = static_cast<VkAccessFlagBits>(accessFlags & (~(accessFlags - 1)));
			if(AccessFlag == 0 || (AccessFlag & (AccessFlag - 1)) != 0)
				core::error::report(e_kind::fatal_error, "Vulkan : an error has been caught during access flag to pipeline stage operation");
			accessFlags &= ~AccessFlag;

			switch(AccessFlag)
			{
				case VK_ACCESS_INDIRECT_COMMAND_READ_BIT: stages |= VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT; break;
				case VK_ACCESS_INDEX_READ_BIT: stages |= VK_PIPELINE_STAGE_VERTEX_INPUT_BIT; break;
				case VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT: stages |= VK_PIPELINE_STAGE_VERTEX_INPUT_BIT; break;
				case VK_ACCESS_UNIFORM_READ_BIT: stages |= stageFlags | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT; break;
				case VK_ACCESS_INPUT_ATTACHMENT_READ_BIT: stages |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT; break;
				case VK_ACCESS_SHADER_READ_BIT: stages |= stageFlags | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT; break;
				case VK_ACCESS_SHADER_WRITE_BIT: stages |= stageFlags | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT; break;
				case VK_ACCESS_COLOR_ATTACHMENT_READ_BIT: stages |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT; break;
				case VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT: stages |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT; break;
				case VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT: stages |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT; break;
				case VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT: stages |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT; break;
				case VK_ACCESS_TRANSFER_READ_BIT: stages |= VK_PIPELINE_STAGE_TRANSFER_BIT; break;
				case VK_ACCESS_TRANSFER_WRITE_BIT: stages |= VK_PIPELINE_STAGE_TRANSFER_BIT; break;
				case VK_ACCESS_HOST_READ_BIT: stages |= VK_PIPELINE_STAGE_HOST_BIT; break;
				case VK_ACCESS_HOST_WRITE_BIT: stages |= VK_PIPELINE_STAGE_HOST_BIT; break;
				case VK_ACCESS_MEMORY_READ_BIT: break;
				case VK_ACCESS_MEMORY_WRITE_BIT: break;

				default: core::error::report(e_kind::error, "Vulkan : unknown access flag"); break;
			}
		}
		return stages;
	}

	void Image::create(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, const char* name, bool dedicated_memory)
	{
		_width = width;
		_height = height;
		_format = format;
		_tiling = tiling;

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

		VmaAllocationCreateInfo alloc_info{};
		alloc_info.usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;
		if(dedicated_memory)
		{
			alloc_info.flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
			alloc_info.priority = 1.0f;
		}

		_allocation = Render_Core::get().getAllocator().createImage(&imageInfo, &alloc_info, _image, name);
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
		VkSamplerCreateInfo info{};
		info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		info.magFilter = VK_FILTER_NEAREST;
		info.minFilter = VK_FILTER_NEAREST;
		info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
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

	void Image::copyFromBuffer(Buffer& buffer)
	{
		CmdBuffer& cmd = Render_Core::get().getSingleTimeCmdBuffer();
		cmd.beginRecord();

		VkImageMemoryBarrier copy_barrier{};
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
		vkCmdPipelineBarrier(cmd.get(), VK_PIPELINE_STAGE_HOST_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &copy_barrier);

		VkBufferImageCopy region{};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;
		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;
		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = { _width, _height, 1 };

		vkCmdCopyBufferToImage(cmd.get(), buffer.get(), _image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

		VkImageMemoryBarrier use_barrier{};
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
		vkCmdPipelineBarrier(cmd.get(), VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &use_barrier);

		cmd.endRecord();
		cmd.submitIdle();
	}

	void Image::copyToBuffer(Buffer& buffer)
	{
		CmdBuffer& cmd = Render_Core::get().getSingleTimeCmdBuffer();
		cmd.beginRecord();

		VkImageMemoryBarrier copy_barrier{};
		copy_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		copy_barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		copy_barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		copy_barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		copy_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		copy_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		copy_barrier.image = _image;
		copy_barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		copy_barrier.subresourceRange.levelCount = 1;
		copy_barrier.subresourceRange.layerCount = 1;
		vkCmdPipelineBarrier(cmd.get(), VK_PIPELINE_STAGE_HOST_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &copy_barrier);

		VkBufferImageCopy region{};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;
		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;
		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = { _width, _height, 1 };

		vkCmdCopyImageToBuffer(cmd.get(), _image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, buffer.get(), 1, &region);

		VkImageMemoryBarrier use_barrier{};
		use_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		use_barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		use_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		use_barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		use_barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		use_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		use_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		use_barrier.image = _image;
		use_barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		use_barrier.subresourceRange.levelCount = 1;
		use_barrier.subresourceRange.layerCount = 1;
		vkCmdPipelineBarrier(cmd.get(), VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &use_barrier);

		cmd.endRecord();
		cmd.submitIdle();
	}

	void Image::transitionLayout(VkImageLayout new_layout)
	{
		if(new_layout == _layout)
			return;

		CmdBuffer& cmd = Render_Core::get().getSingleTimeCmdBuffer();
		cmd.beginRecord();

		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = _layout;
		barrier.newLayout = new_layout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = _image;
		barrier.subresourceRange.aspectMask = isDepthFormat(_format) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;
		barrier.srcAccessMask = layoutToAccessMask(_layout, false);
		barrier.dstAccessMask = layoutToAccessMask(new_layout, true);
		if(isStencilFormat(_format))
			barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;

		VkPipelineStageFlags sourceStage = 0;
		if(barrier.oldLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
			sourceStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		else if(barrier.srcAccessMask != 0)
			sourceStage = accessFlagsToPipelineStage(barrier.srcAccessMask, VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT);
		else
			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

		VkPipelineStageFlags destinationStage = 0;
		if(barrier.newLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
			destinationStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		else if(barrier.dstAccessMask != 0)
			destinationStage = accessFlagsToPipelineStage(barrier.dstAccessMask, VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT);
		else
			destinationStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;

		vkCmdPipelineBarrier(cmd.get(), sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

		cmd.endRecord();
		cmd.submitIdle();
		_layout = new_layout;
	}

	void Image::destroySampler() noexcept
	{
		if(_sampler != VK_NULL_HANDLE)
			vkDestroySampler(Render_Core::get().getDevice().get(), _sampler, nullptr);
		_sampler = VK_NULL_HANDLE;
	}

	void Image::destroyImageView() noexcept
	{
		if(_image_view != VK_NULL_HANDLE)
			vkDestroyImageView(Render_Core::get().getDevice().get(), _image_view, nullptr);
		_image_view = VK_NULL_HANDLE;
	}

	void Image::destroy() noexcept
	{
		destroySampler();
		destroyImageView();

		if(_image != VK_NULL_HANDLE)
			Render_Core::get().getAllocator().destroyImage(_allocation, _image);
		_image = VK_NULL_HANDLE;
	}

	uint32_t formatSize(VkFormat format)
	{
		switch(format)
		{
			case VK_FORMAT_UNDEFINED: return 0;
			case VK_FORMAT_R4G4_UNORM_PACK8: return 1;
			case VK_FORMAT_R4G4B4A4_UNORM_PACK16: return 2;
			case VK_FORMAT_B4G4R4A4_UNORM_PACK16: return 2;
			case VK_FORMAT_R5G6B5_UNORM_PACK16: return 2;
			case VK_FORMAT_B5G6R5_UNORM_PACK16: return 2;
			case VK_FORMAT_R5G5B5A1_UNORM_PACK16: return 2;
			case VK_FORMAT_B5G5R5A1_UNORM_PACK16: return 2;
			case VK_FORMAT_A1R5G5B5_UNORM_PACK16: return 2;
			case VK_FORMAT_R8_UNORM: return 1;
			case VK_FORMAT_R8_SNORM: return 1;
			case VK_FORMAT_R8_USCALED: return 1;
			case VK_FORMAT_R8_SSCALED: return 1;
			case VK_FORMAT_R8_UINT: return 1;
			case VK_FORMAT_R8_SINT: return 1;
			case VK_FORMAT_R8_SRGB: return 1;
			case VK_FORMAT_R8G8_UNORM: return 2;
			case VK_FORMAT_R8G8_SNORM: return 2;
			case VK_FORMAT_R8G8_USCALED: return 2;
			case VK_FORMAT_R8G8_SSCALED: return 2;
			case VK_FORMAT_R8G8_UINT: return 2;
			case VK_FORMAT_R8G8_SINT: return 2;
			case VK_FORMAT_R8G8_SRGB: return 2;
			case VK_FORMAT_R8G8B8_UNORM: return 3;
			case VK_FORMAT_R8G8B8_SNORM: return 3;
			case VK_FORMAT_R8G8B8_USCALED: return 3;
			case VK_FORMAT_R8G8B8_SSCALED: return 3;
			case VK_FORMAT_R8G8B8_UINT: return 3;
			case VK_FORMAT_R8G8B8_SINT: return 3;
			case VK_FORMAT_R8G8B8_SRGB: return 3;
			case VK_FORMAT_B8G8R8_UNORM: return 3;
			case VK_FORMAT_B8G8R8_SNORM: return 3;
			case VK_FORMAT_B8G8R8_USCALED: return 3;
			case VK_FORMAT_B8G8R8_SSCALED: return 3;
			case VK_FORMAT_B8G8R8_UINT: return 3;
			case VK_FORMAT_B8G8R8_SINT: return 3;
			case VK_FORMAT_B8G8R8_SRGB: return 3;
			case VK_FORMAT_R8G8B8A8_UNORM: return 4;
			case VK_FORMAT_R8G8B8A8_SNORM: return 4;
			case VK_FORMAT_R8G8B8A8_USCALED: return 4;
			case VK_FORMAT_R8G8B8A8_SSCALED: return 4;
			case VK_FORMAT_R8G8B8A8_UINT: return 4;
			case VK_FORMAT_R8G8B8A8_SINT: return 4;
			case VK_FORMAT_R8G8B8A8_SRGB: return 4;
			case VK_FORMAT_B8G8R8A8_UNORM: return 4;
			case VK_FORMAT_B8G8R8A8_SNORM: return 4;
			case VK_FORMAT_B8G8R8A8_USCALED: return 4;
			case VK_FORMAT_B8G8R8A8_SSCALED: return 4;
			case VK_FORMAT_B8G8R8A8_UINT: return 4;
			case VK_FORMAT_B8G8R8A8_SINT: return 4;
			case VK_FORMAT_B8G8R8A8_SRGB: return 4;
			case VK_FORMAT_A8B8G8R8_UNORM_PACK32: return 4;
			case VK_FORMAT_A8B8G8R8_SNORM_PACK32: return 4;
			case VK_FORMAT_A8B8G8R8_USCALED_PACK32: return 4;
			case VK_FORMAT_A8B8G8R8_SSCALED_PACK32: return 4;
			case VK_FORMAT_A8B8G8R8_UINT_PACK32: return 4;
			case VK_FORMAT_A8B8G8R8_SINT_PACK32: return 4;
			case VK_FORMAT_A8B8G8R8_SRGB_PACK32: return 4;
			case VK_FORMAT_A2R10G10B10_UNORM_PACK32: return 4;
			case VK_FORMAT_A2R10G10B10_SNORM_PACK32: return 4;
			case VK_FORMAT_A2R10G10B10_USCALED_PACK32: return 4;
			case VK_FORMAT_A2R10G10B10_SSCALED_PACK32: return 4;
			case VK_FORMAT_A2R10G10B10_UINT_PACK32: return 4;
			case VK_FORMAT_A2R10G10B10_SINT_PACK32: return 4;
			case VK_FORMAT_A2B10G10R10_UNORM_PACK32: return 4;
			case VK_FORMAT_A2B10G10R10_SNORM_PACK32: return 4;
			case VK_FORMAT_A2B10G10R10_USCALED_PACK32: return 4;
			case VK_FORMAT_A2B10G10R10_SSCALED_PACK32: return 4;
			case VK_FORMAT_A2B10G10R10_UINT_PACK32: return 4;
			case VK_FORMAT_A2B10G10R10_SINT_PACK32: return 4;
			case VK_FORMAT_R16_UNORM: return 2;
			case VK_FORMAT_R16_SNORM: return 2;
			case VK_FORMAT_R16_USCALED: return 2;
			case VK_FORMAT_R16_SSCALED: return 2;
			case VK_FORMAT_R16_UINT: return 2;
			case VK_FORMAT_R16_SINT: return 2;
			case VK_FORMAT_R16_SFLOAT: return 2;
			case VK_FORMAT_R16G16_UNORM: return 4;
			case VK_FORMAT_R16G16_SNORM: return 4;
			case VK_FORMAT_R16G16_USCALED: return 4;
			case VK_FORMAT_R16G16_SSCALED: return 4;
			case VK_FORMAT_R16G16_UINT: return 4;
			case VK_FORMAT_R16G16_SINT: return 4;
			case VK_FORMAT_R16G16_SFLOAT: return 4;
			case VK_FORMAT_R16G16B16_UNORM: return 6;
			case VK_FORMAT_R16G16B16_SNORM: return 6;
			case VK_FORMAT_R16G16B16_USCALED: return 6;
			case VK_FORMAT_R16G16B16_SSCALED: return 6;
			case VK_FORMAT_R16G16B16_UINT: return 6;
			case VK_FORMAT_R16G16B16_SINT: return 6;
			case VK_FORMAT_R16G16B16_SFLOAT: return 6;
			case VK_FORMAT_R16G16B16A16_UNORM: return 8;
			case VK_FORMAT_R16G16B16A16_SNORM: return 8;
			case VK_FORMAT_R16G16B16A16_USCALED: return 8;
			case VK_FORMAT_R16G16B16A16_SSCALED: return 8;
			case VK_FORMAT_R16G16B16A16_UINT: return 8;
			case VK_FORMAT_R16G16B16A16_SINT: return 8;
			case VK_FORMAT_R16G16B16A16_SFLOAT: return 8;
			case VK_FORMAT_R32_UINT: return 4;
			case VK_FORMAT_R32_SINT: return 4;
			case VK_FORMAT_R32_SFLOAT: return 4;
			case VK_FORMAT_R32G32_UINT: return 8;
			case VK_FORMAT_R32G32_SINT: return 8;
			case VK_FORMAT_R32G32_SFLOAT: return 8;
			case VK_FORMAT_R32G32B32_UINT: return 12;
			case VK_FORMAT_R32G32B32_SINT: return 12;
			case VK_FORMAT_R32G32B32_SFLOAT: return 12;
			case VK_FORMAT_R32G32B32A32_UINT: return 16;
			case VK_FORMAT_R32G32B32A32_SINT: return 16;
			case VK_FORMAT_R32G32B32A32_SFLOAT: return 16;
			case VK_FORMAT_R64_UINT: return 8;
			case VK_FORMAT_R64_SINT: return 8;
			case VK_FORMAT_R64_SFLOAT: return 8;
			case VK_FORMAT_R64G64_UINT: return 16;
			case VK_FORMAT_R64G64_SINT: return 16;
			case VK_FORMAT_R64G64_SFLOAT: return 16;
			case VK_FORMAT_R64G64B64_UINT: return 24;
			case VK_FORMAT_R64G64B64_SINT: return 24;
			case VK_FORMAT_R64G64B64_SFLOAT: return 24;
			case VK_FORMAT_R64G64B64A64_UINT: return 32;
			case VK_FORMAT_R64G64B64A64_SINT: return 32;
			case VK_FORMAT_R64G64B64A64_SFLOAT: return 32;
			case VK_FORMAT_B10G11R11_UFLOAT_PACK32: return 4;
			case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32: return 4;

			default: return 0;
		}
	}
}
