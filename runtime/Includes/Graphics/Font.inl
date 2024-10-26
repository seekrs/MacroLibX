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
		return std::find_if(m_fonts_registry.begin(), m_fonts_registry.end(), [&font](std::shared_ptr<Font> rhs)
		{
			return font->GetName() == rhs->GetName() && font->GetScale() == rhs->GetScale();
		}) != m_fonts_registry.end();
	}
}
