/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_image.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 11:54:21 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/23 14:17:11 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __VK_IMAGE__
#define __VK_IMAGE__

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
		public:
			Image() = default;

			void create(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, std::vector<VkMemoryPropertyFlags> properties);
			void createImageView(VkImageViewType type, VkImageAspectFlags aspectFlags) noexcept;
			void createSampler() noexcept;
			void copyFromBuffer(class Buffer& buffer);
			void copyToBuffer(class Buffer& buffer);
			virtual void destroy() noexcept;

			inline VkImage get() noexcept { return _image; }
			inline VkImage operator()() noexcept { return _image; }
			inline VkDeviceMemory getDeviceMemory() noexcept { return _memory; }
			inline VkImageView getImageView() noexcept { return _image_view; }
			inline VkFormat getFormat() noexcept { return _format; }
			inline VkImageTiling getTiling() noexcept { return _tiling; }
			inline VkSampler getSampler() noexcept { return _sampler; }
			inline uint32_t getWidth() const noexcept { return _width; }
			inline uint32_t getHeight() const noexcept { return _height; }

			virtual ~Image() = default;

		private:
			CmdBuffer _transfer_cmd;
			CmdPool _pool;
			VkImage _image = VK_NULL_HANDLE;
			VkDeviceMemory _memory = VK_NULL_HANDLE;
			VkImageView _image_view = VK_NULL_HANDLE;
			VkSampler _sampler = VK_NULL_HANDLE;
			VkFormat _format;
			VkImageTiling _tiling;
			uint32_t _width = 0;
			uint32_t _height = 0;
	};
}

#endif
