#ifndef __MLX_SPRITE__
#define __MLX_SPRITE__

#include <Maths/Vec3.h>
#include <Maths/Vec4.h>
#include <Graphics/Mesh.h>
#include <Renderer/Descriptor.h>
#include <Renderer/Image.h>
#include <Graphics/Drawable.h>

namespace mlx
{
	class Sprite : public Drawable
	{
		friend class Render2DPass;

		public:
			Sprite(NonOwningPtr<Texture> texture);
			Sprite(std::shared_ptr<Mesh> mesh, NonOwningPtr<Texture> texture);

			MLX_FORCEINLINE void Update(VkCommandBuffer cmd) override
			{
				Verify((bool)p_texture, "a sprite has no texture attached (internal mlx issue, please report to the devs)");
				p_texture->Update(cmd);
			}

			[[nodiscard]] MLX_FORCEINLINE NonOwningPtr<Texture> GetTexture() const { return p_texture; }

			inline ~Sprite() = default;

		private:
			inline void Bind(std::size_t frame_index, VkCommandBuffer cmd) override
			{
				if(!p_set)
					return;
				p_set->SetImage(frame_index, 0, *p_texture);
				p_set->Update(frame_index, cmd);
			}

		private:
			NonOwningPtr<Texture> p_texture;
	};
}

#endif
