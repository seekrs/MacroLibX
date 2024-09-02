#ifndef __MLX_IMAGE__
#define __MLX_IMAGE__

#include <Maths/Vec4.h>
#include <Renderer/RenderCore.h>
#include <Renderer/Buffer.h>
#include <Utils/Buffer.h>
#include <Renderer/Enums.h>

namespace mlx
{
	class Image
	{
		public:
			Image() = default;

			inline void Init(VkImage image, VkFormat format, std::uint32_t width, std::uint32_t height, VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED) noexcept
			{
				m_image = image;
				m_format = format;
				m_width = width;
				m_height = height;
				m_layout = layout;
			}

			void Init(ImageType type, std::uint32_t width, std::uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, bool is_multisampled = false);
			void CreateImageView(VkImageViewType type, VkImageAspectFlags aspectFlags, int layer_count = 1) noexcept;
			void CreateSampler() noexcept;
			void TransitionLayout(VkImageLayout new_layout, VkCommandBuffer cmd = VK_NULL_HANDLE);
			void Clear(VkCommandBuffer cmd, Vec4f color);

			void DestroySampler() noexcept;
			void DestroyImageView() noexcept;
			virtual void Destroy() noexcept;

			[[nodiscard]] MLX_FORCEINLINE VkImage Get() const noexcept { return m_image; }
			[[nodiscard]] MLX_FORCEINLINE VkDeviceMemory GetDeviceMemory() const noexcept { return m_memory.memory; }
			[[nodiscard]] MLX_FORCEINLINE VkImageView GetImageView() const noexcept { return m_image_view; }
			[[nodiscard]] MLX_FORCEINLINE VkFormat GetFormat() const noexcept { return m_format; }
			[[nodiscard]] MLX_FORCEINLINE VkImageTiling GetTiling() const noexcept { return m_tiling; }
			[[nodiscard]] MLX_FORCEINLINE VkImageLayout GetLayout() const noexcept { return m_layout; }
			[[nodiscard]] MLX_FORCEINLINE VkSampler GetSampler() const noexcept { return m_sampler; }
			[[nodiscard]] MLX_FORCEINLINE std::uint32_t GetWidth() const noexcept { return m_width; }
			[[nodiscard]] MLX_FORCEINLINE std::uint32_t GetHeight() const noexcept { return m_height; }
			[[nodiscard]] MLX_FORCEINLINE bool IsInit() const noexcept { return m_image != VK_NULL_HANDLE; }
			[[nodiscard]] MLX_FORCEINLINE ImageType GetType() const noexcept { return m_type; }

			virtual ~Image() = default;

		private:
			VmaAllocation m_allocation;
			VkImage m_image = VK_NULL_HANDLE;
			VkImageView m_image_view = VK_NULL_HANDLE;
			VkSampler m_sampler = VK_NULL_HANDLE;
			VkFormat m_format;
			VkImageTiling m_tiling;
			VkImageLayout m_layout = VK_IMAGE_LAYOUT_UNDEFINED;
			ImageType m_type;
			std::uint32_t m_width = 0;
			std::uint32_t m_height = 0;
			bool m_is_multisampled = false;
	};

	class DepthImage : public Image
	{
		public:
			DepthImage() = default;
			inline void Init(std::uint32_t width, std::uint32_t height, bool is_multisampled = false)
			{
				std::vector<VkFormat> candidates = { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT };
				VkFormat format = kvfFindSupportFormatInCandidates(RenderCore::Get().GetDevice(), candidates.data(), candidates.size(), VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
				Image::Init(ImageType::Depth, width, height, format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, is_multisampled); 
				Image::CreateImageView(VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_DEPTH_BIT);
				Image::TransitionLayout(VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
			}
			~DepthImage() = default;
	};

	class Texture : public Image
	{
		public:
			Texture() = default;
			Texture(CPUBuffer pixels, std::uint32_t width, std::uint32_t height, VkFormat format = VK_FORMAT_R8G8B8A8_SRGB, bool is_multisampled = false)
			{
				Init(std::move(pixels), width, height, format, is_multisampled);
			}
			inline void Init(CPUBuffer pixels, std::uint32_t width, std::uint32_t height, VkFormat format = VK_FORMAT_R8G8B8A8_SRGB, bool is_multisampled = false)
			{
				Image::Init(ImageType::Color, width, height, format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, is_multisampled);
				Image::CreateImageView(VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_COLOR_BIT);
				Image::CreateSampler();
				if(pixels)
				{
					TransitionLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
					GPUBuffer staging_buffer;
					std::size_t size = width * height * kvfFormatSize(format);
					staging_buffer.Init(BufferType::Staging, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, pixels);
					VkCommandBuffer cmd = kvfCreateCommandBuffer(RenderCore::Get().GetDevice());
					kvfBeginCommandBuffer(cmd, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
					kvfCopyBufferToImage(cmd, Image::Get(), staging_buffer.Get(), staging_buffer.GetOffset(), VK_IMAGE_ASPECT_COLOR_BIT, { width, height, 1 });
					vkEndCommandBuffer(cmd);
					VkFence fence = kvfCreateFence(RenderCore::Get().GetDevice());
					kvfSubmitSingleTimeCommandBuffer(RenderCore::Get().GetDevice(), cmd, KVF_GRAPHICS_QUEUE, fence);
					kvfDestroyFence(RenderCore::Get().GetDevice(), fence);
					staging_buffer.Destroy();
				}
				if(!pixels)
					TransitionLayout(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
				else
					TransitionLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
			}
			~Texture() override { Destroy(); }
	};
}

#endif
