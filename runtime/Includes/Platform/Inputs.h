#ifndef __MLX_INPUTS__
#define __MLX_INPUTS__

#include <mlx.h>
#include <mlx_profile.h>
#include <Platform/Window.h>

namespace mlx
{
	class Inputs
	{
		public:
			struct Hook
			{
				std::function<void(int, void*)> fn;
				void* param = nullptr;

				Hook(std::function<void(int, void*)> fn, void* param) : fn(fn), param(param) {}
			};

		public:
			Inputs() = default;

			void FetchInputs();

			inline void RegisterWindow(std::shared_ptr<Window> window) { m_windows[window->GetID()] = window; }

			std::int32_t GetX() const noexcept;
			std::int32_t GetY() const noexcept;
			std::int32_t GetXRel() const noexcept;
			std::int32_t GetYRel() const noexcept;

			int GetDefaultControllerId() noexcept;
			float GetControllerAxis(int controller_id, int axis_kind) const noexcept;
			void RumbleController(int controller_id, float low_freq, float high_freq, float duration) const noexcept;

			inline bool IsMouseMoving() const noexcept { return GetXRel() || GetYRel(); }
			MLX_FORCEINLINE bool IsRunning() const noexcept { return m_run; }
			MLX_FORCEINLINE constexpr void Finish() noexcept { m_run = false; }
			MLX_FORCEINLINE constexpr void Run() noexcept { m_run = true; }

			inline void OnEvent(std::uint32_t id, int event, void(*f)(int, void*), void* param) noexcept
			{
				m_events_hooks[id][event].emplace_back(f, param);
			}

			~Inputs() = default;

		private:
			std::unordered_map<std::uint32_t, std::shared_ptr<Window>> m_windows;
			std::unordered_map<std::uint32_t, std::array<std::vector<Hook>, 9>> m_events_hooks;
			bool m_run = false;
	};
}

#endif
