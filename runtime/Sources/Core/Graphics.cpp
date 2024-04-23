/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Graphics.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 15:13:55 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/23 14:03:51 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <PreCompiled.h>

#include <Core/Graphics.h>

namespace mlx
{
	GraphicsSupport::GraphicsSupport(std::size_t w, std::size_t h, NonOwningPtr<Texture> render_target, int id) :
		p_window(nullptr),
		m_width(w),
		m_height(h),
		m_id(id),
		m_has_window(false)
	{
		MLX_PROFILE_FUNCTION();
		m_renderer.SetWindow(nullptr);
		m_renderer.Init(render_target);
		m_pixel_put_pipeline.Init(w, h, m_renderer);
		m_text_manager.Init(m_renderer);
	}

	GraphicsSupport::GraphicsSupport(std::size_t w, std::size_t h, std::string title, int id) :
		p_window(std::make_shared<Window>(w, h, title)),
		m_width(w),
		m_height(h),
		m_id(id),
		m_has_window(true)
	{
		MLX_PROFILE_FUNCTION();
		m_renderer.SetWindow(p_window.get());
		m_renderer.Init(nullptr);
		m_pixel_put_pipeline.Init(w, h, m_renderer);
		m_text_manager.Init(m_renderer);
	}

	void GraphicsSupport::Render() noexcept
	{
		MLX_PROFILE_FUNCTION();
		if(!m_renderer.BeginFrame())
			return;
		m_proj = glm::ortho<float>(0, m_width, 0, m_height);
		m_renderer.GetUniformBuffer()->SetData(sizeof(m_proj), &m_proj);

		m_renderer.getVertDescriptorSet().Bind();

		for(auto& data : m_drawlist)
			data->Render(m_renderer);

		m_pixel_put_pipeline.Render(m_renderer);

		m_renderer.EndFrame();

		for(auto& data : _drawlist)
			data->ResetUpdate();

		#ifdef GRAPHICS_MEMORY_DUMP
			// dump memory to file every two seconds
			using namespace std::chrono_literals;
			static std::int64_t timer = static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
			if(std::chrono::duration<std::uint64_t>{static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count()) - timer} >= 1s)
			{
				RenderCore::Get().GetAllocator().DumpMemoryToJson();
				timer = static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
			}
		#endif
	}

	GraphicsSupport::~GraphicsSupport()
	{
		MLX_PROFILE_FUNCTION();
		vkDeviceWaitIdle(RenderCore::Get().GetDevice().Get());
		m_text_manager.Destroy();
		m_pixel_put_pipeline.Destroy();
		m_renderer->Destroy();
		if(p_window)
			p_window->Destroy();
	}
}
