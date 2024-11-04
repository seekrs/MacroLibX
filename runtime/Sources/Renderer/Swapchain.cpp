#include <PreCompiled.h>

#include <Platform/Window.h>
#include <Renderer/Swapchain.h>
#include <Renderer/RenderCore.h>
#include <Core/Logs.h>
#include <Core/EventBus.h>

namespace mlx
{
	namespace Internal
	{
		struct ResizeEventBroadcast : public EventBase
		{
			Event What() const override { return Event::ResizeEventCode; }
		};
	}

	void Swapchain::Init(NonOwningPtr<Window> window)
	{
		p_window = window;
		m_surface = window->CreateVulkanSurface(RenderCore::Get().GetInstance());
		DebugLog("Vulkan: surface created");
		CreateSwapchain();
	}

	void Swapchain::AquireFrame(VkSemaphore signal)
	{
		if(m_resize)
		{
			RenderCore::Get().WaitDeviceIdle();
			CreateSwapchain();
			EventBus::SendBroadcast(Internal::ResizeEventBroadcast{});
		}

		VkResult result = RenderCore::Get().vkAcquireNextImageKHR(RenderCore::Get().GetDevice(), m_swapchain, UINT64_MAX, signal, VK_NULL_HANDLE, &m_current_image_index);
		if(result == VK_SUBOPTIMAL_KHR)
			m_resize = true; // Recreate Swapchain next time
		else if(result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			m_resize = true;
			AquireFrame(signal);
		}
		else if(result != VK_SUCCESS)
			FatalError("Vulkan: failed to acquire swapchain image, %", kvfVerbaliseVkResult(result));
	}

	void Swapchain::Present(VkSemaphore wait) noexcept
	{
		if(!kvfQueuePresentKHR(RenderCore::Get().GetDevice(), wait, m_swapchain, m_current_image_index))
			m_resize = true;
	}

	void Swapchain::Destroy()
	{
		RenderCore::Get().WaitDeviceIdle();

		for(Image& img : m_swapchain_images)
			img.DestroyImageView();
		kvfDestroySwapchainKHR(RenderCore::Get().GetDevice(), m_swapchain);

		RenderCore::Get().vkDestroySurfaceKHR(RenderCore::Get().GetInstance(), m_surface, nullptr);
		m_surface = VK_NULL_HANDLE;
		DebugLog("Vulkan: surface destroyed");
	}

	void Swapchain::CreateSwapchain()
	{
		for(Image& img : m_swapchain_images)
			img.DestroyImageView();
		m_swapchain_images.clear();

		VkExtent2D extent;
		do
		{
			Vec2ui size = p_window->GetVulkanDrawableSize();
			extent = { size.x, size.y };
		} while(extent.width == 0 || extent.height == 0);

		VkSwapchainKHR old_swapchain = m_swapchain;
		m_swapchain = kvfCreateSwapchainKHR(RenderCore::Get().GetDevice(), RenderCore::Get().GetPhysicalDevice(), m_surface, extent, VK_NULL_HANDLE, true);
		if(old_swapchain != VK_NULL_HANDLE)
			kvfDestroySwapchainKHR(RenderCore::Get().GetDevice(), old_swapchain);

		m_images_count = kvfGetSwapchainImagesCount(m_swapchain);
		m_min_images_count = kvfGetSwapchainMinImagesCount(m_swapchain);
		std::vector<VkImage> tmp(m_images_count);
		m_swapchain_images.resize(m_images_count);
		RenderCore::Get().vkGetSwapchainImagesKHR(RenderCore::Get().GetDevice(), m_swapchain, &m_images_count, tmp.data());
		VkCommandBuffer cmd = kvfCreateCommandBuffer(RenderCore::Get().GetDevice());
		kvfBeginCommandBuffer(cmd, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
		for(std::size_t i = 0; i < m_images_count; i++)
		{
			#ifdef DEBUG
				m_swapchain_images[i].Init(tmp[i], kvfGetSwapchainImagesFormat(m_swapchain), extent.width, extent.height, VK_IMAGE_LAYOUT_UNDEFINED, "mlx_swapchain_image_" + std::to_string(i));
			#else
				m_swapchain_images[i].Init(tmp[i], kvfGetSwapchainImagesFormat(m_swapchain), extent.width, extent.height, VK_IMAGE_LAYOUT_UNDEFINED, {});
			#endif
			m_swapchain_images[i].TransitionLayout(VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, cmd);
			m_swapchain_images[i].CreateImageView(VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_COLOR_BIT);
		}
		kvfEndCommandBuffer(cmd);
		VkFence fence = kvfCreateFence(RenderCore::Get().GetDevice());
		kvfSubmitSingleTimeCommandBuffer(RenderCore::Get().GetDevice(), cmd, KVF_GRAPHICS_QUEUE, fence);
		kvfDestroyFence(RenderCore::Get().GetDevice(), fence);
		DebugLog("Vulkan: swapchain created");
	}
}
