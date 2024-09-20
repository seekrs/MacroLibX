#ifndef __MLX_MEMORY__
#define __MLX_MEMORY__

namespace mlx
{
	class MemManager
	{
		public:
			MemManager();

			static void* Malloc(std::size_t size);
			static void* Calloc(std::size_t n, std::size_t size);
			static void* Realloc(void* ptr, std::size_t size);
			static void Free(void* ptr);

			inline static bool IsInit() noexcept { return s_instance != nullptr; }
			inline static MemManager& Get() noexcept { return *s_instance; }

			~MemManager();

		private:
			static MemManager* s_instance;
			inline static std::list<void*> s_blocks;
	};
}

#endif
