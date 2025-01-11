#ifndef __MLX_DRAWABLE__
#define __MLX_DRAWABLE__

#include <Graphics/Enums.h>
#include <Maths/Quaternions.h>
#include <Maths/EulerAngles.h>

namespace mlx
{
	class Drawable
	{
		friend class Render2DPass;

		public:
			inline Drawable(DrawableType type) : m_type(type) {}

			inline void SetColor(Vec4f color) noexcept { m_color = std::move(color); }
			inline void SetPosition(Vec2f position) noexcept { m_position = std::move(position); }
			inline void SetScale(Vec2f scale) noexcept { m_scale = std::move(scale); }
			inline void SetRotation(float rotation) noexcept { m_rotation = EulerAnglesf{ 0.0f, 0.0f, rotation }; }
			inline void SetCenter(Vec2f center) noexcept { m_center = std::move(center); }

			inline virtual void Update([[maybe_unused]] VkCommandBuffer cmd) {}

			[[nodiscard]] MLX_FORCEINLINE const Vec4f& GetColor() const noexcept { return m_color; }
			[[nodiscard]] MLX_FORCEINLINE const Vec2f& GetPosition() const noexcept { return m_position; }
			[[nodiscard]] MLX_FORCEINLINE const Vec2f& GetScale() const noexcept { return m_scale; }
			[[nodiscard]] MLX_FORCEINLINE const Quatf& GetRotation() const noexcept { return m_rotation; }
			[[nodiscard]] MLX_FORCEINLINE const Vec2f& GetCenter() const noexcept { return m_center; }
			[[nodiscard]] MLX_FORCEINLINE std::shared_ptr<Mesh> GetMesh() const { return p_mesh; }
			[[nodiscard]] MLX_FORCEINLINE DrawableType GetType() const noexcept { return m_type; }

			inline virtual ~Drawable() { if(p_set) p_set->ReturnDescriptorSetToPool(); }

		protected:
			[[nodiscard]] inline bool IsSetInit() const noexcept { return p_set && p_set->IsInit(); }
			[[nodiscard]] inline VkDescriptorSet GetSet(std::size_t frame_index) const noexcept { return p_set ? p_set->GetSet(frame_index) : VK_NULL_HANDLE; }

			inline void UpdateDescriptorSet(std::shared_ptr<DescriptorSet> set)
			{
				p_set = RenderCore::Get().GetDescriptorPoolManager().GetAvailablePool().RequestDescriptorSet(set->GetShaderLayout(), set->GetShaderType());
			}

			virtual void Bind(std::size_t frame_index, VkCommandBuffer cmd) = 0;

		protected:
			std::shared_ptr<DescriptorSet> p_set;
			std::shared_ptr<Mesh> p_mesh;
			Quatf m_rotation = Quatf::Identity();
			Vec4f m_color = Vec4f{ 1.0f, 1.0f, 1.0f, 1.0f };
			Vec2f m_position = Vec2f{ 0.0f, 0.0f };
			Vec2f m_scale = Vec2f{ 1.0f, 1.0f };
			Vec2f m_center = Vec2f{ 0.0f, 0.0f };
			DrawableType m_type;
	};
}

#endif
