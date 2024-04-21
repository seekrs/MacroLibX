/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Texture.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 02:24:58 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/28 22:11:21 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_TEXTURE__
#define __MLX_TEXTURE__

#include <Renderer/Images/Image.h>
#include <Renderer/Descriptors/DescriptorSet.h>
#include <Renderer/Buffers/IndexBuffer.h>
#include <Renderer/Buffers/VertexBuffer.h>

namespace mlx
{
	class Texture : public Image 
	{
		public:
			Texture() = default;

			void Create(std::uint8_t* pixels, std::uint32_t width, std::uint32_t height, VkFormat format, const char* name, bool dedicated_memory = false);
			void Render(std::array<VkDescriptorSet, 2>& sets, class Renderer& renderer, int x, int y);
			void Destroy() noexcept override;

			void SetPixel(int x, int y, std::uint32_t color) noexcept;
			int GetPixel(int x, int y) noexcept;

			inline void SetDescriptor(DescriptorSet&& set) noexcept { m_set = set; }
			inline VkDescriptorSet GetSet() noexcept { return m_set.isInit() ? m_set.get() : VK_NULL_HANDLE; }
			inline void UpdateSet(int binding) noexcept { m_set.writeDescriptor(binding, *this); m_has_set_been_updated = true; }
			inline bool HasBeenUpdated() const noexcept { return m_has_set_been_updated; }
			inline constexpr void ResetUpdate() noexcept { m_has_set_been_updated = false; }

			~Texture() = default;

		private:
			void OpenCPUmap();

		private:
			ConstantVertexBuffer m_vbo;
			ConstantIndexBuffer m_ibo;
			#ifdef DEBUG
				std::string m_name;
			#endif
			DescriptorSet m_set;
			std::vector<std::uint32_t> m_cpu_map;
			std::optional<Buffer> m_buf_map = std::nullopt;
			void* m_map = nullptr;
			bool m_has_been_modified = false;
			bool m_has_set_been_updated = false;
	};

	Texture StbTextureLoad(std::filesystem::path file, int* w, int* h);
}

#endif
