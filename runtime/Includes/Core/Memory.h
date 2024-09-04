#ifndef __MLX_MEMORY__
#define __MLX_MEMORY__

namespace mlx
{
	class MemManager : public Singleton<MemManager>
	{
		friend class Singleton<MemManager>;

		public:
			static void* Malloc(std::size_t size);
			static void* Calloc(std::size_t n, std::size_t size);
			static void* Realloc(void* ptr, std::size_t size);
			static void Free(void* ptr);

		private:
			MemManager() = default;
			~MemManager();

		private:
			inline static std::list<void*> s_blocks;
	};
}

#endif
