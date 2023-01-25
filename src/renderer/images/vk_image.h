/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_image.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 11:54:21 by maldavid          #+#    #+#             */
/*   Updated: 2023/01/25 11:56:03 by maldavid         ###   ########.fr       */
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

			virtual void create(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImageAspectFlags aspectFlags);
			void destroy() noexcept;

			void copyBuffer(class Buffer& buffer);

			inline VkImage& get() noexcept { return _image; }
			inline VkImage& operator()() noexcept { return _image; }
			inline VkDeviceMemory getDeviceMemory() noexcept { return _memory; }
			inline VkImageView& getImageView() noexcept { return _image_view; }
			inline VkFormat& getFormat() noexcept { return _format; }

			virtual ~Image() = default;

		private:
			VkImage _image = VK_NULL_HANDLE;
			VkDeviceMemory _memory = VK_NULL_HANDLE;
			VkImageView _image_view = VK_NULL_HANDLE;
			uint32_t _width = 0;
			uint32_t _height = 0;
			VkFormat _format;
	};
}

#endif
