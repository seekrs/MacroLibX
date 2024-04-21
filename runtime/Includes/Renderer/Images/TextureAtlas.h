/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TextureAtlas.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 16:36:33 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/28 22:12:13 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_TEXTURE_ATLAS__
#define __MLX_TEXTURE_ATLAS__

#include <Renderer/Images/Texture.h>

namespace mlx
{
	class TextureAtlas : public Image
	{
		public:
			TextureAtlas() = default;

			void Create(std::uint8_t* pixels, std::uint32_t width, std::uint32_t height, VkFormat format, const char* name, bool dedicated_memory = false);
			void Render(class Renderer& renderer, int x, int y, std::uint32_t ibo_size) const;
			void Destroy() noexcept override;

			inline void SetDescriptor(DescriptorSet&& set) noexcept { m_set = set; }
			inline VkDescriptorSet GetVkSet() noexcept { return m_set.isInit() ? m_set.get() : VK_NULL_HANDLE; }
			inline DescriptorSet GetSet() noexcept { return m_set; }
			inline void UpdateSet(int binding) noexcept { m_set.writeDescriptor(binding, *this); m_has_been_updated = true; }
			inline bool HasBeenUpdated() const noexcept { return m_has_been_updated; }
			inline constexpr void ResetUpdate() noexcept { m_has_been_updated = false; }

			~TextureAtlas() = default;

		private:
			DescriptorSet m_set;
			bool m_has_been_updated = false;
	};
}

#endif
