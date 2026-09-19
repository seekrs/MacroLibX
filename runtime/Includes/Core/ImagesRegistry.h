#ifndef __MLX_CORE_IMAGES_REGISTRY__
#define __MLX_CORE_IMAGES_REGISTRY__

namespace mlx
{
	class ImageRegistry
	{
		public:
			ImageRegistry() = default;

			inline void RegisterTexture(NonOwningPtr<class Texture> texture)
			{
				m_textures_registry.insert(texture);
			}

			inline void UnregisterTexture(NonOwningPtr<class Texture> texture)
			{
				m_textures_registry.erase(texture);
			}

			inline bool IsTextureKnown(NonOwningPtr<class Texture> texture)
			{
				return m_textures_registry.find(texture) != m_textures_registry.end();
			}

			~ImageRegistry() = default;

		private:
			std::unordered_set<NonOwningPtr<class Texture>> m_textures_registry;
	};
}

#endif
