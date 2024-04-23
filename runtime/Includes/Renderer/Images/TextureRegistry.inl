/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TextureRegistry.inl                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 22:08:46 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/23 22:11:09 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <Renderer/Images/TextureRegistry.h>

namespace mlx
{
	void TextureRegistry::Clear()
	{
		m_texture_descriptors.clear();
	}

	std::pair<NonOwningPtr<DrawableResource>, bool> TextureRegistry::RegisterTexture(NonOwningPtr<Texture> texture, int x, int y)
	{
		MLX_PROFILE_FUNCTION();
		auto res = m_texture_descriptors.emplace(texture, x, y);
		return std::make_pair(static_cast<DrawableResource*>(&const_cast<TextureRenderDescriptor&>(*res.first)), res.second);
	}

	bool TextureRegistry::IsTextureKnown(NonOwningPtr<Texture> texture) noexcept
	{
		MLX_PROFILE_FUNCTION();
		for(const auto& desc : m_texture_descriptors)
		{
			if(desc.texture == texture)
				return true;
		}
		return false;
	}

	void TextureRegistry::EraseTextures(NonOwningPtr<Texture> texture)
	{
		MLX_PROFILE_FUNCTION();
		for(auto it = m_texture_descriptors.begin(); it != m_texture_descriptors.end();)
		{
			if(it->texture == texture)
				it = m_texture_descriptors.erase(it);
			else
				++it;
		}
	}
}
