#include <PreCompiled.h>

#include <Graphics/PutPixelManager.h>
#include <Renderer/Renderer.h>

namespace mlx
{
	NonOwningPtr<Texture> PutPixelManager::DrawPixel(int x, int y, std::uint64_t z, std::uint32_t color)
	{
		Verify((bool)p_renderer, "invalid renderer pointer");
		auto it = m_textures.find(z);
		if(it == m_textures.end())
		{
			VkExtent2D swapchain_extent = kvfGetSwapchainImagesSize(p_renderer->GetSwapchain());
			Texture& texture = m_textures[z] = Texture({}, swapchain_extent.width, swapchain_extent.height);
			texture.SetPixel(x, y, color);
			return &texture;
		}
		it->second.SetPixel(x, y, color);
		return nullptr;
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
