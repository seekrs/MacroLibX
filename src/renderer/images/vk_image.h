/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_image.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 11:54:21 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/02 20:20:55 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __VK_IMAGE__
#define __VK_IMAGE__

#include <volk.h>
#include <cstddef>
#include <vector>

namespace mlx
{
	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

	class Image
	{
		public:
			Image() = default;

			void create(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
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
			inline VkSampler getSampler() noexcept { return _sampler; }
			inline uint32_t getWidth() const noexcept { return _width; }
			inline uint32_t getHeight() const noexcept { return _height; }

			virtual ~Image() = default;

		private:
			VkImage _image = VK_NULL_HANDLE;
			VkDeviceMemory _memory = VK_NULL_HANDLE;
			VkImageView _image_view = VK_NULL_HANDLE;
			VkSampler _sampler = VK_NULL_HANDLE;
			VkFormat _format;
			uint32_t _width = 0;
			uint32_t _height = 0;
	};
}

#endif
