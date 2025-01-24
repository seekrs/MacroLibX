#include <PreCompiled.h>
#include <Renderer/Image.h>
#include <Maths/Vec4.h>
#include <Maths/Vec2.h>
#include <Renderer/RenderCore.h>
#include <Utils/CallOnExit.h>
#include <Core/Memory.h>

#define STB_IMAGE_IMPLEMENTATION

#define STBI_ASSERT(x) (mlx::Assert(x, "internal stb assertion " #x))
#define STBI_MALLOC(x) (mlx::MemManager::Malloc(x))
#define STBI_REALLOC(p, x) (mlx::MemManager::Realloc(p, x))
#define STBI_FREE(x) (mlx::MemManager::Free(x))

#ifdef MLX_COMPILER_GCC
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wstringop-overflow"
		#include <stb_image.h>
	#pragma GCC diagnostic pop
#else
	#include <stb_image.h>
#endif

#ifdef IMAGE_OPTIMIZED
	#define TILING VK_IMAGE_TILING_OPTIMAL
#else
	#define TILING VK_IMAGE_TILING_LINEAR
#endif

namespace mlx
{
	mlx_color ReverseColor(mlx_color color)
	{
		mlx_color reversed_color;
		reversed_color.r = color.a;
		reversed_color.g = color.b;
		reversed_color.b = color.g;
		reversed_color.a = color.r;
		return reversed_color;
	}

	void Image::Init(ImageType type, std::uint32_t width, std::uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, bool is_multisampled, [[maybe_unused]] std::string_view debug_name)
	{
		MLX_PROFILE_FUNCTION();
		Verify(width > 0 && height > 0, "width or height cannot be null");

		m_type = type;
		m_width = width;
		m_height = height;
		m_format = format;
		m_tiling = tiling;
		m_is_multisampled = is_multisampled;
		#ifdef DEBUG
			m_debug_name = std::move(debug_name);
		#endif

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
		#ifdef DEBUG
			m_allocation = RenderCore::Get().GetAllocator().CreateImage(&image_info, &alloc_info, m_image, m_debug_name.c_str());
		#else
			m_allocation = RenderCore::Get().GetAllocator().CreateImage(&image_info, &alloc_info, m_image, nullptr);
		#endif
	}

	void Image::CreateImageView(VkImageViewType type, VkImageAspectFlags aspect_flags, int layer_count) noexcept
	{
		MLX_PROFILE_FUNCTION();
		m_image_view = kvfCreateImageView(RenderCore::Get().GetDevice(), m_image, m_format, type, aspect_flags, layer_count);
		#ifdef MLX_HAS_DEBUG_UTILS_FUNCTIONS
			VkDebugUtilsObjectNameInfoEXT name_info{};
			name_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
			name_info.objectType = VK_OBJECT_TYPE_IMAGE_VIEW;
			name_info.objectHandle = reinterpret_cast<std::uint64_t>(m_image_view);
			name_info.pObjectName = m_debug_name.c_str();
			RenderCore::Get().vkSetDebugUtilsObjectNameEXT(RenderCore::Get().GetDevice(), &name_info);
		#endif
	}

	void Image::CreateSampler() noexcept
	{
		MLX_PROFILE_FUNCTION();
		m_sampler = kvfCreateSampler(RenderCore::Get().GetDevice(), VK_FILTER_NEAREST, VK_SAMPLER_ADDRESS_MODE_REPEAT, VK_SAMPLER_MIPMAP_MODE_NEAREST);
	}

	void Image::TransitionLayout(VkImageLayout new_layout, VkCommandBuffer cmd)
	{
		MLX_PROFILE_FUNCTION();
		if(new_layout == m_layout)
			return;
		bool is_single_time_cmd_buffer = (cmd == VK_NULL_HANDLE);
		if(is_single_time_cmd_buffer)
			cmd = kvfCreateCommandBuffer(RenderCore::Get().GetDevice());
		kvfTransitionImageLayout(RenderCore::Get().GetDevice(), m_image, KVF_IMAGE_COLOR, cmd, m_format, m_layout, new_layout, is_single_time_cmd_buffer);
		if(is_single_time_cmd_buffer)
			kvfDestroyCommandBuffer(RenderCore::Get().GetDevice(), cmd);
		m_layout = new_layout;
	}

