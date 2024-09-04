#include <PreCompiled.h>
#include <Renderer/Image.h>
#include <Maths/Vec4.h>
#include <Renderer/RenderCore.h>

namespace mlx
{
	void Image::Init(ImageType type, std::uint32_t width, std::uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, bool is_multisampled)
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
		{
			cmd = kvfCreateCommandBuffer(RenderCore::Get().GetDevice());
			kvfBeginCommandBuffer(cmd, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
		}
		KvfImageType kvf_type = KVF_IMAGE_OTHER;
		switch(m_type)
		{
			case ImageType::Color: kvf_type = KVF_IMAGE_COLOR; break;
			case ImageType::Depth: kvf_type = KVF_IMAGE_DEPTH; break;

			default: break;
		}
		kvfTransitionImageLayout(RenderCore::Get().GetDevice(), m_image, kvf_type, cmd, m_format, m_layout, new_layout, is_single_time_cmd_buffer);
		m_layout = new_layout;
		if(is_single_time_cmd_buffer)
		{
			vkEndCommandBuffer(cmd);
			VkFence fence = kvfCreateFence(RenderCore::Get().GetDevice());
			kvfSubmitSingleTimeCommandBuffer(RenderCore::Get().GetDevice(), cmd, KVF_GRAPHICS_QUEUE, fence);
			kvfDestroyFence(RenderCore::Get().GetDevice(), fence);
		}
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

	void Texture::SetPixel(int x, int y, std::uint32_t color) noexcept
	{
		MLX_PROFILE_FUNCTION();
		if(x < 0 || y < 0 || static_cast<std::uint32_t>(x) > m_width || static_cast<std::uint32_t>(y) > m_height)
			return;
		if(!m_staging_buffer.has_value())
			OpenCPUBuffer();
		m_cpu_buffer[(y * m_width) + x] = color;
		m_has_been_modified = true;
	}

	int GetPixel(int x, int y) noexcept
	{
		MLX_PROFILE_FUNCTION();
		if(x < 0 || y < 0 || static_cast<std::uint32_t>(x) > getWidth() || static_cast<std::uint32_t>(y) > getHeight())
			return 0;
		if(!m_staging_buffer.has_value())
			OpenCPUBuffer();
		std::uint32_t color = m_cpu_buffer[(y * m_width) + x];
		std::uint8_t* bytes = reinterpret_cast<std::uint8_t*>(&color);
		std::uint8_t tmp = bytes[0];
		bytes[0] = bytes[2];
		bytes[2] = tmp;
		return *reinterpret_cast<int*>(bytes);
	}

	void Update(VkCommandBuffer cmd) const
	{
		if(!m_has_been_modified)
			return;
		std::memcpy(m_staging_buffer.GetMap(), m_cpu_buffer.data(), m_cpu_buffer.size() * kvfGetFormatSize(m_format));

		VkImageLayout old_layout = m_layout;
		VkCommandBuffer cmd = kvfCreateCommandBuffer(RenderCore::Get().GetDevice());
		kvfBeginCommandBuffer(cmd, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
		TransitionLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, cmd);
		kvfCopyBufferToImage(cmd, Image::Get(), staging_buffer.Get(), staging_buffer.GetOffset(), VK_IMAGE_ASPECT_COLOR_BIT, { width, height, 1 });
		TransitionLayout(old_layout, cmd);
		vkEndCommandBuffer(cmd);
		VkFence fence = kvfCreateFence(RenderCore::Get().GetDevice());
		kvfSubmitSingleTimeCommandBuffer(RenderCore::Get().GetDevice(), cmd, KVF_GRAPHICS_QUEUE, fence);
		kvfDestroyFence(RenderCore::Get().GetDevice(), fence);

		m_has_been_modified = false;
	}

	void Texture::OpenCPUBuffer()
	{
		MLX_PROFILE_FUNCTION();
		if(m_staging_buffer.has_value())
			return;
		DebugLog("Texture : enabling CPU mapping");
		m_staging_buffer.emplace();
		std::size_t size = m_width * m_height * kvfGetFormatSize(m_format);
		m_staging_buffer->Init(BufferType::Staging, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, {});

		VkImageLayout old_layout = m_layout;
		VkCommandBuffer cmd = kvfCreateCommandBuffer(RenderCore::Get().GetDevice());
		kvfBeginCommandBuffer(cmd, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
		TransitionLayout(VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, cmd);
		kvfImageToBuffer(cmd, m_image, m_staging_buffer.Get(), m_staging_buffer.GetOffset(), VK_IMAGE_ASPECT_COLOR_BIT, { m_width, m_height, 1 });
		TransitionLayout(old_layout, cmd);
		vkEndCommandBuffer(cmd);
		VkFence fence = kvfCreateFence(RenderCore::Get().GetDevice());
		kvfSubmitSingleTimeCommandBuffer(RenderCore::Get().GetDevice(), cmd, KVF_GRAPHICS_QUEUE, fence);
		kvfDestroyFence(RenderCore::Get().GetDevice(), fence);

		m_cpu_buffer.resize(m_width * m_height);
		std::memcpy(m_cpu_buffer.data(), m_staging_buffer.GetMap(), m_cpu_buffer.size());
	}

	Texture* StbTextureLoad(const std::filesystem::path& file, int* w, int* h)
	{
		MLX_PROFILE_FUNCTION();
		std::string filename = file.string();

		if(!std::filesystem::exists(file))
		{
			Error("Image : file not found %", file);
			return nullptr;
		}
		if(stbi_is_hdr(filename.c_str()))
		{
			Error("Texture : unsupported image format %", file);
			return nullptr;
		}
		int dummy_w;
		int dummy_h;
		int channels;
		std::uint8_t* data = stbi_load(filename.c_str(), (w == nullptr ? &dummy_w : w), (h == nullptr ? &dummy_h : h), &channels, 4);
		CPUBuffer buffer((w == nullptr ? dummy_w : *w) * (h == nullptr ? dummy_h : *h) * 4);
		std::memcpy(buffer.GetData(), data, buffer.GetSize());
		Texture* texture;

		try { texture = new Texture(buffer, (w == nullptr ? dummy_w : *w), (h == nullptr ? dummy_h : *h)); }
		catch(...) { return NULL; }
		
		stbi_image_free(data);
		return texture;
	}
}
