/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 02:24:58 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/14 19:06:07 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_TEXTURE__
#define __MLX_TEXTURE__

#include <filesystem>
#include <array>
#include <renderer/images/vk_image.h>
#include <renderer/descriptors/vk_descriptor_set.h>
#include <renderer/buffers/vk_ibo.h>
#include <renderer/buffers/vk_vbo.h>
#include <mlx_profile.h>
#ifdef DEBUG
	#include <string>
#endif

namespace mlx
{
	class Texture : public Image 
	{
		public:
			Texture() = default;

			void create(std::uint8_t* pixels, std::uint32_t width, std::uint32_t height, VkFormat format, const char* name, bool dedicated_memory = false);
			void render(std::array<VkDescriptorSet, 2>& sets, class Renderer& renderer, int x, int y);
			void destroy() noexcept override;

			void setPixel(int x, int y, std::uint32_t color) noexcept;
			int getPixel(int x, int y) noexcept;

			inline void setDescriptor(DescriptorSet&& set) noexcept { _set = set; }
			inline VkDescriptorSet getSet() noexcept { return _set.isInit() ? _set.get() : VK_NULL_HANDLE; }
			inline void updateSet(int binding) noexcept { _set.writeDescriptor(binding, *this); _has_set_been_updated = true; }
			inline bool hasBeenUpdated() const noexcept { return _has_set_been_updated; }
			inline constexpr void resetUpdate() noexcept { _has_set_been_updated = false; }

			~Texture() = default;

		private:
			void openCPUmap();

		private:
			C_VBO _vbo;
			C_IBO _ibo;
			#ifdef DEBUG
				std::string _name;
			#endif
			DescriptorSet _set;
			std::vector<std::uint32_t> _cpu_map;
			std::optional<Buffer> _buf_map = std::nullopt;
			void* _map = nullptr;
			bool _has_been_modified = false;
			bool _has_set_been_updated = false;
	};

	Texture stbTextureLoad(std::filesystem::path file, int* w, int* h);
}

#endif