	void Image::Clear(VkCommandBuffer cmd, Vec4f color)
	{
		MLX_PROFILE_FUNCTION();
		VkImageSubresourceRange subresource_range{};
		subresource_range.baseMipLevel = 0;
		subresource_range.layerCount = 1;
		subresource_range.levelCount = 1;
		subresource_range.baseArrayLayer = 0;

		bool is_single_time_cmd_buffer = (cmd == VK_NULL_HANDLE);
		if(is_single_time_cmd_buffer)
		{
			cmd = kvfCreateCommandBuffer(RenderCore::Get().GetDevice());
			kvfBeginCommandBuffer(cmd, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
		}

		VkImageLayout old_layout = m_layout;
		TransitionLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, cmd);
		subresource_range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		VkClearColorValue clear_color = VkClearColorValue({ { color.x, color.y, color.z, color.w } });
		RenderCore::Get().vkCmdClearColorImage(cmd, m_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &clear_color, 1, &subresource_range);
		TransitionLayout(old_layout, cmd);

		if(is_single_time_cmd_buffer)
		{
			kvfEndCommandBuffer(cmd);
			VkFence fence = kvfCreateFence(RenderCore::Get().GetDevice());
			kvfSubmitSingleTimeCommandBuffer(RenderCore::Get().GetDevice(), cmd, KVF_GRAPHICS_QUEUE, fence);
			kvfDestroyFence(RenderCore::Get().GetDevice(), fence);
			kvfDestroyCommandBuffer(RenderCore::Get().GetDevice(), cmd);
		}
	}

	void Image::DestroySampler() noexcept
	{
		MLX_PROFILE_FUNCTION();
		if(m_sampler != VK_NULL_HANDLE)
			kvfDestroySampler(RenderCore::Get().GetDevice(), m_sampler);
		m_sampler = VK_NULL_HANDLE;
	}

	void Image::DestroyImageView() noexcept
	{
		MLX_PROFILE_FUNCTION();
		if(m_image_view != VK_NULL_HANDLE)
			kvfDestroyImageView(RenderCore::Get().GetDevice(), m_image_view);
		m_image_view = VK_NULL_HANDLE;
	}

	void Image::Destroy() noexcept
	{
		MLX_PROFILE_FUNCTION();
		DestroySampler();
		DestroyImageView();

		if(m_image != VK_NULL_HANDLE)
		{
			#ifdef DEBUG
				RenderCore::Get().GetAllocator().DestroyImage(m_allocation, m_image, m_debug_name.c_str());
			#else
				RenderCore::Get().GetAllocator().DestroyImage(m_allocation, m_image, nullptr);
			#endif
		}
		m_image = VK_NULL_HANDLE;
		m_layout = VK_IMAGE_LAYOUT_UNDEFINED;
		m_width = 0;
		m_height = 0;
		m_is_multisampled = false;
	}

	void Texture::Init(CPUBuffer pixels, std::uint32_t width, std::uint32_t height, VkFormat format, bool is_multisampled, [[maybe_unused]] std::string_view debug_name)
	{
		MLX_PROFILE_FUNCTION();
		Image::Init(ImageType::Color, width, height, format, TILING, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, is_multisampled, std::move(debug_name));
		Image::CreateImageView(VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_COLOR_BIT);
		Image::CreateSampler();
		if(pixels)
		{
			GPUBuffer staging_buffer;
			std::size_t size = width * height * kvfFormatSize(format);
			staging_buffer.Init(BufferType::Staging, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, std::move(pixels), debug_name);
			VkCommandBuffer cmd = kvfCreateCommandBuffer(RenderCore::Get().GetDevice());
			kvfBeginCommandBuffer(cmd, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
			TransitionLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, cmd);
			kvfCopyBufferToImage(cmd, Image::Get(), staging_buffer.Get(), staging_buffer.GetOffset(), VK_IMAGE_ASPECT_COLOR_BIT, { width, height, 1 });
			kvfEndCommandBuffer(cmd);
			VkFence fence = kvfCreateFence(RenderCore::Get().GetDevice());
			kvfSubmitSingleTimeCommandBuffer(RenderCore::Get().GetDevice(), cmd, KVF_GRAPHICS_QUEUE, fence);
			kvfDestroyFence(RenderCore::Get().GetDevice(), fence);
			kvfDestroyCommandBuffer(RenderCore::Get().GetDevice(), cmd);
			staging_buffer.Destroy();
		}
		TransitionLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	}

	void Texture::Destroy() noexcept
	{
		if(m_staging_buffer.has_value())
			m_staging_buffer->Destroy();
		Image::Destroy();
	}

