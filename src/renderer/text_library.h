/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text_library.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 11:52:30 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/12 11:38:57 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_TEXT_LIBRARY__
#define __MLX_TEXT_LIBRARY__

#include <renderer/buffers/vk_vbo.h>
#include <renderer/buffers/vk_ibo.h>
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include <cstdint>

namespace mlx
{
	using TextID = uint32_t;
	constexpr TextID nulltext = 0;

	class TextData
	{
		public:
			TextData() = default;

			void init(std::string text, std::vector<Vertex> vbo_data, std::vector<uint16_t> ibo_data);
			void bind(class Renderer& renderer) noexcept;
			inline uint32_t getIBOsize() noexcept { return _ibo.getSize(); }
			inline const std::string& getText() const { return _text; }
			void destroy() noexcept;

			~TextData() = default;

		private:
			C_VBO _vbo;
			C_IBO _ibo;
			std::string _text;
	};

	class TextLibrary
	{
		public:
			TextLibrary() = default;

			std::shared_ptr<TextData> getTextData(TextID id);
			TextID addTextToLibrary(std::shared_ptr<TextData> text);
			void removeTextFromLibrary(TextID id);

			void clearLibrary();

			~TextLibrary() = default;

		private:
			std::unordered_map<TextID, std::shared_ptr<TextData>> _cache;
			TextID _current_id = 1;
	};
}

#endif
