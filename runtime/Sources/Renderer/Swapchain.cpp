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

	
	std::string VulkanFormatName(VkFormat format)
	{
		#define STRINGIFY(x) case x: return #x

		switch(format)
		{
			STRINGIFY(VK_FORMAT_UNDEFINED);
			STRINGIFY(VK_FORMAT_R4G4_UNORM_PACK8);
			STRINGIFY(VK_FORMAT_R4G4B4A4_UNORM_PACK16);
			STRINGIFY(VK_FORMAT_B4G4R4A4_UNORM_PACK16);
			STRINGIFY(VK_FORMAT_R5G6B5_UNORM_PACK16);
			STRINGIFY(VK_FORMAT_B5G6R5_UNORM_PACK16);
			STRINGIFY(VK_FORMAT_R5G5B5A1_UNORM_PACK16);
			STRINGIFY(VK_FORMAT_B5G5R5A1_UNORM_PACK16);
			STRINGIFY(VK_FORMAT_A1R5G5B5_UNORM_PACK16);
			STRINGIFY(VK_FORMAT_R8_UNORM);
			STRINGIFY(VK_FORMAT_R8_SNORM);
			STRINGIFY(VK_FORMAT_R8_USCALED);
			STRINGIFY(VK_FORMAT_R8_SSCALED);
			STRINGIFY(VK_FORMAT_R8_UINT);
			STRINGIFY(VK_FORMAT_R8_SINT);
			STRINGIFY(VK_FORMAT_R8_SRGB);
			STRINGIFY(VK_FORMAT_R8G8_UNORM);
			STRINGIFY(VK_FORMAT_R8G8_SNORM);
			STRINGIFY(VK_FORMAT_R8G8_USCALED);
			STRINGIFY(VK_FORMAT_R8G8_SSCALED);
			STRINGIFY(VK_FORMAT_R8G8_UINT);
			STRINGIFY(VK_FORMAT_R8G8_SINT);
			STRINGIFY(VK_FORMAT_R8G8_SRGB);
			STRINGIFY(VK_FORMAT_R8G8B8_UNORM);
			STRINGIFY(VK_FORMAT_R8G8B8_SNORM);
			STRINGIFY(VK_FORMAT_R8G8B8_USCALED);
			STRINGIFY(VK_FORMAT_R8G8B8_SSCALED);
			STRINGIFY(VK_FORMAT_R8G8B8_UINT);
			STRINGIFY(VK_FORMAT_R8G8B8_SINT);
			STRINGIFY(VK_FORMAT_R8G8B8_SRGB);
			STRINGIFY(VK_FORMAT_B8G8R8_UNORM);
			STRINGIFY(VK_FORMAT_B8G8R8_SNORM);
			STRINGIFY(VK_FORMAT_B8G8R8_USCALED);
			STRINGIFY(VK_FORMAT_B8G8R8_SSCALED);
			STRINGIFY(VK_FORMAT_B8G8R8_UINT);
			STRINGIFY(VK_FORMAT_B8G8R8_SINT);
			STRINGIFY(VK_FORMAT_B8G8R8_SRGB);
			STRINGIFY(VK_FORMAT_R8G8B8A8_UNORM);
			STRINGIFY(VK_FORMAT_R8G8B8A8_SNORM);
			STRINGIFY(VK_FORMAT_R8G8B8A8_USCALED);
			STRINGIFY(VK_FORMAT_R8G8B8A8_SSCALED);
			STRINGIFY(VK_FORMAT_R8G8B8A8_UINT);
			STRINGIFY(VK_FORMAT_R8G8B8A8_SINT);
			STRINGIFY(VK_FORMAT_R8G8B8A8_SRGB);
			STRINGIFY(VK_FORMAT_B8G8R8A8_UNORM);
			STRINGIFY(VK_FORMAT_B8G8R8A8_SNORM);
			STRINGIFY(VK_FORMAT_B8G8R8A8_USCALED);
			STRINGIFY(VK_FORMAT_B8G8R8A8_SSCALED);
			STRINGIFY(VK_FORMAT_B8G8R8A8_UINT);
			STRINGIFY(VK_FORMAT_B8G8R8A8_SINT);
			STRINGIFY(VK_FORMAT_B8G8R8A8_SRGB);
			STRINGIFY(VK_FORMAT_A8B8G8R8_UNORM_PACK32);
			STRINGIFY(VK_FORMAT_A8B8G8R8_SNORM_PACK32);
			STRINGIFY(VK_FORMAT_A8B8G8R8_USCALED_PACK32);
			STRINGIFY(VK_FORMAT_A8B8G8R8_SSCALED_PACK32);
			STRINGIFY(VK_FORMAT_A8B8G8R8_UINT_PACK32);
			STRINGIFY(VK_FORMAT_A8B8G8R8_SINT_PACK32);
			STRINGIFY(VK_FORMAT_A8B8G8R8_SRGB_PACK32);
			STRINGIFY(VK_FORMAT_A2R10G10B10_UNORM_PACK32);
			STRINGIFY(VK_FORMAT_A2R10G10B10_SNORM_PACK32);
			STRINGIFY(VK_FORMAT_A2R10G10B10_USCALED_PACK32);
			STRINGIFY(VK_FORMAT_A2R10G10B10_SSCALED_PACK32);
			STRINGIFY(VK_FORMAT_A2R10G10B10_UINT_PACK32);
			STRINGIFY(VK_FORMAT_A2R10G10B10_SINT_PACK32);
			STRINGIFY(VK_FORMAT_A2B10G10R10_UNORM_PACK32);
			STRINGIFY(VK_FORMAT_A2B10G10R10_SNORM_PACK32);
			STRINGIFY(VK_FORMAT_A2B10G10R10_USCALED_PACK32);
			STRINGIFY(VK_FORMAT_A2B10G10R10_SSCALED_PACK32);
			STRINGIFY(VK_FORMAT_A2B10G10R10_UINT_PACK32);
			STRINGIFY(VK_FORMAT_A2B10G10R10_SINT_PACK32);
			STRINGIFY(VK_FORMAT_R16_UNORM);
			STRINGIFY(VK_FORMAT_R16_SNORM);
			STRINGIFY(VK_FORMAT_R16_USCALED);
			STRINGIFY(VK_FORMAT_R16_SSCALED);
			STRINGIFY(VK_FORMAT_R16_UINT);
			STRINGIFY(VK_FORMAT_R16_SINT);
			STRINGIFY(VK_FORMAT_R16_SFLOAT);
			STRINGIFY(VK_FORMAT_R16G16_UNORM);
			STRINGIFY(VK_FORMAT_R16G16_SNORM);
			STRINGIFY(VK_FORMAT_R16G16_USCALED);
			STRINGIFY(VK_FORMAT_R16G16_SSCALED);
			STRINGIFY(VK_FORMAT_R16G16_UINT);
			STRINGIFY(VK_FORMAT_R16G16_SINT);
			STRINGIFY(VK_FORMAT_R16G16_SFLOAT);
			STRINGIFY(VK_FORMAT_R16G16B16_UNORM);
			STRINGIFY(VK_FORMAT_R16G16B16_SNORM);
			STRINGIFY(VK_FORMAT_R16G16B16_USCALED);
			STRINGIFY(VK_FORMAT_R16G16B16_SSCALED);
			STRINGIFY(VK_FORMAT_R16G16B16_UINT);
			STRINGIFY(VK_FORMAT_R16G16B16_SINT);
			STRINGIFY(VK_FORMAT_R16G16B16_SFLOAT);
			STRINGIFY(VK_FORMAT_R16G16B16A16_UNORM);
			STRINGIFY(VK_FORMAT_R16G16B16A16_SNORM);
			STRINGIFY(VK_FORMAT_R16G16B16A16_USCALED);
			STRINGIFY(VK_FORMAT_R16G16B16A16_SSCALED);
			STRINGIFY(VK_FORMAT_R16G16B16A16_UINT);
			STRINGIFY(VK_FORMAT_R16G16B16A16_SINT);
			STRINGIFY(VK_FORMAT_R16G16B16A16_SFLOAT);
			STRINGIFY(VK_FORMAT_R32_UINT);
			STRINGIFY(VK_FORMAT_R32_SINT);
			STRINGIFY(VK_FORMAT_R32_SFLOAT);
			STRINGIFY(VK_FORMAT_R32G32_UINT);
			STRINGIFY(VK_FORMAT_R32G32_SINT);
			STRINGIFY(VK_FORMAT_R32G32_SFLOAT);
			STRINGIFY(VK_FORMAT_R32G32B32_UINT);
			STRINGIFY(VK_FORMAT_R32G32B32_SINT);
			STRINGIFY(VK_FORMAT_R32G32B32_SFLOAT);
			STRINGIFY(VK_FORMAT_R32G32B32A32_UINT);
			STRINGIFY(VK_FORMAT_R32G32B32A32_SINT);
			STRINGIFY(VK_FORMAT_R32G32B32A32_SFLOAT);
			STRINGIFY(VK_FORMAT_R64_UINT);
			STRINGIFY(VK_FORMAT_R64_SINT);
			STRINGIFY(VK_FORMAT_R64_SFLOAT);
			STRINGIFY(VK_FORMAT_R64G64_UINT);
			STRINGIFY(VK_FORMAT_R64G64_SINT);
			STRINGIFY(VK_FORMAT_R64G64_SFLOAT);
			STRINGIFY(VK_FORMAT_R64G64B64_UINT);
			STRINGIFY(VK_FORMAT_R64G64B64_SINT);
			STRINGIFY(VK_FORMAT_R64G64B64_SFLOAT);
			STRINGIFY(VK_FORMAT_R64G64B64A64_UINT);
			STRINGIFY(VK_FORMAT_R64G64B64A64_SINT);
			STRINGIFY(VK_FORMAT_R64G64B64A64_SFLOAT);
			STRINGIFY(VK_FORMAT_B10G11R11_UFLOAT_PACK32);
			STRINGIFY(VK_FORMAT_E5B9G9R9_UFLOAT_PACK32);

			default: return "Unknown format";
		}

		#undef STRINGIFY

		return "Unknown format"; // To avoid warnings
	}

	void Swapchain::Init(NonOwningPtr<Window> window)
	{
		p_window = window;
		CreateSwapchain();
	}

	void Swapchain::AquireFrame(VkSemaphore signal)
	{
		if(m_resize)
		{
			RenderCore::Get().WaitDeviceIdle();
			Destroy();
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
		if(m_swapchain == VK_NULL_HANDLE)
			return;
		RenderCore::Get().WaitDeviceIdle();

		for(Image& img : m_swapchain_images)
			img.DestroyImageView();
		m_swapchain_images.clear();
		kvfDestroySwapchainKHR(RenderCore::Get().GetDevice(), m_swapchain);
		m_swapchain = VK_NULL_HANDLE;
		DebugLog("Vulkan: swapchain destroyed");

		RenderCore::Get().vkDestroySurfaceKHR(RenderCore::Get().GetInstance(), m_surface, nullptr);
		m_surface = VK_NULL_HANDLE;
		DebugLog("Vulkan: surface destroyed");
	}

	void Swapchain::CreateSwapchain()
	{
		VkExtent2D extent;
		do
		{
			Vec2ui size = p_window->GetVulkanDrawableSize();
			extent = { size.x, size.y };
		} while(extent.width == 0 || extent.height == 0);

		m_surface = p_window->CreateVulkanSurface(RenderCore::Get().GetInstance());
		DebugLog("Vulkan: surface created");
		m_swapchain = kvfCreateSwapchainKHR(RenderCore::Get().GetDevice(), RenderCore::Get().GetPhysicalDevice(), m_surface, extent, m_swapchain, false, true);

		m_images_count = kvfGetSwapchainImagesCount(m_swapchain);
		m_min_images_count = kvfGetSwapchainMinImagesCount(m_swapchain);
		std::vector<VkImage> tmp(m_images_count);
		m_swapchain_images.resize(m_images_count);
		RenderCore::Get().vkGetSwapchainImagesKHR(RenderCore::Get().GetDevice(), m_swapchain, &m_images_count, tmp.data());
		VkCommandBuffer cmd = kvfCreateCommandBuffer(RenderCore::Get().GetDevice());
		kvfBeginCommandBuffer(cmd, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
		extent = kvfGetSwapchainImagesSize(m_swapchain); // fix the extent
		VkFormat format = kvfGetSwapchainImagesFormat(m_swapchain);
		for(std::size_t i = 0; i < m_images_count; i++)
		{
			#ifdef DEBUG
				m_swapchain_images[i].Init(tmp[i], format, extent.width, extent.height, VK_IMAGE_LAYOUT_UNDEFINED, "mlx_swapchain_image_" + std::to_string(i));
			#else
				m_swapchain_images[i].Init(tmp[i], format, extent.width, extent.height, VK_IMAGE_LAYOUT_UNDEFINED, {});
			#endif
			m_swapchain_images[i].TransitionLayout(VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, cmd);
			m_swapchain_images[i].CreateImageView(VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_COLOR_BIT);
		}
		kvfEndCommandBuffer(cmd);
		VkFence fence = kvfCreateFence(RenderCore::Get().GetDevice());
		kvfSubmitSingleTimeCommandBuffer(RenderCore::Get().GetDevice(), cmd, KVF_GRAPHICS_QUEUE, fence);
		kvfDestroyFence(RenderCore::Get().GetDevice(), fence);
		kvfDestroyCommandBuffer(RenderCore::Get().GetDevice(), cmd);
		m_resize = false;
		DebugLog("Vulkan: swapchain created with format %", VulkanFormatName(format));
	}
}