	void Texture::SetPixel(int x, int y, mlx_color color) noexcept
	{
		MLX_PROFILE_FUNCTION();
		if(x < 0 || y < 0 || static_cast<std::uint32_t>(x) > m_width || static_cast<std::uint32_t>(y) > m_height)
			return;
		if(!m_staging_buffer.has_value())
			OpenCPUBuffer();
		if constexpr(std::endian::native == std::endian::little)
			m_staging_buffer->GetMap<mlx_color*>()[(y * m_width) + x] = ReverseColor(color);
		else
			m_staging_buffer->GetMap<mlx_color*>()[(y * m_width) + x] = color;
		m_has_been_modified = true;
	}

	void Texture::SetRegion(int x, int y, int w, int h, mlx_color* pixels) noexcept
	{
		MLX_PROFILE_FUNCTION();
		if(x < 0 || y < 0 || static_cast<std::uint32_t>(x) > m_width || static_cast<std::uint32_t>(y) > m_height)
			return;
		if(w < 0 || h < 0)
			return;
		if(!m_staging_buffer.has_value())
			OpenCPUBuffer();
		for(std::uint32_t i = 0, moving_x = x, moving_y = y;; i++, moving_x++)
		{
			if(moving_x >= static_cast<std::uint32_t>(x + w) || moving_x >= m_width)
			{
				moving_x = x;
				if(moving_y >= static_cast<std::uint32_t>(y + h) || moving_y >= m_height)
					break;
				moving_y++;
			}
			if constexpr(std::endian::native == std::endian::little)
				m_staging_buffer->GetMap<mlx_color*>()[(moving_y * m_width) + moving_x] = ReverseColor(pixels[i]);
			else
				m_staging_buffer->GetMap<mlx_color*>()[(moving_y * m_width) + moving_x] = pixels[i];
		}
		m_has_been_modified = true;
	}

	void Texture::SetLinearRegion(int x, int y, std::size_t len, mlx_color* pixels) noexcept
	{
		MLX_PROFILE_FUNCTION();
		if(x < 0 || y < 0 || static_cast<std::uint32_t>(x) > m_width || static_cast<std::uint32_t>(y) > m_height)
			return;
		if(!m_staging_buffer.has_value())
			OpenCPUBuffer();
		if constexpr(std::endian::native == std::endian::little)
		{
			for(std::size_t i = 0; i < len && (y * m_width) + x + i < m_width * m_height; i++)
				m_staging_buffer->GetMap<mlx_color*>()[(y * m_width) + x + i] = ReverseColor(pixels[i]);
		}
		else
		{
			std::size_t len_guard;
			if((y * m_width + x + len) < m_width * m_height)
				len_guard = len;
			else
				len_guard = len - (m_width * m_height - (y * m_width + x + len));
			std::memcpy(&m_staging_buffer->GetMap<mlx_color*>()[(y * m_width) + x], pixels, len_guard);
		}
		m_has_been_modified = true;
	}

	mlx_color Texture::GetPixel(int x, int y) noexcept
	{
		MLX_PROFILE_FUNCTION();
		if(x < 0 || y < 0 || static_cast<std::uint32_t>(x) > m_width || static_cast<std::uint32_t>(y) > m_height)
			return { .rgba = 0x00000000 };
		if(!m_staging_buffer.has_value())
			OpenCPUBuffer();
		if constexpr(std::endian::native == std::endian::little)
			return ReverseColor(m_staging_buffer->GetMap<mlx_color*>()[(y * m_width) + x]);
		else
			return m_staging_buffer->GetMap<mlx_color*>()[(y * m_width) + x];
	}

	void Texture::GetRegion(int x, int y, int w, int h, mlx_color* dst) noexcept
	{
		MLX_PROFILE_FUNCTION();
		if(x < 0 || y < 0 || static_cast<std::uint32_t>(x) > m_width || static_cast<std::uint32_t>(y) > m_height)
			return;
		if(!m_staging_buffer.has_value())
			OpenCPUBuffer();
		for(std::uint32_t i = 0, moving_x = x, moving_y = y;; i++, moving_x++)
		{
			if(moving_x >= static_cast<std::uint32_t>(x + w) || moving_x >= m_width)
			{
				moving_x = x;
				if(moving_y >= static_cast<std::uint32_t>(y + h) || moving_y >= m_height)
					break;
				moving_y++;
			}
			if constexpr(std::endian::native == std::endian::little)
				dst[i] = ReverseColor(m_staging_buffer->GetMap<mlx_color*>()[(moving_y * m_width) + moving_x]);
			else
				dst[i] = m_staging_buffer->GetMap<mlx_color*>()[(moving_y * m_width) + moving_x];
		}
	}

