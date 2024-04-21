/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TextDescriptor.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 00:13:34 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/28 22:25:09 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_TEXT_DESCRIPTOR__
#define __MLX_TEXT_DESCRIPTOR__

#include <Utils/CombineHash.h>
#include <Renderer/Core/DrawableResource.h>
#include <Renderer/Texts/TextLibrary.h>
#include <Renderer/Texts/FontLibrary.h>

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

			void Init(FontID font) noexcept;
			bool operator==(const TextDrawDescriptor& rhs) const { return m_text == rhs.m_text && x == rhs.x && y == rhs.y && color == rhs.color; }
			void Render(std::array<VkDescriptorSet, 2>& sets, Renderer& renderer) override;
			void ResetUpdate() override;

			TextDrawDescriptor() = default;

		private:
			std::string m_text;
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
			mlx::HashCombine(hash, d.x, d.y, d.color, d.m_text);
			return hash;
		}
	};
}

#endif
