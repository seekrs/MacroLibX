/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_image.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 11:54:21 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/07 01:20:31 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_VK_IMAGE__
#define __MLX_VK_IMAGE__

#include <mlx_profile.h>
#include <volk.h>
#include <cstddef>
#include <vector>
#include <vma.h>
#include <renderer/core/cmd_resource.h>
#include <renderer/command/vk_cmd_buffer.h>
#include <renderer/command/vk_cmd_pool.h>

#ifdef DEBUG
	#include <string>
#endif

namespace mlx
{
	uint32_t formatSize(VkFormat format);
	bool isStencilFormat(VkFormat format);
	bool isDepthFormat(VkFormat format);
	VkFormat bitsToFormat(uint32_t bits);
	VkPipelineStageFlags layoutToAccessMask(VkImageLayout layout, bool isDestination);

	class Image : public CmdResource
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
			}
			void create(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, const char* name, bool decated_memory = false);
			void createImageView(VkImageViewType type, VkImageAspectFlags aspectFlags) noexcept;
			void createSampler() noexcept;
			void copyFromBuffer(class Buffer& buffer);
			void copyToBuffer(class Buffer& buffer);
			void transitionLayout(VkImageLayout new_layout, CmdBuffer* cmd = nullptr);
			virtual void destroy() noexcept;

			inline VkImage get() noexcept { return _image; }
			inline VkImage operator()() noexcept { return _image; }
			inline VkImageView getImageView() const noexcept { return _image_view; }
			inline VkFormat getFormat() const noexcept { return _format; }
			inline VkImageTiling getTiling() const noexcept { return _tiling; }
			inline VkImageLayout getLayout() const noexcept { return _layout; }
			inline VkSampler getSampler() const noexcept { return _sampler; }
			inline uint32_t getWidth() const noexcept { return _width; }
			inline uint32_t getHeight() const noexcept { return _height; }
			inline bool isInit() const noexcept { return _image != VK_NULL_HANDLE; }

			virtual ~Image() = default;

		private:
			void destroySampler() noexcept;
			void destroyImageView() noexcept;

		private:
			VmaAllocation _allocation;
			VkImage _image = VK_NULL_HANDLE;
			VkImageView _image_view = VK_NULL_HANDLE;
			VkSampler _sampler = VK_NULL_HANDLE;
			#ifdef DEBUG
				std::string _name;
			#endif
			VkFormat _format;
			VkImageTiling _tiling;
			VkImageLayout _layout = VK_IMAGE_LAYOUT_UNDEFINED;
			uint32_t _width = 0;
			uint32_t _height = 0;
	};
}

#endif
