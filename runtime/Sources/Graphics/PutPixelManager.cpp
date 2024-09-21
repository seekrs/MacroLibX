#include <PreCompiled.h>

#include <Graphics/PutPixelManager.h>
#include <Renderer/Renderer.h>

namespace mlx
{
	NonOwningPtr<Texture> PutPixelManager::DrawPixel(int x, int y, std::uint64_t z, std::uint32_t color)
	{
		Verify((bool)p_renderer, "invalid renderer pointer");

		VkExtent2D swapchain_extent = kvfGetSwapchainImagesSize(p_renderer->GetSwapchain());
		#ifdef DEBUG
			auto res = m_textures.try_emplace(z, CPUBuffer{}, swapchain_extent.width, swapchain_extent.height, VK_FORMAT_R8G8B8A8_SRGB, false, "mlx_put_pixel_layer_" + std::to_string(z));
		#else
			auto res = m_textures.try_emplace(z, CPUBuffer{}, swapchain_extent.width, swapchain_extent.height, VK_FORMAT_R8G8B8A8_SRGB, false, {});
		#endif
		res.first->second.SetPixel(x, y, color);
		return (res.second ? &res.first->second : nullptr);
	}

	void PutPixelManager::ResetRenderData()
	{
		m_textures.clear();
	}

	PutPixelManager::~PutPixelManager()
	{
		ResetRenderData();
	}
}
