/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   font.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 22:06:09 by kbz_8             #+#    #+#             */
/*   Updated: 2024/01/18 13:16:18 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <renderer/texts/font.h>
#include <renderer/renderer.h>
#include <core/profiler.h>
#include <fstream>

constexpr const int RANGE = 1024;

namespace mlx
{
	Font::Font(Renderer& renderer, const std::filesystem::path& path, float scale) : _name(path.string()), _renderer(renderer), _scale(scale)
	{
		_build_data = path;
	}

	Font::Font(class Renderer& renderer, const std::string& name, const std::vector<std::uint8_t>& ttf_data, float scale) : _name(name), _renderer(renderer), _scale(scale)
	{
		_build_data = ttf_data;
	}

	void Font::buildFont()
	{
		MLX_PROFILE_FUNCTION();
		std::vector<std::uint8_t> file_bytes;
		if(std::holds_alternative<std::filesystem::path>(_build_data))
		{
			std::ifstream file(std::get<std::filesystem::path>(_build_data), std::ios::binary);
			if(!file.is_open())
			{
				core::error::report(e_kind::error, "Font load : cannot open font file, %s", _name.c_str());
				return;
			}
			std::ifstream::pos_type fileSize = std::filesystem::file_size(std::get<std::filesystem::path>(_build_data));
			file.seekg(0, std::ios::beg);
			file_bytes.resize(fileSize);
			file.read(reinterpret_cast<char*>(file_bytes.data()), fileSize);
			file.close();
		}

		std::vector<std::uint8_t> tmp_bitmap(RANGE * RANGE);
		std::vector<std::uint8_t> vulkan_bitmap(RANGE * RANGE * 4);
		stbtt_pack_context pc;
		stbtt_PackBegin(&pc, tmp_bitmap.data(), RANGE, RANGE, RANGE, 1, nullptr);
		if(std::holds_alternative<std::filesystem::path>(_build_data))
			stbtt_PackFontRange(&pc, file_bytes.data(), 0, _scale, 32, 96, _cdata.data());
		else
			stbtt_PackFontRange(&pc, std::get<std::vector<std::uint8_t>>(_build_data).data(), 0, _scale, 32, 96, _cdata.data());
		stbtt_PackEnd(&pc);
		for(int i = 0, j = 0; i < RANGE * RANGE; i++, j += 4)
		{
			vulkan_bitmap[j + 0] = tmp_bitmap[i];
			vulkan_bitmap[j + 1] = tmp_bitmap[i];
			vulkan_bitmap[j + 2] = tmp_bitmap[i];
			vulkan_bitmap[j + 3] = tmp_bitmap[i];
		}
		#ifdef DEBUG
			_atlas.create(vulkan_bitmap.data(), RANGE, RANGE, VK_FORMAT_R8G8B8A8_UNORM, std::string(_name + "_font_altas").c_str(), true);
		#else
			_atlas.create(vulkan_bitmap.data(), RANGE, RANGE, VK_FORMAT_R8G8B8A8_UNORM, nullptr, true);
		#endif
		_atlas.setDescriptor(_renderer.getFragDescriptorSet().duplicate());
		_is_init = true;
	}

	void Font::destroy()
	{
		MLX_PROFILE_FUNCTION();
		_atlas.destroy();
		_is_init = false;
	}

	Font::~Font()
	{
		if(_is_init)
			destroy();
	}
}
