#include <PreCompiled.h>
#include <Renderer/RenderPasses/Passes.h>
#include <Renderer/Renderer.h>
#include <Graphics/Scene.h>

namespace mlx
{
	void RenderPasses::Init(Renderer& renderer)
	{
		m_2Dpass.Init(renderer);
		m_final.Init(renderer);
		func::function<void(const EventBase&)> functor = [this, renderer](const EventBase& event)
		{
			if(event.What() == Event::ResizeEventCode)
			{
				m_main_render_texture.Destroy();
				auto extent = kvfGetSwapchainImagesSize(renderer.GetSwapchain());
				#ifdef DEBUG
					m_main_render_texture.Init({}, extent.width, extent.height, VK_FORMAT_R8G8B8A8_SRGB, false, "mlx_renderpasses_target");
				#else
					m_main_render_texture.Init({}, extent.width, extent.height, VK_FORMAT_R8G8B8A8_SRGB, false, {});
				#endif
				m_main_render_texture.TransitionLayout(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
			}
		};
		EventBus::RegisterListener({ functor, "__MlxRenderPasses" });
		auto extent = kvfGetSwapchainImagesSize(renderer.GetSwapchain());

		#ifdef DEBUG
			m_main_render_texture.Init({}, extent.width, extent.height, VK_FORMAT_R8G8B8A8_SRGB, false, "mlx_renderpasses_target");
		#else
			m_main_render_texture.Init({}, extent.width, extent.height, VK_FORMAT_R8G8B8A8_SRGB, false, {});
		#endif
		m_main_render_texture.TransitionLayout(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
	}

	void RenderPasses::Pass(Scene& scene, Renderer& renderer)
	{
		m_main_render_texture.Clear(renderer.GetActiveCommandBuffer(), Vec4f{ 0.0f, 0.0f, 0.0f, 1.0f });
		scene.GetDepth().Clear(renderer.GetActiveCommandBuffer(), {});

		m_2Dpass.Pass(scene, renderer, m_main_render_texture);
		m_final.Pass(scene, renderer, m_main_render_texture);
	}

	void RenderPasses::Destroy()
	{
		m_2Dpass.Destroy();
		m_final.Destroy();
		m_main_render_texture.Destroy();
	}
}
