#include <PreCompiled.h>

#include <Core/Memory.h>

namespace mlx
{
	MemManager* MemManager::s_instance = nullptr;

	MemManager::MemManager()
	{
		s_instance = this;
	}

	void* MemManager::Malloc(std::size_t size)
	{
		void* ptr = std::malloc(size);
		if(ptr != nullptr)
			s_blocks.emplace_back(ptr, size, false);
		return ptr;
	}

	void* MemManager::AlignedMalloc(std::size_t alignment, std::size_t size)
	{
		#ifdef MLX_COMPILER_MSVC
			void* ptr = _aligned_malloc(size, alignment);
		#else
			if(alignment < sizeof(void*))
				alignment = sizeof(void*);
			if(size % alignment != 0)
				size += alignment - (size % alignment);

			void* ptr = std::aligned_alloc(alignment, size);
		#endif
		if(ptr != nullptr)
			s_blocks.emplace_back(ptr, size, true);
		return ptr;
	}

	void* MemManager::Calloc(std::size_t n, std::size_t size)
	{
		void* ptr = std::calloc(n, size);
		if(ptr != nullptr)
			s_blocks.emplace_back(ptr, n * size, false);
		return ptr;
	}

	void* MemManager::AlignedCalloc(std::size_t alignment, std::size_t n, std::size_t size)
	{
		void* ptr = AlignedMalloc(alignment, n * size);
		std::memset(ptr, 0, n * size);
		return ptr;
	}

	void* MemManager::Realloc(void* ptr, std::size_t size)
	{
		auto it = std::find_if(s_blocks.begin(), s_blocks.end(), [=](const Descriptor& rhs){ return ptr == rhs.ptr; });
		if(it != s_blocks.end())
			s_blocks.erase(it);
		void* ptr2 = std::realloc(ptr, size);
		if(ptr2 != nullptr)
			s_blocks.emplace_back(ptr2, size, false);
		return ptr2;
	}

	void* MemManager::AlignedRealloc(void* ptr, std::size_t alignment, std::size_t size)
	{
		auto it = std::find_if(s_blocks.begin(), s_blocks.end(), [=](const Descriptor& rhs){ return ptr == rhs.ptr; });

		#ifdef MLX_COMPILER_MSVC
			void* ptr2 = _aligned_realloc(ptr, size, alignment);
			if(it != s_blocks.end())
				s_blocks.erase(it);
		#else
			void* ptr2 = AlignedMalloc(alignment, size);
			if(it != s_blocks.end())
			{
				std::memcpy(ptr2, ptr, it->size);
				Free(ptr);
			}
		#endif
		if(ptr2 != nullptr)
			s_blocks.emplace_back(ptr2, size, true);
		return ptr2;
	}

	void MemManager::Free(void* ptr)
	{
		if(ptr == nullptr)
			return;
		auto it = std::find_if(s_blocks.begin(), s_blocks.end(), [=](const Descriptor& rhs){ return ptr == rhs.ptr; });
		if(it == s_blocks.end())
			return;
		#ifdef MLX_COMPILER_MSVC
			if(it->aligned)
				_aligned_free(it->ptr);
			else
				std::free(it->ptr);
		#else
			std::free(it->ptr);
		#endif
		s_blocks.erase(it);
	}

	MemManager::~MemManager()
	{
		for(const Descriptor& desc : s_blocks)
		{
			#ifdef MLX_COMPILER_MSVC
				if(desc.aligned)
					_aligned_free(desc.ptr);
				else
					std::free(desc.ptr);
			#else
				std::free(desc.ptr);
			#endif
		}
		DebugLog("Memory Manager: finished garbage collection");
		s_instance = nullptr;
	}
}
