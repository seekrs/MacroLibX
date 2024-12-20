#include <PreCompiled.h>
#include <Core/Graphics.h>
#include <Core/Application.h>
#include <Core/Handles.h>

namespace mlx
{
	GraphicsSupport::GraphicsSupport(const mlx_window_create_info* info, int id) :
		m_put_pixel_manager(&m_renderer),
		m_id(id)
	{
		MLX_PROFILE_FUNCTION();

		if(info->render_target == nullptr)
		{
			p_window = std::make_shared<Window>(info);
			m_has_window = true;
		}
		else
			m_has_window = false;

		if(info->render_target != nullptr)
		{
			m_renderer.Init(info->render_target->texture);
			m_scene_renderer.Init(info->render_target->texture);
		}
		else
		{
			m_renderer.Init(p_window.get());
			m_scene_renderer.Init(nullptr);
		}
		p_scene = std::make_unique<Scene>();
	}

	void GraphicsSupport::Render() noexcept
	{
		MLX_PROFILE_FUNCTION();
		m_renderer.BeginFrame();
			m_draw_layer = 0;
			m_scene_renderer.Render(*p_scene, m_renderer);
		m_renderer.EndFrame();
		#ifdef GRAPHICS_MEMORY_DUMP
			// Dump memory usage to file every two seconds
			using namespace std::chrono_literals;
			static std::int64_t timer = static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
			if(std::chrono::duration<std::uint64_t>{ static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count()) - timer } >= 1s)
			{
				RenderCore::Get().GetAllocator().DumpMemoryToJson();
				timer = static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
			}
		#endif
	}

	GraphicsSupport::~GraphicsSupport()
	{
		MLX_PROFILE_FUNCTION();
		RenderCore::Get().WaitDeviceIdle();
		p_scene.reset();
		m_scene_renderer.Destroy();
		m_renderer.Destroy();
		if(p_window)
			p_window->Destroy();
	}
}
