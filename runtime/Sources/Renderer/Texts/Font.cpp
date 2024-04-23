/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Font.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 22:06:09 by kbz_8             #+#    #+#             */
/*   Updated: 2024/04/23 22:48:30 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <PreCompiled.h>

#include <Renderer/Texts/Font.h>
#include <Renderer/Renderer.h>

constexpr const int RANGE = 1024;

namespace mlx
{
	Font::Font(Renderer& renderer, const std::filesystem::path& path, float scale) : m_name(path.string()), m_renderer(renderer), m_scale(scale)
	{
		m_build_data = path;
	}

	Font::Font(class Renderer& renderer, const std::string& name, const std::vector<std::uint8_t>& ttf_data, float scale) : m_name(name), m_renderer(renderer), m_scale(scale)
	{
		m_build_data = ttf_data;
	}

	void Font::BuildFont()
	{
		MLX_PROFILE_FUNCTION();
		std::vector<std::uint8_t> file_bytes;
		if(std::holds_alternative<std::filesystem::path>(m_build_data))
		{
			std::ifstream file(std::get<std::filesystem::path>(m_build_data), std::ios::binary);
			if(!file.is_open())
			{
				Error("Font load : cannot open font file, %", m_name.c_str());
				return;
			}
			std::ifstream::pos_type fileSize = std::filesystem::file_size(std::get<std::filesystem::path>(m_build_data));
			file.seekg(0, std::ios::beg);
			file_bytes.resize(fileSize);
			file.read(reinterpret_cast<char*>(file_bytes.data()), fileSize);
			file.close();
		}

		std::vector<std::uint8_t> tmp_bitmap(RANGE * RANGE);
		std::vector<std::uint8_t> vulkan_bitmap(RANGE * RANGE * 4);
		stbtt_pack_context pc;
		stbtt_PackBegin(&pc, tmp_bitmap.data(), RANGE, RANGE, RANGE, 1, nullptr);
		if(std::holds_alternative<std::filesystem::path>(m_build_data))
			stbtt_PackFontRange(&pc, file_bytes.data(), 0, m_scale, 32, 96, m_cdata.data());
		else
			stbtt_PackFontRange(&pc, std::get<std::vector<std::uint8_t>>(m_build_data).data(), 0, m_scale, 32, 96, m_cdata.data());
		stbtt_PackEnd(&pc);
		for(int i = 0, j = 0; i < RANGE * RANGE; i++, j += 4)
		{
			vulkan_bitmap[j + 0] = tmp_bitmap[i];
			vulkan_bitmap[j + 1] = tmp_bitmap[i];
			vulkan_bitmap[j + 2] = tmp_bitmap[i];
			vulkan_bitmap[j + 3] = tmp_bitmap[i];
		}
		#ifdef DEBUG
			m_atlas.Create(vulkan_bitmap.data(), RANGE, RANGE, VK_FORMAT_R8G8B8A8_UNORM, std::string(m_name + "_font_altas").c_str(), true);
		#else
			m_atlas.Create(vulkan_bitmap.data(), RANGE, RANGE, VK_FORMAT_R8G8B8A8_UNORM, nullptr, true);
		#endif
		m_atlas.SetDescriptor(m_renderer.GetFragDescriptorSet().Duplicate());
		m_is_init = true;
	}

	void Font::Destroy()
	{
		MLX_PROFILE_FUNCTION();
		m_atlas.Destroy();
		m_is_init = false;
	}

	Font::~Font()
	{
		if(m_is_init)
			destroy();
	}
}
