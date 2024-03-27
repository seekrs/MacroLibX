/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Inputs.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 16:27:35 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/27 18:36:21 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_INPUTS__
#define __MLX_INPUTS__

#include <mlx_profile.h>
#include <Platform/Window.h>

namespace mlx
{
	class Inputs
	{
		public:
			struct Hook
			{
				func::function<int(int, void*)> hook;
				void* param = nullptr;
			};

		public:
			Inputs() = default;

			virtual void Update() noexcept = 0;

			virtual void RegisterWindow(std::shared_ptr<Window> window) = 0;

			virtual std::int32_t GetX() const noexcept = 0;
			virtual std::int32_t GetY() const noexcept = 0;
			virtual std::int32_t GetXRel() const noexcept = 0;
			virtual std::int32_t GetYRel() const noexcept = 0;

			inline bool IsMouseMoving() const noexcept { return GetXRel() || GetYRel(); }
			MLX_FORCEINLINE bool IsRunning() const noexcept { return m_run; }
			MLX_FORCEINLINE constexpr void Finish() noexcept { m_run = false; }
			MLX_FORCEINLINE constexpr void Run() noexcept { m_run = true; }

			inline void OnEvent(std::uint32_t id, int event, int (*funct_ptr)(int, void*), void* param) noexcept
			{
				m_events_hooks[id][event].hook = funct_ptr;
				m_events_hooks[id][event].param = param;
			}

			virtual ~Inputs() = default;

		protected:
			std::unordered_map<std::uint32_t, std::shared_ptr<Window>> m_windows;
			std::unordered_map<std::uint32_t, std::array<Hook, 6>> m_events_hooks;
			bool m_run = false;
	};
}

#endif
