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
		struct ResizeEventBroadcast : public EventBase
		{
			Event What() const override { return Event::ResizeEventCode; }
		};

		struct FrameBeginEventBroadcast : public EventBase
		{
			Event What() const override { return Event::FrameBeginEventCode; }
		};

		struct DescriptorPoolResetEventBroadcast : public EventBase
		{
			Event What() const override { return Event::DescriptorPoolResetEventCode; }
		};
	}

	void Renderer::Init(NonOwningPtr<Window> window)
	{
		MLX_PROFILE_FUNCTION();
		func::function<void(const EventBase&)> functor = [this](const EventBase& event)
		{
			if(event.What() == Event::ResizeEventCode)
				this->RequireFramebufferResize();
		};
		EventBus::RegisterListener({ functor, "__MlxRenderer" + std::to_string(reinterpret_cast<std::uintptr_t>(this)) });

		p_window = window;

		m_surface = p_window->CreateVulkanSurface(RenderCore::Get().GetInstance());
		DebugLog("Vulkan : surface created");

		CreateSwapchain();

		for(std::size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			m_image_available_semaphores[i] = kvfCreateSemaphore(RenderCore::Get().GetDevice());
			DebugLog("Vulkan : image available semaphore created");
			m_render_finished_semaphores[i] = kvfCreateSemaphore(RenderCore::Get().GetDevice());
			DebugLog("Vulkan : render finished semaphore created");
			m_cmd_buffers[i] = kvfCreateCommandBuffer(RenderCore::Get().GetDevice());
			DebugLog("Vulkan : command buffer created");
			m_cmd_fences[i] = kvfCreateFence(RenderCore::Get().GetDevice());
			DebugLog("Vulkan : fence created");
		}
	}

	bool Renderer::BeginFrame()
	{
		MLX_PROFILE_FUNCTION();
		kvfWaitForFence(RenderCore::Get().GetDevice(), m_cmd_fences[m_current_frame_index]);
		static bool first_run = true;
		if(!first_run)
		{
			m_descriptor_pool_manager.ResetPoolsFromFrameIndex(m_current_frame_index);
			EventBus::SendBroadcast(Internal::DescriptorPoolResetEventBroadcast{});
		}
		first_run = false;
		VkResult result = RenderCore::Get().vkAcquireNextImageKHR(RenderCore::Get().GetDevice(), m_swapchain, UINT64_MAX, m_image_available_semaphores[m_current_frame_index], VK_NULL_HANDLE, &m_swapchain_image_index);
		if(result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			DestroySwapchain();
			CreateSwapchain();
			EventBus::SendBroadcast(Internal::ResizeEventBroadcast{});
			return false;
		}
		else if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			FatalError("Vulkan error : failed to acquire swapchain image, %", kvfVerbaliseVkResult(result));

		RenderCore::Get().vkResetCommandBuffer(m_cmd_buffers[m_current_frame_index], 0);
		kvfBeginCommandBuffer(m_cmd_buffers[m_current_frame_index], 0);
		m_drawcalls = 0;
		m_polygons_drawn = 0;
		EventBus::SendBroadcast(Internal::FrameBeginEventBroadcast{});
		return true;
	}

	void Renderer::EndFrame()
	{
		MLX_PROFILE_FUNCTION();
		VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		kvfEndCommandBuffer(m_cmd_buffers[m_current_frame_index]);
		kvfSubmitCommandBuffer(RenderCore::Get().GetDevice(), m_cmd_buffers[m_current_frame_index], KVF_GRAPHICS_QUEUE, m_render_finished_semaphores[m_current_frame_index], m_image_available_semaphores[m_current_frame_index], m_cmd_fences[m_current_frame_index], wait_stages);
		if(!kvfQueuePresentKHR(RenderCore::Get().GetDevice(), m_render_finished_semaphores[m_current_frame_index], m_swapchain, m_swapchain_image_index) || m_framebuffers_resize)
		{
			m_framebuffers_resize = false;
			DestroySwapchain();
			CreateSwapchain();
			EventBus::SendBroadcast(Internal::ResizeEventBroadcast{});
		}
		m_current_frame_index = (m_current_frame_index + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	void Renderer::CreateSwapchain()
	{
		MLX_PROFILE_FUNCTION();
		Vec2ui drawable_size = p_window->GetVulkanDrawableSize();
		VkExtent2D extent = { drawable_size.x, drawable_size.y };
		m_swapchain = kvfCreateSwapchainKHR(RenderCore::Get().GetDevice(), RenderCore::Get().GetPhysicalDevice(), m_surface, extent, false);

		std::uint32_t images_count = kvfGetSwapchainImagesCount(m_swapchain);
		std::vector<VkImage> tmp(images_count);
		m_swapchain_images.resize(images_count);
		RenderCore::Get().vkGetSwapchainImagesKHR(RenderCore::Get().GetDevice(), m_swapchain, &images_count, tmp.data());
		for(std::size_t i = 0; i < images_count; i++)
		{
			#ifdef DEBUG
				m_swapchain_images[i].Init(tmp[i], kvfGetSwapchainImagesFormat(m_swapchain), extent.width, extent.height, VK_IMAGE_LAYOUT_UNDEFINED, "mlx_swapchain_image_" + std::to_string(i));
			#else
				m_swapchain_images[i].Init(tmp[i], kvfGetSwapchainImagesFormat(m_swapchain), extent.width, extent.height, VK_IMAGE_LAYOUT_UNDEFINED, {});
			#endif
			m_swapchain_images[i].TransitionLayout(VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
			m_swapchain_images[i].CreateImageView(VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_COLOR_BIT);
		}
		DebugLog("Vulkan : swapchain created");
	}

	void Renderer::DestroySwapchain()
	{
		MLX_PROFILE_FUNCTION();
		RenderCore::Get().WaitDeviceIdle();
		for(Image& img : m_swapchain_images)
			img.DestroyImageView();
		kvfDestroySwapchainKHR(RenderCore::Get().GetDevice(), m_swapchain);
		DebugLog("Vulkan : swapchain destroyed");
	}

	void Renderer::Destroy() noexcept
	{
		MLX_PROFILE_FUNCTION();
		RenderCore::Get().WaitDeviceIdle();

		for(std::size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			kvfDestroySemaphore(RenderCore::Get().GetDevice(), m_image_available_semaphores[i]);
			DebugLog("Vulkan : image available semaphore destroyed");
			kvfDestroySemaphore(RenderCore::Get().GetDevice(), m_render_finished_semaphores[i]);
			DebugLog("Vulkan : render finished semaphore destroyed");
			kvfDestroyFence(RenderCore::Get().GetDevice(), m_cmd_fences[i]);
			DebugLog("Vulkan : fence destroyed");
		}

		m_descriptor_pool_manager.Destroy();
		DestroySwapchain();
		RenderCore::Get().vkDestroySurfaceKHR(RenderCore::Get().GetInstance(), m_surface, nullptr);
		DebugLog("Vulkan : surface destroyed");
		m_surface = VK_NULL_HANDLE;
	}
}
