// This file is a part of Akel
// Authors : @kbz_8
// Created : 21/04/2024
// Updated : 21/04/2024

#pragma once
#include <Core/ImagesRegistry.h>

namespace mlx
{
	void ImageRegistry::RegisterTexture(NonOwningPtr<class Texture> texture)
	{
		m_textures_registry.insert(texture);
	}

	void ImageRegistry::UnregisterTexture(NonOwningPtr<class Texture> texture)
	{
		m_textures_registry.erase(texture);
	}

	bool ImageRegistry::IsTextureKnown(NonOwningPtr<class Texture> texture)
	{
		return m_textures_registry.find(texture) != m_textures_registry.end();
	}
}
