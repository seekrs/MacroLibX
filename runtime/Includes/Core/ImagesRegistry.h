#ifndef __MLX_CORE_IMAGES_REGISTRY__
#define __MLX_CORE_IMAGES_REGISTRY__

namespace mlx
{
	class ImageRegistry
	{
		public:
			ImageRegistry() = default;

			inline void RegisterTexture(NonOwningPtr<class Texture> texture);
			inline void UnregisterTexture(NonOwningPtr<class Texture> texture);
			inline bool IsTextureKnown(NonOwningPtr<class Texture> texture);

			~ImageRegistry() = default;

		private:
			std::unordered_set<NonOwningPtr<class Texture>> m_textures_registry;
	};
}

#include <Core/ImagesRegistry.inl>

#endif
