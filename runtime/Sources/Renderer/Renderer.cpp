#include <PreCompiled.h>
#include <Renderer/Renderer.h>
#include <Renderer/RenderCore.h>
#include <Core/Enums.h>
#include <Renderer/Pipelines/Shader.h>
#include <Core/EventBus.h>

namespace mlx
{
	namespace Internal
	{
		struct FrameBeginEventBroadcast : public EventBase
		{
			Event What() const override { return Event::FrameBeginEventCode; }
		};
	}

	void Renderer::Init(NonOwningPtr<Window> window)
	{
		MLX_PROFILE_FUNCTION();
		p_window = window;
		m_swapchain.Init(p_window);
		for(std::size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			m_image_available_semaphores[i] = kvfCreateSemaphore(RenderCore::Get().GetDevice());
			DebugLog("Vulkan: image available semaphore created");
			m_render_finished_semaphores[i] = kvfCreateSemaphore(RenderCore::Get().GetDevice());
			DebugLog("Vulkan: render finished semaphore created");
			m_cmd_buffers[i] = kvfCreateCommandBuffer(RenderCore::Get().GetDevice());
			DebugLog("Vulkan: command buffer created");
			m_cmd_fences[i] = kvfCreateFence(RenderCore::Get().GetDevice());
			DebugLog("Vulkan: fence created");
		}
	}

	void Renderer::Init(NonOwningPtr<Texture> render_target)
	{
		MLX_PROFILE_FUNCTION();
		p_render_target = render_target;
		for(std::size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			m_image_available_semaphores[i] = kvfCreateSemaphore(RenderCore::Get().GetDevice());
			DebugLog("Vulkan: image available semaphore created");
			m_render_finished_semaphores[i] = kvfCreateSemaphore(RenderCore::Get().GetDevice());
			DebugLog("Vulkan: render finished semaphore created");
			m_cmd_buffers[i] = kvfCreateCommandBuffer(RenderCore::Get().GetDevice());
			DebugLog("Vulkan: command buffer created");
			m_cmd_fences[i] = kvfCreateFence(RenderCore::Get().GetDevice());
			DebugLog("Vulkan: fence created");
		}
	}

	void Renderer::BeginFrame()
	{
		MLX_PROFILE_FUNCTION();
		kvfWaitForFence(RenderCore::Get().GetDevice(), m_cmd_fences[m_current_frame_index]);
		if(p_window)
			m_swapchain.AquireFrame(m_image_available_semaphores[m_current_frame_index]);
		RenderCore::Get().vkResetCommandBuffer(m_cmd_buffers[m_current_frame_index], 0);
		kvfBeginCommandBuffer(m_cmd_buffers[m_current_frame_index], 0);
		m_drawcalls = 0;
		m_polygons_drawn = 0;
		EventBus::SendBroadcast(Internal::FrameBeginEventBroadcast{});
	}

	void Renderer::EndFrame()
	{
		MLX_PROFILE_FUNCTION();
		VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		kvfEndCommandBuffer(m_cmd_buffers[m_current_frame_index]);
		if(p_window)
			kvfSubmitCommandBuffer(RenderCore::Get().GetDevice(), m_cmd_buffers[m_current_frame_index], KVF_GRAPHICS_QUEUE, m_render_finished_semaphores[m_current_frame_index], m_image_available_semaphores[m_current_frame_index], m_cmd_fences[m_current_frame_index], wait_stages);
		else
			kvfSubmitCommandBuffer(RenderCore::Get().GetDevice(), m_cmd_buffers[m_current_frame_index], KVF_GRAPHICS_QUEUE, VK_NULL_HANDLE, VK_NULL_HANDLE, m_cmd_fences[m_current_frame_index], wait_stages);
		if(p_window)
			m_swapchain.Present(m_render_finished_semaphores[m_current_frame_index]);
		m_current_frame_index = (m_current_frame_index + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	void Renderer::Destroy() noexcept
	{
		MLX_PROFILE_FUNCTION();
		RenderCore::Get().WaitDeviceIdle();
		for(std::size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			kvfDestroySemaphore(RenderCore::Get().GetDevice(), m_image_available_semaphores[i]);
			DebugLog("Vulkan: image available semaphore destroyed");
			kvfDestroySemaphore(RenderCore::Get().GetDevice(), m_render_finished_semaphores[i]);
			DebugLog("Vulkan: render finished semaphore destroyed");
			kvfDestroyCommandBuffer(RenderCore::Get().GetDevice(), m_cmd_buffers[i]);
			DebugLog("Vulkan: command buffer destroyed");
			kvfDestroyFence(RenderCore::Get().GetDevice(), m_cmd_fences[i]);
			DebugLog("Vulkan: fence destroyed");
		}
		if(p_window)
			m_swapchain.Destroy();
	}
}
