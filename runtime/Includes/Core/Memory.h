#ifndef __MLX_MEMORY__
#define __MLX_MEMORY__

namespace mlx
{
	class MemManager
	{
		public:
			MemManager();

			static void* Malloc(std::size_t size);
			static void* AlignedMalloc(std::size_t alignment, std::size_t size);
			static void* Calloc(std::size_t n, std::size_t size);
			static void* AlignedCalloc(std::size_t alignment, std::size_t n, std::size_t size);
			static void* Realloc(void* ptr, std::size_t size);
			static void* AlignedRealloc(void* ptr, std::size_t alignment, std::size_t size);
			static void Free(void* ptr);

			static inline bool IsInit() noexcept { return s_instance != nullptr; }
			static inline MemManager& Get() noexcept { return *s_instance; }

			~MemManager();

		private:
			struct Descriptor
			{
				void* ptr;
				std::size_t size;
				bool aligned;

				Descriptor(void* ptr, std::size_t size, bool aligned) : ptr(ptr), size(size), aligned(aligned) {}
			};

		private:
			static MemManager* s_instance;
			inline static std::vector<Descriptor> s_blocks;
	};
}

#endif
