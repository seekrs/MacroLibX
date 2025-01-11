#include <PreCompiled.h>
#include <Renderer/RenderPasses/Passes.h>
#include <Renderer/Renderer.h>
#include <Graphics/Scene.h>

namespace mlx
{
	void RenderPasses::Init(NonOwningPtr<class Texture> render_target)
	{
		p_render_target = render_target;

		m_2Dpass.Init();
		m_final.Init();
		std::function<void(const EventBase&)> functor = [this](const EventBase& event)
		{
			if(event.What() == Event::ResizeEventCode)
				m_main_render_texture.Destroy();
		};
		EventBus::RegisterListener({ functor, "mlx_render_passes" });
	}

	void RenderPasses::Pass(Scene& scene, Renderer& renderer, const Vec4f& clear_color)
	{
		bool force_render = false;
		if(!m_main_render_texture.IsInit())
		{
			VkExtent2D extent;
			if(p_render_target)
				extent = VkExtent2D{ .width = p_render_target->GetWidth(), .height = p_render_target->GetHeight() };
			else
				extent = kvfGetSwapchainImagesSize(renderer.GetSwapchain().Get());
			#ifdef DEBUG
				m_main_render_texture.Init({}, extent.width, extent.height, VK_FORMAT_R8G8B8A8_UNORM, false, "mlx_renderpasses_target");
			#else
				m_main_render_texture.Init({}, extent.width, extent.height, VK_FORMAT_R8G8B8A8_UNORM, false, {});
			#endif
			m_main_render_texture.TransitionLayout(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
			force_render = true;
		}
		if(scene.HasSceneChanged() || force_render)
		{
			m_main_render_texture.Clear(renderer.GetActiveCommandBuffer(), clear_color);
			m_2Dpass.Pass(scene, renderer, m_main_render_texture);
		}
		m_final.Pass(scene, renderer, m_main_render_texture, p_render_target);
		scene.ResetChangeChecker();
	}

	void RenderPasses::Destroy()
	{
		m_2Dpass.Destroy();
		m_final.Destroy();
		m_main_render_texture.Destroy();
	}
}
