/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Image.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 11:59:07 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/23 20:02:25 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <PreCompiled.h>

#include <Renderer/Images/Image.h>
#include <Renderer/Core/RenderCore.h>

namespace mlx
{
	bool IsStencilFormat(VkFormat format)
	{
		switch(format)
		{
			case VK_FORMAT_D32_SFLOAT_S8_UINT:
			case VK_FORMAT_D24_UNORM_S8_UINT:
				return true;

			default: return false;
		}
	}

	bool IsDepthFormat(VkFormat format)
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

	VkFormat BitsToFormat(std::uint32_t bits)
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
				FatalError("Vulkan : unsupported image bit-depth");
				return VK_FORMAT_R8G8B8A8_UNORM;
		}
	}

	VkPipelineStageFlags LayoutToAccessMask(VkImageLayout layout, bool is_destination)
	{
		VkPipelineStageFlags access_mask = 0;

		switch(layout)
		{
			case VK_IMAGE_LAYOUT_UNDEFINED:
				if(is_destination)
					Error("Vulkan : the new layout used in a transition must not be VK_IMAGE_LAYOUT_UNDEFINED");
			break;
			case VK_IMAGE_LAYOUT_GENERAL: access_mask = VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT; break;
			case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL: access_mask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT; break;
			case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL: access_mask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT; break;
			case VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL:
				access_mask = VK_ACCESS_SHADER_READ_BIT; // VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
			break;
			case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL: access_mask = VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_INPUT_ATTACHMENT_READ_BIT; break;
			case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL: access_mask = VK_ACCESS_TRANSFER_READ_BIT; break;
			case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL: access_mask = VK_ACCESS_TRANSFER_WRITE_BIT; break;
			case VK_IMAGE_LAYOUT_PREINITIALIZED:
				if(!is_destination)
					access_mask = VK_ACCESS_HOST_WRITE_BIT;
				else
					Error("Vulkan : the new layout used in a transition must not be VK_IMAGE_LAYOUT_PREINITIALIZED");
			break;
			case VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL: access_mask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT; break;
			case VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL: access_mask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT; break;
			case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR: access_mask = VK_ACCESS_MEMORY_READ_BIT; break;

			default: Error("Vulkan : unexpected image layout"); break;
		}

		return access_mask;
	}

	void Image::Create(std::uint32_t width, std::uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, const char* name, bool dedicated_memory)
	{
		m_width = width;
		m_height = height;
		m_format = format;
		m_tiling = tiling;

		VkImageCreateInfo image_info{};
		image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		image_info.imageType = VK_IMAGE_TYPE_2D;
		image_info.extent.width = width;
		image_info.extent.height = height;
		image_info.extent.depth = 1;
		image_info.mipLevels = 1;
		image_info.arrayLayers = 1;
		image_info.format = format;
		image_info.tiling = tiling;
		image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		image_info.usage = usage;
		image_info.samples = VK_SAMPLE_COUNT_1_BIT;
		image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VmaAllocationCreateInfo alloc_info{};
		alloc_info.usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;
		if(dedicated_memory)
		{
			alloc_info.flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;
			alloc_info.priority = 1.0f;
		}

		m_allocation = RenderCore::Get().GetAllocator().CreateImage(&image_info, &alloc_info, m_image, name);
		#ifdef DEBUG
			m_name = name;
		#endif
	}

	void Image::CreateImageView(VkImageViewType type, VkImageAspectFlags aspect_flags) noexcept
	{
		VkImageViewCreateInfo view_info{};
		view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		view_info.image = m_image;
		view_info.viewType = type;
		view_info.format = m_format;
		view_info.subresourceRange.aspectMask = aspect_flags;
		view_info.subresourceRange.baseMipLevel = 0;
		view_info.subresourceRange.levelCount = 1;
		view_info.subresourceRange.baseArrayLayer = 0;
		view_info.subresourceRange.layerCount = 1;

		VkResult res = vkCreateImageView(RenderCore::Get().GetDevice().Get(), &view_info, nullptr, &m_image_view);
		if(res != VK_SUCCESS)
			FatalError("Vulkan : failed to create an image view, %s", VerbaliseVkResult(res));
		#ifdef DEBUG
		else
			RenderCore::Get().GetLayers().SetDebugUtilsObjectNameEXT(VK_OBJECT_TYPE_IMAGE_VIEW, (std::uint64_t)m_image_view, m_name.c_str());
		#endif
	}

	void Image::CreateSampler() noexcept
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

		VkResult res = vkCreateSampler(RenderCore::Get().GetDevice().Get(), &info, nullptr, &m_sampler);
		if(res != VK_SUCCESS)
			FatalError("Vulkan : failed to create an image sampler, %", VerbaliseVkResult(res));
		#ifdef DEBUG
		else
			RenderCore::Get().GetLayers().SetDebugUtilsObjectNameEXT(VK_OBJECT_TYPE_SAMPLER, (std::uint64_t)m_sampler, m_name.c_str());
		#endif
	}

	void Image::CopyFromBuffer(Buffer& buffer)
	{
		CommandBuffer& cmd = RenderCore::Get().GetSingleTimeCmdBuffer();
		cmd.BeginRecord();

		VkImageLayout layout_save = m_layout;
		TransitionLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &cmd);

		cmd.CopyBufferToImage(buffer, *this);

		TransitionLayout(layout_save, &cmd);

		cmd.EndRecord();
		cmd.SubmitIdle();
	}

	void Image::CopyToBuffer(Buffer& buffer)
	{
		CommandBuffer& cmd = RenderCore::Get().GetSingleTimeCmdBuffer();
		cmd.BeginRecord();

		VkImageLayout layout_save = m_layout;
		TransitionLayout(VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, &cmd);

		cmd.CopyImagetoBuffer(*this, buffer);

		TransitionLayout(layout_save, &cmd);

		cmd.EndRecord();
		cmd.SubmitIdle();
	}

	void Image::TransitionLayout(VkImageLayout new_layout, NonOwningPtr<CommandBuffer> cmd)
	{
		if(new_layout == m_layout)
			return;

		bool single_time = (cmd == nullptr);
		if(single_time)
		{
			cmd = &RenderCore::Get().GetSingleTimeCmdBuffer();
			cmd->BeginRecord();
		}

		cmd->TransitionImageLayout(*this, new_layout);

		if(single_time)
		{
			cmd->EndRecord();
			cmd->SubmitIdle();
		}
		m_layout = new_layout;
	}

	void Image::DestroySampler() noexcept
	{
		if(m_sampler != VK_NULL_HANDLE)
			vkDestroySampler(RenderCore::Get().GetDevice().Get(), m_sampler, nullptr);
		m_sampler = VK_NULL_HANDLE;
	}

	void Image::DestroyImageView() noexcept
	{
		if(m_image_view != VK_NULL_HANDLE)
			vkDestroyImageView(RenderCore::Get().GetDevice().Get(), m_image_view, nullptr);
		m_image_view = VK_NULL_HANDLE;
	}

	void Image::Destroy() noexcept
	{
		DestroySampler();
		DestroyImageView();

		if(m_image != VK_NULL_HANDLE)
			RenderCore::Get().GetAllocator().DestroyImage(m_allocation, m_image);
		m_image = VK_NULL_HANDLE;
	}

	std::uint32_t FormatSize(VkFormat format)
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
