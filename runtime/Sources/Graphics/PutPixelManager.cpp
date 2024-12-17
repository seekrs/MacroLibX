#include <PreCompiled.h>

#include <Graphics/PutPixelManager.h>
#include <Renderer/Renderer.h>
#include <Utils/Bits.h>

namespace mlx
{
	NonOwningPtr<Texture> PutPixelManager::DrawPixel(int x, int y, std::uint64_t draw_layer, mlx_color color)
	{
		MLX_PROFILE_FUNCTION();
		bool is_newlayer;
		NonOwningPtr<Texture> layer = GetLayer(draw_layer, is_newlayer);
		if(!layer)
			return nullptr;
		layer->SetPixel(x, y, color);
		return (is_newlayer ? layer : nullptr);
	}

	NonOwningPtr<Texture> PutPixelManager::DrawPixelsArray(int x, int y, std::uint64_t draw_layer, mlx_color* pixels, std::size_t pixels_size)
	{
		MLX_PROFILE_FUNCTION();
		bool is_newlayer;
		NonOwningPtr<Texture> layer = GetLayer(draw_layer, is_newlayer);
		if(!layer)
			return nullptr;
		layer->SetLinearRegion(x, y, pixels_size, pixels);
		return (is_newlayer ? layer : nullptr);
	}

	NonOwningPtr<Texture> PutPixelManager::DrawPixelsRegion(int x, int y, int w, int h, std::uint64_t draw_layer, mlx_color* pixels)
	{
		MLX_PROFILE_FUNCTION();
		bool is_newlayer;
		NonOwningPtr<Texture> layer = GetLayer(draw_layer, is_newlayer);
		if(!layer)
			return nullptr;
		layer->SetRegion(x, y, w, h, pixels);
		return (is_newlayer ? layer : nullptr);
	}

	NonOwningPtr<Texture> PutPixelManager::GetLayer(std::uint64_t draw_layer, bool& is_newlayer)
	{
		MLX_PROFILE_FUNCTION();
		Verify((bool)p_renderer, "invalid renderer pointer");

		auto it = m_placements.find(draw_layer);
		if(it != m_placements.end())
		{
			is_newlayer = false;
			return it->second;
		}
		is_newlayer = true;

		if(m_current_texture_index >= m_textures.size())
		{
			VkExtent2D extent{ .width = 0, .height = 0 };
			if(p_renderer->GetWindow())
				extent = kvfGetSwapchainImagesSize(p_renderer->GetSwapchain().Get());
			else if(p_renderer->GetRenderTarget())
				extent = VkExtent2D{ .width = p_renderer->GetRenderTarget()->GetWidth(), .height = p_renderer->GetRenderTarget()->GetHeight() };
			else
				FatalError("a renderer was created without window nor render target attached (wtf)");

			#ifdef DEBUG
				m_textures.push_back(std::make_unique<Texture>(CPUBuffer{}, extent.width, extent.height, VK_FORMAT_R8G8B8A8_SRGB, false, "mlx_put_pixel_layer_" + std::to_string(m_current_texture_index)));
			#else
				m_textures.push_back(std::make_unique<Texture>(CPUBuffer{}, extent.width, extent.height, VK_FORMAT_R8G8B8A8_SRGB, false, std::string_view{}));
			#endif
		}
		try
		{
			m_placements[draw_layer] = m_textures.at(m_current_texture_index).get();
			m_textures.at(m_current_texture_index)->Clear(VK_NULL_HANDLE, Vec4f{ 0.f });
			NonOwningPtr<Texture> texture = m_textures.at(m_current_texture_index).get();
			m_current_texture_index++;
			return texture;
		}
		catch(...)
		{
			Error("PutPixelManager: invalid texture index; % is not in range of 0-% (internal mlx issue, please report to devs)", m_current_texture_index, m_textures.size());
			return nullptr;
		}
	}

	void PutPixelManager::ResetRenderData()
	{
		m_placements.clear();
		m_current_texture_index = 0;
	}
}
