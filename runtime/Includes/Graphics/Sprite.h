#ifndef __MLX_SPRITE__
#define __MLX_SPRITE__

#include <Maths/Vec3.h>
#include <Maths/Vec4.h>
#include <Graphics/Mesh.h>
#include <Renderer/Descriptor.h>
#include <Renderer/Image.h>

namespace mlx
{
	class Sprite
	{
		friend class Render2DPass;

		public:
			Sprite(NonOwningPtr<Texture> texture);

			inline void SetColor(Vec4f color) noexcept { m_color = color; }
			inline void SetPosition(Vec2f position) noexcept { m_position = position; }

			[[nodiscard]] MLX_FORCEINLINE const Vec4f& GetColor() const noexcept { return m_color; }
			[[nodiscard]] MLX_FORCEINLINE const Vec2f& GetPosition() const noexcept { return m_position; }
			[[nodiscard]] MLX_FORCEINLINE std::shared_ptr<Mesh> GetMesh() const { return p_mesh; }
			[[nodiscard]] MLX_FORCEINLINE NonOwningPtr<Texture> GetTexture() const { return p_texture; }

			inline ~Sprite() { if(p_set) p_set->ReturnDescriptorSetToPool(); }

		private:
			[[nodiscard]] inline bool IsSetInit() const noexcept { return p_set && p_set->IsInit(); }
			[[nodiscard]] inline VkDescriptorSet GetSet(std::size_t frame_index) const noexcept { return p_set ? p_set->GetSet(frame_index) : VK_NULL_HANDLE; }

			inline void UpdateDescriptorSet(std::shared_ptr<DescriptorSet> set)
			{
				p_set = RenderCore::Get().GetDescriptorPoolManager().GetAvailablePool().RequestDescriptorSet(set->GetShaderLayout(), set->GetShaderType());
			}

			inline void Bind(std::size_t frame_index, VkCommandBuffer cmd)
			{
				if(!p_set)
					return;
				p_set->SetImage(frame_index, 0, *p_texture);
				p_set->Update(frame_index, cmd);
			}

		private:
			std::shared_ptr<DescriptorSet> p_set;
			NonOwningPtr<Texture> p_texture;
			std::shared_ptr<Mesh> p_mesh;
			Vec4f m_color = Vec4f{ 1.0f, 1.0f, 1.0f, 1.0f };
			Vec2f m_position = Vec2f{ 0.0f, 0.0f };
	};
}

#endif
