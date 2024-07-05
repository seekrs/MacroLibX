/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Image.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 11:54:21 by maldavid          #+#    #+#             */
/*   Updated: 2024/07/05 13:40:51 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_VK_IMAGE__
#define __MLX_VK_IMAGE__

#include <Renderer/Command/CommandResource.h>
#include <Renderer/Command/CommandBuffer.h>
#include <Renderer/Command/CommandPool.h>

namespace mlx
{
	std::uint32_t FormatSize(VkFormat format);
	bool IsStencilFormat(VkFormat format);
	bool IsDepthFormat(VkFormat format);
	VkFormat BitsToFormat(std::uint32_t bits);
	VkPipelineStageFlags LayoutToAccessMask(VkImageLayout layout, bool is_destination);

	class Image : public CommandResource
	{
		friend class Swapchain;

		public:
			Image() = default;

			inline void Create(VkImage image, VkFormat format, std::uint32_t width, std::uint32_t height, VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED) noexcept
			{
				m_image = image;
				m_format = format;
				m_width = width;
				m_height = height;
				m_layout = layout;
			}
			void Create(std::uint32_t width, std::uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, const char* name, bool decated_memory = false);
			void CreateImageView(VkImageViewType type, VkImageAspectFlags aspect_flags) noexcept;
			void CreateSampler() noexcept;
			void CopyFromBuffer(class Buffer& buffer);
			void CopyToBuffer(class Buffer& buffer);
			void TransitionLayout(VkImageLayout new_layout, NonOwningPtr<CommandBuffer> cmd = nullptr);
			virtual void Destroy() noexcept;

			inline VkImage Get() noexcept { return m_image; }
			inline VkImage operator()() noexcept { return m_image; }
			inline VkImageView GetImageView() const noexcept { return m_image_view; }
			inline VkFormat GetFormat() const noexcept { return m_format; }
			inline VkImageTiling GetTiling() const noexcept { return m_tiling; }
			inline VkImageLayout GetLayout() const noexcept { return m_layout; }
			inline VkSampler GetSampler() const noexcept { return m_sampler; }
			inline std::uint32_t GetWidth() const noexcept { return m_width; }
			inline std::uint32_t GetHeight() const noexcept { return m_height; }
			inline bool IsInit() const noexcept { return m_image != VK_NULL_HANDLE; }

			virtual ~Image() = default;

		private:
			void DestroySampler() noexcept;
			void DestroyImageView() noexcept;

		private:
			VmaAllocation m_allocation;
			VkImage m_image = VK_NULL_HANDLE;
			VkImageView m_image_view = VK_NULL_HANDLE;
			VkSampler m_sampler = VK_NULL_HANDLE;
			#ifdef DEBUG
				std::string m_name;
			#endif
			VkFormat m_format;
			VkImageTiling m_tiling;
			VkImageLayout m_layout = VK_IMAGE_LAYOUT_UNDEFINED;
			std::uint32_t m_width = 0;
			std::uint32_t m_height = 0;
	};
}

#endif
