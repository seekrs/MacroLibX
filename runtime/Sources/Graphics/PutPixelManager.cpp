#include <PreCompiled.h>

#include <Graphics/PutPixelManager.h>
#include <Renderer/Renderer.h>

namespace mlx
{
	NonOwningPtr<Texture> PutPixelManager::DrawPixel(int x, int y, std::uint64_t draw_layer, std::uint32_t color)
	{
		Verify((bool)p_renderer, "invalid renderer pointer");

		VkExtent2D extent;
		if(p_renderer->GetWindow())
			extent = kvfGetSwapchainImagesSize(p_renderer->GetSwapchain().Get());
		else if(p_renderer->GetRenderTarget())
			extent = VkExtent2D{ .width = p_renderer->GetRenderTarget()->GetWidth(), .height = p_renderer->GetRenderTarget()->GetHeight() };
		else
			FatalError("a renderer was created without window nor render target attached (wtf)");

		#ifdef DEBUG
			auto res = m_textures.try_emplace(draw_layer, CPUBuffer{}, extent.width, extent.height, VK_FORMAT_R8G8B8A8_SRGB, false, "mlx_put_pixel_layer_" + std::to_string(draw_layer));
		#else
			auto res = m_textures.try_emplace(draw_layer, CPUBuffer{}, extent.width, extent.height, VK_FORMAT_R8G8B8A8_SRGB, false, std::string_view{});
		#endif
		if(res.second)
			res.first->second.Clear(VK_NULL_HANDLE, Vec4f{ 0.0f });
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
