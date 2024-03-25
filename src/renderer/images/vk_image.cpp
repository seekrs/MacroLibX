/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_image.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 11:59:07 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/14 17:28:25 by maldavid         ###   ########.fr       */
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

	VkFormat bitsToFormat(std::uint32_t bits)
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

	void Image::create(std::uint32_t width, std::uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, const char* name, bool dedicated_memory)
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
		#ifdef DEBUG
			_name = name;
		#endif
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

		VkResult res = vkCreateImageView(Render_Core::get().getDevice().get(), &viewInfo, nullptr, &_image_view);
		if(res != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to create an image view, %s", RCore::verbaliseResultVk(res));
		#ifdef DEBUG
		else
			Render_Core::get().getLayers().setDebugUtilsObjectNameEXT(VK_OBJECT_TYPE_IMAGE_VIEW, (std::uint64_t)_image_view, _name.c_str());
		#endif
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

		VkResult res = vkCreateSampler(Render_Core::get().getDevice().get(), &info, nullptr, &_sampler);
		if(res != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to create an image sampler, %s", RCore::verbaliseResultVk(res));
		#ifdef DEBUG
		else
			Render_Core::get().getLayers().setDebugUtilsObjectNameEXT(VK_OBJECT_TYPE_SAMPLER, (std::uint64_t)_sampler, _name.c_str());
		#endif
	}

	void Image::copyFromBuffer(Buffer& buffer)
	{
		CmdBuffer& cmd = Render_Core::get().getSingleTimeCmdBuffer();
		cmd.beginRecord();

		VkImageLayout layout_save = _layout;
		transitionLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &cmd);

		cmd.copyBufferToImage(buffer, *this);

		transitionLayout(layout_save, &cmd);

		cmd.endRecord();
		cmd.submitIdle();
	}

	void Image::copyToBuffer(Buffer& buffer)
	{
		CmdBuffer& cmd = Render_Core::get().getSingleTimeCmdBuffer();
		cmd.beginRecord();

		VkImageLayout layout_save = _layout;
		transitionLayout(VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, &cmd);

		cmd.copyImagetoBuffer(*this, buffer);

		transitionLayout(layout_save, &cmd);

		cmd.endRecord();
		cmd.submitIdle();
	}

	void Image::transitionLayout(VkImageLayout new_layout, CmdBuffer* cmd)
	{
		if(new_layout == _layout)
			return;

		bool singleTime = (cmd == nullptr);
		if(singleTime)
		{
			cmd = &Render_Core::get().getSingleTimeCmdBuffer();
			cmd->beginRecord();
		}

		cmd->transitionImageLayout(*this, new_layout);

		if(singleTime)
		{
			cmd->endRecord();
			cmd->submitIdle();
		}
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

	std::uint32_t formatSize(VkFormat format)
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
