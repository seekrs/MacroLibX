/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SDLManager.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 15:28:59 by maldavid          #+#    #+#             */
/*   Updated: 2024/05/25 16:11:50 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_SDL_MANAGER__
#define __MLX_SDL_MANAGER__

#include <Utils/Singleton.h>

namespace mlx
{
	class SDLManager : public Singleton<SDLManager>
	{
		friend class Singleton<SDLManager>;

		public:
			void Init() noexcept;
			void Shutdown() noexcept;

			void* CreateWindow(const std::string& title, std::size_t w, std::size_t h);
			void DestroyWindow(void* window) noexcept;

		private:
			SDLManager() = default;
			~SDLManager() = default;

		private:
			std::unordered_set<void*> m_windows_registry;
			bool m_drop_sdl_responsability = false;
	};
}

#endif
