/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 02:24:58 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/01 15:31:26 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_TEXTURE__
#define __MLX_TEXTURE__

#include <filesystem>
#include <renderer/images/vk_image.h>
#include <renderer/descriptors/vk_descriptor_set.h>
#include <renderer/buffers/vk_ibo.h>
#include <renderer/buffers/vk_vbo.h>

namespace mlx
{
	class Texture : public Image
	{
		public:
			Texture() = default;
			
			void create(uint8_t* pixels, uint32_t width, uint32_t height, VkFormat format);
			void render(class Renderer& renderer, int x, int y);
			void destroy() noexcept override;

			inline void setDescriptor(DescriptorSet set) noexcept { _set = std::move(set); }
			inline VkDescriptorSet getSet() noexcept { return _set.isInit() ? _set.get() : VK_NULL_HANDLE; }
			inline void updateSet(int binding) noexcept { _set.writeDescriptor(binding, getImageView(), getSampler());}

			~Texture() = default;

		private:
			C_VBO _vbo;
			C_IBO _ibo;
			DescriptorSet _set;
	};

	Texture stb_texture_load(std::filesystem::path file, int* w, int* h);
}

#endif
