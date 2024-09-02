#include <PreCompiled.h>
#include <Renderer/Image.h>
#include <Maths/Vec4.h>
#include <Renderer/RenderCore.h>

namespace mlx
{
	void Image::Init(ImageType type, std::uint32_t width, std::uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, bool is_multisampled)
	{
		m_type = type;
		m_width = width;
		m_height = height;
		m_format = format;
		m_tiling = tiling;
		m_is_multisampled = is_multisampled;

		VmaAllocationCreateInfo alloc_info{};
		alloc_info.usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;

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
		image_info.samples = (m_is_multisampled ? VK_SAMPLE_COUNT_4_BIT : VK_SAMPLE_COUNT_1_BIT);
		image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		m_allocation = RenderCore::Get().GetAllocator().CreateImage(&image_info, alloc_info, &m_image);
	}

	void Image::CreateImageView(VkImageViewType type, VkImageAspectFlags aspect_flags, int layer_count) noexcept
	{
		m_image_view = kvfCreateImageView(RenderCore::Get().GetDevice(), m_image, m_format, type, aspect_flags, layer_count);
	}

	void Image::CreateSampler() noexcept
	{
		m_sampler = kvfCreateSampler(RenderCore::Get().GetDevice(), VK_FILTER_NEAREST, VK_SAMPLER_ADDRESS_MODE_REPEAT, VK_SAMPLER_MIPMAP_MODE_NEAREST);
	}

	void Image::TransitionLayout(VkImageLayout new_layout, VkCommandBuffer cmd)
	{
		if(new_layout == m_layout)
			return;
		bool is_single_time_cmd_buffer = (cmd == VK_NULL_HANDLE);
		if(is_single_time_cmd_buffer)
			cmd = kvfCreateCommandBuffer(RenderCore::Get().GetDevice());
		KvfImageType kvf_type = KVF_IMAGE_OTHER;
		switch(m_type)
		{
			case ImageType::Color: kvf_type = KVF_IMAGE_COLOR; break;
			default: break;
		}
		kvfTransitionImageLayout(RenderCore::Get().GetDevice(), m_image, kvf_type, cmd, m_format, m_layout, new_layout, is_single_time_cmd_buffer);
		m_layout = new_layout;
	}

	void Image::Clear(VkCommandBuffer cmd, Vec4f color)
	{
		VkImageSubresourceRange subresource_range{};
		subresource_range.baseMipLevel = 0;
		subresource_range.layerCount = (m_type == ImageType::Cube ? 6 : 1);
		subresource_range.levelCount = 1;
		subresource_range.baseArrayLayer = 0;

		if(m_type == ImageType::Color || m_type == ImageType::Cube)
		{
			VkImageLayout old_layout = m_layout;
			TransitionLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, cmd);
			subresource_range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			VkClearColorValue clear_color = VkClearColorValue({ { color.x, color.y, color.z, color.w } });
			vkCmdClearColorImage(cmd, m_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &clear_color, 1, &subresource_range);
			TransitionLayout(old_layout, cmd);
		}
		else if(m_type == ImageType::Depth)
		{
			VkClearDepthStencilValue clear_depth_stencil = { 1.0f, 1 };
			subresource_range.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
			TransitionLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, cmd);
			vkCmdClearDepthStencilImage(cmd, m_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &clear_depth_stencil, 1, &subresource_range);
		}
	}

	void Image::DestroySampler() noexcept
	{
		if(m_sampler != VK_NULL_HANDLE)
			kvfDestroySampler(RenderCore::Get().GetDevice(), m_sampler);
		m_sampler = VK_NULL_HANDLE;
	}

	void Image::DestroyImageView() noexcept
	{
		if(m_image_view != VK_NULL_HANDLE)
			kvfDestroyImageView(RenderCore::Get().GetDevice(), m_image_view);
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
}
