#include <PreCompiled.h>

#include <Graphics/Font.h>
#include <Core/Memory.h>

#define STB_TRUETYPE_IMPLEMENTATION
#define STB_malloc(x, u) ((void)(u), MemManager::Get().Malloc(x))
#define STB_free(x, u) ((void)(u), MemManager::Get().Free(x))
#include <stb_truetype.h>

namespace mlx
{
	void Font::BuildFont()
	{
		MLX_PROFILE_FUNCTION();
		std::vector<std::uint8_t> file_bytes;
		if(std::holds_alternative<std::filesystem::path>(m_build_data))
		{
			std::ifstream file(std::get<std::filesystem::path>(m_build_data), std::ios::binary);
			if(!file.is_open())
			{
				Error("Font: cannot open font file, %", m_name);
				return;
			}
			std::ifstream::pos_type file_size = std::filesystem::file_size(std::get<std::filesystem::path>(m_build_data));
			file.seekg(0, std::ios::beg);
			file_bytes.resize(file_size);
			file.read(reinterpret_cast<char*>(file_bytes.data()), file_size);
			file.close();
		}

		CPUBuffer tmp_bitmap(RANGE * RANGE);

		stbtt_pack_context pc;
		stbtt_PackBegin(&pc, tmp_bitmap.GetData(), RANGE, RANGE, RANGE, 1, nullptr);
		if(std::holds_alternative<std::filesystem::path>(m_build_data))
			stbtt_PackFontRange(&pc, file_bytes.data(), 0, m_scale, 32, 96, m_cdata.data());
		else
			stbtt_PackFontRange(&pc, std::get<std::vector<std::uint8_t>>(m_build_data).data(), 0, m_scale, 32, 96, m_cdata.data());
		stbtt_PackEnd(&pc);

		CPUBuffer vulkan_bitmap(RANGE * RANGE * 4);

		for(int i = 0, j = 0; i < RANGE * RANGE; i++, j += 4)
		{
			vulkan_bitmap.GetData()[j + 0] = tmp_bitmap.GetData()[i];
			vulkan_bitmap.GetData()[j + 1] = tmp_bitmap.GetData()[i];
			vulkan_bitmap.GetData()[j + 2] = tmp_bitmap.GetData()[i];
			vulkan_bitmap.GetData()[j + 3] = tmp_bitmap.GetData()[i];
		}

		#ifdef DEBUG
			m_atlas.Init(vulkan_bitmap, RANGE, RANGE, VK_FORMAT_R8G8B8A8_SRGB, false, m_name + "_font_atlas");
		#else
			m_atlas.Init(vulkan_bitmap, RANGE, RANGE, VK_FORMAT_R8G8B8A8_SRGB, false, {});
		#endif

		DebugLog("Font: loaded %", m_name);
	}

	void Font::Destroy()
	{
		m_atlas.Destroy();
		DebugLog("Font: unloaded %", m_name);
	}
}
