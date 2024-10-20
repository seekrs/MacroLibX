#include <PreCompiled.h>

#include <Graphics/PutPixelManager.h>
#include <Renderer/Renderer.h>

namespace mlx
{
	NonOwningPtr<Texture> PutPixelManager::DrawPixel(int x, int y, bool insert_new_texture, std::uint32_t color)
	{
		Verify((bool)p_renderer, "invalid renderer pointer");

		VkExtent2D swapchain_extent = kvfGetSwapchainImagesSize(p_renderer->GetSwapchain());
		if(insert_new_texture)
		{
			#ifdef DEBUG
				Texture& texture = m_textures.emplace_back(CPUBuffer{}, swapchain_extent.width, swapchain_extent.height, VK_FORMAT_R8G8B8A8_SRGB, false, "mlx_put_pixel_layer_" + std::to_string(m_textures.size()));
			#else
				Texture& texture = m_textures.emplace_back(CPUBuffer{}, swapchain_extent.width, swapchain_extent.height, VK_FORMAT_R8G8B8A8_SRGB, false, std::string_view{});
			#endif
			texture.Clear(VK_NULL_HANDLE, Vec4f{ 0.0f });
		}
		if(!m_textures.empty())
		{
			m_textures.back().SetPixel(x, y, color);
			return (insert_new_texture ? &m_textures.back() : nullptr);
		}
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
