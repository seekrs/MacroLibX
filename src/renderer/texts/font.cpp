/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   font.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 22:06:09 by kbz_8             #+#    #+#             */
/*   Updated: 2024/01/11 01:23:20 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <renderer/texts/font.h>
#include <renderer/renderer.h>
#include <core/profiler.h>
#include <fstream>

constexpr const int RANGE = 1024;

namespace mlx
{
	Font::Font(Renderer& renderer, const std::filesystem::path& path, float scale) : non_copyable(), _name(path.string()), _scale(scale)
	{
		MLX_PROFILE_FUNCTION();
		std::vector<uint8_t> tmp_bitmap(RANGE * RANGE);
		std::vector<uint8_t> vulkan_bitmap(RANGE * RANGE * 4);

		std::ifstream file(path, std::ios::binary);
		if(!file.is_open())
		{
			core::error::report(e_kind::error, "Font load : cannot open font file, %s", _name.c_str());
			return;
		}
		std::ifstream::pos_type fileSize = std::filesystem::file_size(path);
		file.seekg(0, std::ios::beg);
		std::vector<uint8_t> bytes(fileSize);
		file.read(reinterpret_cast<char*>(bytes.data()), fileSize);
		file.close();

		stbtt_pack_context pc;
		stbtt_PackBegin(&pc, tmp_bitmap.data(), RANGE, RANGE, RANGE, 1, nullptr);
		stbtt_PackFontRange(&pc, bytes.data(), 0, scale, 32, 96, _cdata.data());
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
		_atlas.setDescriptor(renderer.getFragDescriptorSet().duplicate());
	}

	Font::Font(class Renderer& renderer, const std::string& name, const std::vector<uint8_t>& ttf_data, float scale) : non_copyable(), _name(name), _scale(scale)
	{
		MLX_PROFILE_FUNCTION();
		std::vector<uint8_t> tmp_bitmap(RANGE * RANGE);
		std::vector<uint8_t> vulkan_bitmap(RANGE * RANGE * 4);
		stbtt_pack_context pc;
		stbtt_PackBegin(&pc, tmp_bitmap.data(), RANGE, RANGE, RANGE, 1, nullptr);
		stbtt_PackFontRange(&pc, ttf_data.data(), 0, scale, 32, 96, _cdata.data());
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
		_atlas.setDescriptor(renderer.getFragDescriptorSet().duplicate());
	}

	Font::~Font()
	{
		MLX_PROFILE_FUNCTION();
		_atlas.destroy();
	}
}
