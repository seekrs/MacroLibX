#pragma once
#include <Graphics/Font.h>

namespace mlx
{
	void FontRegistry::RegisterFont(std::shared_ptr<Font> font)
	{
		m_fonts_registry.insert(font);
	}

	void FontRegistry::UnregisterFont(std::shared_ptr<Font> font)
	{
		m_fonts_registry.erase(font);
	}

	bool FontRegistry::IsFontKnown(std::shared_ptr<Font> font)
	{
		return m_fonts_registry.find(font) != m_fonts_registry.end();
	}
}
