#include <PreCompiled.h>
#include <Core/Graphics.h>
#include <Core/Application.h>
#include <Core/Handles.h>

namespace mlx
{
	GraphicsSupport::GraphicsSupport(const mlx_window_create_info* info, int id) :
		m_put_pixel_manager(&m_renderer),
		m_id(id)
	{
		MLX_PROFILE_FUNCTION();

		if(info->render_target == nullptr)
		{
			p_window = std::make_shared<Window>(info);
			m_has_window = true;
		}
		else
			m_has_window = false;

		if(info->render_target != nullptr)
		{
			m_renderer.Init(info->render_target->texture);
			m_scene_renderer.Init(info->render_target->texture);
		}
		else
		{
			m_renderer.Init(p_window.get());
			m_scene_renderer.Init(nullptr);
		}
		p_scene = std::make_unique<Scene>();
	}

	void GraphicsSupport::Render() noexcept
	{
		MLX_PROFILE_FUNCTION();
		m_renderer.BeginFrame();
			m_draw_layer = 0;
			m_scene_renderer.Render(*p_scene, m_renderer);
		m_renderer.EndFrame();
		#ifdef GRAPHICS_MEMORY_DUMP
			// Dump memory usage to file every two seconds
			using namespace std::chrono_literals;
			static std::int64_t timer = static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
			if(std::chrono::duration<std::uint64_t>{ static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count()) - timer } >= 1s)
			{
				RenderCore::Get().GetAllocator().DumpMemoryToJson();
				timer = static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
			}
		#endif
	}

	void GraphicsSupport::ResetRenderData(mlx_color color) noexcept
	{
		MLX_PROFILE_FUNCTION();
		Vec4f vec_color = {
			static_cast<float>(color.r) / 255.0f,
			static_cast<float>(color.g) / 255.0f,
			static_cast<float>(color.b) / 255.0f,
			static_cast<float>(color.a) / 255.0f
		};
		p_scene->ResetScene(std::move(vec_color));
		m_put_pixel_manager.ResetRenderData();
		m_draw_layer = 0;
		m_pixelput_called = false;
	}

	void GraphicsSupport::PixelPut(int x, int y, mlx_color color) noexcept
	{
		MLX_PROFILE_FUNCTION();
		NonOwningPtr<Texture> texture = m_put_pixel_manager.DrawPixel(x, y, m_draw_layer, color);
		if(texture)
		{
			m_pixelput_called = true;
			Sprite& new_sprite = p_scene->CreateSprite(texture);
			new_sprite.SetPosition(Vec2f{ 0.0f, 0.0f });
		}
	}

	void GraphicsSupport::PixelPutArray(int x, int y, mlx_color* pixels, std::size_t pixels_size) noexcept
	{
		MLX_PROFILE_FUNCTION();
		NonOwningPtr<Texture> texture = m_put_pixel_manager.DrawPixelsArray(x, y, m_draw_layer, pixels, pixels_size);
		if(texture)
		{
			m_pixelput_called = true;
			Sprite& new_sprite = p_scene->CreateSprite(texture);
			new_sprite.SetPosition(Vec2f{ 0.0f, 0.0f });
		}
	}

	void GraphicsSupport::PixelPutRegion(int x, int y, int w, int h, mlx_color* pixels) noexcept
	{
		MLX_PROFILE_FUNCTION();
		NonOwningPtr<Texture> texture = m_put_pixel_manager.DrawPixelsRegion(x, y, w, h, m_draw_layer, pixels);
		if(texture)
		{
			m_pixelput_called = true;
			Sprite& new_sprite = p_scene->CreateSprite(texture);
			new_sprite.SetPosition(Vec2f{ 0.0f, 0.0f });
		}
	}

	void GraphicsSupport::StringPut(int x, int y, mlx_color color, std::string str)
	{
		MLX_PROFILE_FUNCTION();
		if(str.empty())
			return;

		Vec4f vec_color = {
			static_cast<float>(color.r) / 255.0f,
			static_cast<float>(color.g) / 255.0f,
			static_cast<float>(color.b) / 255.0f,
			static_cast<float>(color.a) / 255.0f,
		};

		NonOwningPtr<Text> text = p_scene->GetTextFromPositionAndColor(str, Vec2f{ static_cast<float>(x), static_cast<float>(y) }, vec_color);
		if(!text)
		{
			if(m_pixelput_called)
			{
				m_draw_layer++;
				m_pixelput_called = false;
			}
			Text& new_text = p_scene->CreateText(str);
			new_text.SetPosition(Vec2f{ static_cast<float>(x), static_cast<float>(y) });
			new_text.SetColor(std::move(vec_color));
		}
		else if(!p_scene->IsTextAtGivenDrawLayer(str, m_draw_layer))
			p_scene->BringToDrawLayer(text.Get(), m_draw_layer);
	}

	void GraphicsSupport::TexturePut(NonOwningPtr<Texture> texture, int x, int y, float scale_x, float scale_y, float angle)
	{
		MLX_PROFILE_FUNCTION();
		NonOwningPtr<Sprite> sprite = p_scene->GetSpriteFromTexturePositionScaleRotation(texture, Vec2f{ static_cast<float>(x), static_cast<float>(y) }, scale_x, scale_y, angle);
		if(!sprite)
		{
			if(m_pixelput_called)
			{
				m_draw_layer++;
				m_pixelput_called = false;
			}	
			Sprite& new_sprite = p_scene->CreateSprite(texture);
			new_sprite.SetCenter(Vec2f{ texture->GetWidth() * 0.5f, texture->GetHeight() * 0.5f });
			new_sprite.SetPosition(Vec2f{ static_cast<float>(x), static_cast<float>(y) });
			new_sprite.SetScale(Vec2f{ scale_x, scale_y });
			new_sprite.SetRotation(angle);
		}
		else if(!p_scene->IsTextureAtGivenDrawLayer(texture, m_draw_layer))	
			p_scene->BringToDrawLayer(sprite.Get(), m_draw_layer);
	}

	GraphicsSupport::~GraphicsSupport()
	{
		MLX_PROFILE_FUNCTION();
		RenderCore::Get().WaitDeviceIdle();
		p_scene.reset();
		m_scene_renderer.Destroy();
		m_renderer.Destroy();
		if(p_window)
			p_window->Destroy();
	}
}
