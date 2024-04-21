/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TextLibrary.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 11:52:30 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/28 22:26:10 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_TEXT_LIBRARY__
#define __MLX_TEXT_LIBRARY__

#include <Renderer/Buffers/VertexBuffer.h>
#include <Renderer/Buffers/IndexBuffer.h>
#include <Renderer/Texts/Font.h>
#include <Renderer/Core/RenderCore.h>
#include <Utils/Singleton.h>

namespace mlx
{
	using TextID = std::uint32_t;
	constexpr TextID nulltext = 0;

	class TextLibrary : public Singleton<TextLibrary>
	{
		friend class Singleton<TextLibrary>;

		public:
			std::shared_ptr<class Text> GetTextData(TextID id);
			TextID AddTextToLibrary(std::shared_ptr<Text> text);
			void RemoveTextFromLibrary(TextID id);

			void ClearLibrary();

		private:
			TextLibrary() = default;
			~TextLibrary() = default;

		private:
			std::unordered_map<TextID, std::shared_ptr<class Text>> m_cache;
			TextID m_current_id = 1;
	};
}

#endif
