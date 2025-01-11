#ifndef __MLX_CPU_BUFFER__
#define __MLX_CPU_BUFFER__

namespace mlx
{
	class CPUBuffer
	{
		public:
			CPUBuffer() {}
			CPUBuffer(std::size_t size) try : m_data(new std::uint8_t[size]), m_size(size)
			{}
			catch(...)
			{
				FatalError("memory allocation for a CPU buffer failed");
			}

			[[nodiscard]] inline CPUBuffer Duplicate() const
			{
				CPUBuffer buffer(m_size);
				std::memcpy(buffer.GetData(), m_data.get(), m_size);
				return buffer;
			}

			inline bool Empty() const { return m_size == 0; }

			[[nodiscard]] inline std::size_t GetSize() const noexcept { return m_size; }

			template<typename T>
			[[nodiscard]] inline T* GetDataAs() const { return reinterpret_cast<T*>(m_data.get()); }
			[[nodiscard]] inline std::uint8_t* GetData() const { return m_data.get(); }
			inline operator bool() const { return (bool)m_data; }

			~CPUBuffer() = default;

		private:
			std::shared_ptr<std::uint8_t[]> m_data;
			std::size_t m_size = 0;
	};
}

#endif
