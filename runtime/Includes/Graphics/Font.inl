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

	std::shared_ptr<Font> FontRegistry::GetFont(const std::filesystem::path& name, float scale)
	{
		auto it = std::find_if(m_fonts_registry.begin(), m_fonts_registry.end(), [&name, scale](std::shared_ptr<Font> rhs)
		{
			return (name == rhs->GetName() && scale == rhs->GetScale());
		});
		return (it != m_fonts_registry.end() ? *it : nullptr);
	}

	void FontRegistry::Reset()
	{
		for(auto& font: m_fonts_registry)
			font->Destroy();
		m_fonts_registry.clear();
	}
}
