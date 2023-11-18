/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_image.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 11:54:21 by maldavid          #+#    #+#             */
/*   Updated: 2023/11/18 17:10:05 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_VK_IMAGE__
#define __MLX_VK_IMAGE__

#include <volk.h>
#include <cstddef>
#include <vector>
#include <renderer/command/vk_cmd_buffer.h>
#include <renderer/command/vk_cmd_pool.h>

namespace mlx
{
	uint32_t formatSize(VkFormat format);

	class Image
	{
		friend class SwapChain;

		public:
			Image() = default;

			inline void create(VkImage image, VkFormat format, uint32_t width, uint32_t height, VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED) noexcept
			{
				_image = image;
				_format = format;
				_width = width;
				_height = height;
				_layout = layout;
				_pool.init();
			}
			void create(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, const char* name, bool decated_memory = false);
			void createImageView(VkImageViewType type, VkImageAspectFlags aspectFlags) noexcept;
			void createSampler() noexcept;
			void copyFromBuffer(class Buffer& buffer);
			void copyToBuffer(class Buffer& buffer);
			void transitionLayout(VkImageLayout new_layout);
			virtual void destroy() noexcept;

			inline VkImage get() noexcept { return _image; }
			inline VkImage operator()() noexcept { return _image; }
			inline VkImageView getImageView() noexcept { return _image_view; }
			inline VkFormat getFormat() noexcept { return _format; }
			inline VkImageTiling getTiling() noexcept { return _tiling; }
			inline VkSampler getSampler() noexcept { return _sampler; }
			inline uint32_t getWidth() const noexcept { return _width; }
			inline uint32_t getHeight() const noexcept { return _height; }

			virtual ~Image() = default;

		private:
			void destroySampler() noexcept;
			void destroyImageView() noexcept;
			inline void destroyCmdPool() noexcept { _transfer_cmd.destroy(); _pool.destroy(); }

		private:
			CmdBuffer _transfer_cmd;
			CmdPool _pool;
			VmaAllocation _allocation;
			VkImage _image = VK_NULL_HANDLE;
			VkImageView _image_view = VK_NULL_HANDLE;
			VkSampler _sampler = VK_NULL_HANDLE;
			VkFormat _format;
			VkImageTiling _tiling;
			VkImageLayout _layout = VK_IMAGE_LAYOUT_UNDEFINED;
			uint32_t _width = 0;
			uint32_t _height = 0;
	};
}

#endif
