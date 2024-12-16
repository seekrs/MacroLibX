#include <PreCompiled.h>

#include <Graphics/PutPixelManager.h>
#include <Renderer/Renderer.h>

namespace mlx
{
	NonOwningPtr<Texture> PutPixelManager::DrawPixel(int x, int y, std::uint64_t draw_layer, int color)
	{
		Verify((bool)p_renderer, "invalid renderer pointer");

		VkExtent2D extent;
		if(p_renderer->GetWindow())
			extent = kvfGetSwapchainImagesSize(p_renderer->GetSwapchain().Get());
		else if(p_renderer->GetRenderTarget())
			extent = VkExtent2D{ .width = p_renderer->GetRenderTarget()->GetWidth(), .height = p_renderer->GetRenderTarget()->GetHeight() };
		else
			FatalError("a renderer was created without window nor render target attached (wtf)");

		auto it = m_placements.find(draw_layer);
		if(it != m_placements.end())
		{
			it->second->SetPixel(x, y, color);
			return nullptr;
		}

		bool adjusment = false;
		if(m_current_texture_index >= m_textures.size())
		{
			#ifdef DEBUG
				m_textures.push_back(std::make_unique<Texture>(CPUBuffer{}, extent.width, extent.height, VK_FORMAT_R8G8B8A8_SRGB, false, "mlx_put_pixel_layer_" + std::to_string(draw_layer)));
			#else
				m_textures.push_back(std::make_unique<Texture>(CPUBuffer{}, extent.width, extent.height, VK_FORMAT_R8G8B8A8_SRGB, false, std::string_view{}));
			#endif
			m_current_texture_index++;
			adjusment = true;
		}
		try
		{
			m_placements[draw_layer] = m_textures.at(m_current_texture_index - adjusment).get();
			m_textures.at(m_current_texture_index - adjusment)->Clear(VK_NULL_HANDLE, Vec4f{ 0.0f });
			m_textures.at(m_current_texture_index - adjusment)->SetPixel(x, y, color);
			return m_textures.at(m_current_texture_index - adjusment).get();
		}
		catch(...)
		{
			Error("PutPixelManager: invalid texture index; % is not in range of 0-% (internal mlx issue, please report to devs)", m_current_texture_index - 1, m_textures.size());
			return nullptr;
		}
	}

	void PutPixelManager::ResetRenderData()
	{
		m_placements.clear();
		for(auto& texture : m_textures)
			texture->Clear(VK_NULL_HANDLE, Vec4f{ 0.0f });
		m_current_texture_index = 0;
	}
}