	void Texture::Clear(VkCommandBuffer cmd, Vec4f color)
	{
		MLX_PROFILE_FUNCTION();
		Image::Clear(cmd, color);
		if(m_staging_buffer.has_value())
		{
			mlx_color processed_color;
			processed_color.r = static_cast<std::uint8_t>(color.r * 255.f);
			processed_color.g = static_cast<std::uint8_t>(color.g * 255.f);
			processed_color.b = static_cast<std::uint8_t>(color.b * 255.f);
			processed_color.a = static_cast<std::uint8_t>(color.a * 255.f);
			if(processed_color.r == 0 && processed_color.g == 0 && processed_color.b == 0)
				std::memset(m_staging_buffer->GetMap(), processed_color.a, m_staging_buffer->GetSize());
			else
			{
				for(std::size_t y = 0; y < m_height; y++)
				{
					for(std::size_t x = 0; x < m_width; x++)
						m_staging_buffer->GetMap<mlx_color*>()[y * m_width + x] = processed_color;
				}
			}
		}
	}

	void Texture::Update(VkCommandBuffer cmd)
	{
		MLX_PROFILE_FUNCTION();
		if(!m_has_been_modified)
			return;
		VkImageLayout old_layout = m_layout;
		TransitionLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, cmd);
		kvfCopyBufferToImage(cmd, Image::Get(), m_staging_buffer->Get(), m_staging_buffer->GetOffset(), VK_IMAGE_ASPECT_COLOR_BIT, { m_width, m_height, 1 });
		TransitionLayout(old_layout, cmd);

		m_has_been_modified = false;
	}

	void Texture::OpenCPUBuffer()
	{
		MLX_PROFILE_FUNCTION();
		if(m_staging_buffer.has_value())
			return;
		#ifdef DEBUG
			DebugLog("Texture: enabling CPU mapping for '%'", m_debug_name);
		#endif
		m_staging_buffer.emplace();
		std::size_t size = m_width * m_height * kvfFormatSize(m_format);
		#ifdef DEBUG
			m_staging_buffer->Init(BufferType::Staging, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, {}, m_debug_name);
		#else
			m_staging_buffer->Init(BufferType::Staging, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, {}, {});
		#endif

		VkImageLayout old_layout = m_layout;
		VkCommandBuffer cmd = kvfCreateCommandBuffer(RenderCore::Get().GetDevice());
		kvfBeginCommandBuffer(cmd, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
		TransitionLayout(VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, cmd);
		kvfCopyImageToBuffer(cmd, m_staging_buffer->Get(), m_image, m_staging_buffer->GetOffset(), VK_IMAGE_ASPECT_COLOR_BIT, { m_width, m_height, 1 });
		TransitionLayout(old_layout, cmd);
		kvfEndCommandBuffer(cmd);
		VkFence fence = kvfCreateFence(RenderCore::Get().GetDevice());
		kvfSubmitSingleTimeCommandBuffer(RenderCore::Get().GetDevice(), cmd, KVF_GRAPHICS_QUEUE, fence);
		kvfDestroyFence(RenderCore::Get().GetDevice(), fence);
		kvfDestroyCommandBuffer(RenderCore::Get().GetDevice(), cmd);
	}

	Texture* StbTextureLoad(const std::filesystem::path& file, int* w, int* h)
	{
		using namespace std::literals;
		MLX_PROFILE_FUNCTION();
		std::string filename = file.string();

		if(file.stem() == "terracotta.pie")
			Message("banana, banana, banana, banana, terracotta banana terracotta, terracotta pie");

		if(!std::filesystem::exists(file))
		{
			Error("Image loader: file not found %", file);
			return nullptr;
		}
		if(stbi_is_hdr(filename.c_str()))
		{
			Error("Image loader: unsupported image format from % (HDR image)", file);
			return nullptr;
		}

		Vec2i size;
		int channels;

		std::uint8_t* data = stbi_load(filename.c_str(), &size.x, &size.y, &channels, 4);
		if(data == nullptr)
		{
			Error("Image loader: could not load % due to %", file, stbi_failure_reason());
			return nullptr;
		}
		CallOnExit defer([&]() { stbi_image_free(data); });

		CPUBuffer buffer(size.x * size.y * 4);
		std::memcpy(buffer.GetData(), data, buffer.GetSize());

		if(w != nullptr)
			*w = size.x;
		if(h != nullptr)
			*h = size.y;
		return new Texture(std::move(buffer), size.x, size.y, VK_FORMAT_R8G8B8A8_SRGB, false, std::move(filename));
	}
}
