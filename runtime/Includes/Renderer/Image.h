#ifndef __MLX_IMAGE__
#define __MLX_IMAGE__

#include <Maths/Vec4.h>
#include <Renderer/RenderCore.h>
#include <Renderer/Buffer.h>
#include <Utils/Buffer.h>
#include <Renderer/Enums.h>

namespace mlx
{
	class Image
	{
		public:
			Image() = default;

			inline void Init(VkImage image, VkFormat format, std::uint32_t width, std::uint32_t height, VkImageLayout layout, [[maybe_unused]] std::string_view debug_name) noexcept
			{
				m_image = image;
				m_format = format;
				m_width = width;
				m_height = height;
				m_layout = layout;
				#ifdef DEBUG
					m_debug_name = std::move(debug_name);
				#endif
			}

			void Init(ImageType type, std::uint32_t width, std::uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, bool is_multisampled, std::string_view debug_name);
			void CreateImageView(VkImageViewType type, VkImageAspectFlags aspectFlags, int layer_count = 1) noexcept;
			void CreateSampler() noexcept;
			void TransitionLayout(VkImageLayout new_layout, VkCommandBuffer cmd = VK_NULL_HANDLE);
			virtual void Clear(VkCommandBuffer cmd, Vec4f color);

			void DestroySampler() noexcept;
			void DestroyImageView() noexcept;
			virtual void Destroy() noexcept;

			[[nodiscard]] MLX_FORCEINLINE VkImage Get() const noexcept { return m_image; }
			[[nodiscard]] MLX_FORCEINLINE VmaAllocation GetAllocation() const noexcept { return m_allocation; }
			[[nodiscard]] MLX_FORCEINLINE VkImageView GetImageView() const noexcept { return m_image_view; }
			[[nodiscard]] MLX_FORCEINLINE VkFormat GetFormat() const noexcept { return m_format; }
			[[nodiscard]] MLX_FORCEINLINE VkImageTiling GetTiling() const noexcept { return m_tiling; }
			[[nodiscard]] MLX_FORCEINLINE VkImageLayout GetLayout() const noexcept { return m_layout; }
			[[nodiscard]] MLX_FORCEINLINE VkSampler GetSampler() const noexcept { return m_sampler; }
			[[nodiscard]] MLX_FORCEINLINE std::uint32_t GetWidth() const noexcept { return m_width; }
			[[nodiscard]] MLX_FORCEINLINE std::uint32_t GetHeight() const noexcept { return m_height; }
			[[nodiscard]] MLX_FORCEINLINE bool IsInit() const noexcept { return m_image != VK_NULL_HANDLE; }
			[[nodiscard]] MLX_FORCEINLINE ImageType GetType() const noexcept { return m_type; }

			#ifdef DEBUG
				[[nodiscard]] MLX_FORCEINLINE const std::string& GetDebugName() const { return m_debug_name; }
			#endif

			virtual ~Image() = default;

		protected:
			#ifdef DEBUG
				std::string m_debug_name;
			#endif
			VmaAllocation m_allocation;
			VkImage m_image = VK_NULL_HANDLE;
			VkImageView m_image_view = VK_NULL_HANDLE;
			VkSampler m_sampler = VK_NULL_HANDLE;
			VkFormat m_format;
			VkImageTiling m_tiling;
			VkImageLayout m_layout = VK_IMAGE_LAYOUT_UNDEFINED;
			ImageType m_type;
			std::uint32_t m_width = 0;
			std::uint32_t m_height = 0;
			bool m_is_multisampled = false;
	};

	class Texture: public Image
	{
		public:
			Texture() = default;
			Texture(CPUBuffer pixels, std::uint32_t width, std::uint32_t height, VkFormat format, bool is_multisampled, [[maybe_unused]] std::string_view debug_name)
			{
				Init(std::move(pixels), width, height, format, is_multisampled, std::move(debug_name));
			}

			void Init(CPUBuffer pixels, std::uint32_t width, std::uint32_t height, VkFormat format, bool is_multisampled, [[maybe_unused]] std::string_view debug_name);
			void Destroy() noexcept override;

			void SetPixel(int x, int y, mlx_color color) noexcept;
			void SetRegion(int x, int y, int w, int h, mlx_color* color) noexcept;
			void SetLinearRegion(int x, int y, std::size_t len, mlx_color* color) noexcept;
			mlx_color GetPixel(int x, int y) noexcept;
			void GetRegion(int x, int y, int w, int h, mlx_color* dst) noexcept;
			void Clear(VkCommandBuffer cmd, Vec4f color) override;

			void Update(VkCommandBuffer cmd);

			~Texture() override { Destroy(); }

		private:
			void OpenCPUBuffer();

		private:
			std::optional<GPUBuffer> m_staging_buffer;
			bool m_has_been_modified = false;
	};

	Texture* StbTextureLoad(const std::filesystem::path& file, int* w, int* h);
}

#endif
