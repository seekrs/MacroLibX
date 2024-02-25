/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text_descriptor.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 00:13:34 by maldavid          #+#    #+#             */
/*   Updated: 2024/02/25 07:58:13 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_TEXT_DESCRIPTOR__
#define __MLX_TEXT_DESCRIPTOR__

#include <string>
#include <mlx_profile.h>
#include <volk.h>
#include <utils/combine_hash.h>
#include <renderer/core/drawable_resource.h>
#include <renderer/texts/text_library.h>
#include <renderer/texts/font_library.h>
#include <array>

namespace mlx
{
	class TextDrawDescriptor : public DrawableResource
	{
		friend class std::hash<TextDrawDescriptor>;

		public:
			TextID id;
			std::uint32_t color;
			int x;
			int y;

		public:
			TextDrawDescriptor(std::string text, std::uint32_t _color, int _x, int _y);

			void init(FontID font) noexcept;
			bool operator==(const TextDrawDescriptor& rhs) const { return _text == rhs._text && x == rhs.x && y == rhs.y && color == rhs.color; }
			void render(std::array<VkDescriptorSet, 2>& sets, Renderer& renderer) override;
			void resetUpdate() override;

			TextDrawDescriptor() = default;

		private:
			std::string _text;
	};
}

namespace std
{
	template <>
	struct hash<mlx::TextDrawDescriptor>
	{
		std::size_t operator()(const mlx::TextDrawDescriptor& d) const noexcept
		{
			std::size_t hash = 0;
			mlx::hashCombine(hash, d.x, d.y, d.color, d._text);
			return hash;
		}
	};
}

#endif
