#ifndef __MLX_SDL_MANAGER__
#define __MLX_SDL_MANAGER__

#include <Maths/Vec2.h>

namespace mlx
{
	class SDLManager : public Singleton<SDLManager>
	{
		friend class Singleton<SDLManager>;

		public:
			void Init() noexcept;
			void Shutdown() noexcept;

			Handle CreateWindow(const std::string& title, std::size_t w, std::size_t h, bool hidden);
			void DestroyWindow(Handle window) noexcept;

			VkSurfaceKHR CreateVulkanSurface(Handle window, VkInstance instance) const noexcept;
			std::vector<const char*> GetRequiredVulkanInstanceExtentions(Handle window) const noexcept;
			Vec2ui GetVulkanDrawableSize(Handle window) const noexcept;

			inline void SetEventCallback(func::function<void(mlx_event_type, int, int, void*)> functor, void* userdata) { f_callback = std::move(functor); p_callback_data = userdata; }

		private:
			SDLManager() = default;
			~SDLManager() = default;

		private:
			std::unordered_set<Handle> m_windows_registry;
			func::function<void(mlx_event_type, int, int, void*)> f_callback;
			void* p_callback_data = nullptr;
			std::int32_t m_x;
			std::int32_t m_y;
			std::int32_t m_rel_x;
			std::int32_t m_rel_y;
			bool m_drop_sdl_responsability = false;
	};
}

#endif
