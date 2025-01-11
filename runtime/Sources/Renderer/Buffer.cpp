#include <PreCompiled.h>
#include <Renderer/RenderCore.h>
#include <Renderer/Buffer.h>

namespace mlx
{
	void GPUBuffer::Init(BufferType type, VkDeviceSize size, VkBufferUsageFlags usage, CPUBuffer data, [[maybe_unused]] std::string_view debug_name)
	{
		MLX_PROFILE_FUNCTION();
		VmaAllocationCreateInfo alloc_info{};
		alloc_info.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT;
		alloc_info.usage = VMA_MEMORY_USAGE_AUTO;

		if(type == BufferType::Constant)
		{
			if(data.Empty())
			{
				Warning("Vulkan: trying to create constant buffer without data (constant buffers cannot be modified after creation)");
				return;
			}
			m_usage = usage | VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		}
		else if(type == BufferType::HighDynamic)
			m_usage = usage;
		else // LowDynamic or Staging
			m_usage = usage | VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

		CreateBuffer(size, m_usage, alloc_info, std::move(debug_name));

		if(!data.Empty())
		{
			if(p_map != nullptr)
				std::memcpy(p_map, data.GetData(), data.GetSize());
		}
		if(type == BufferType::Constant || type == BufferType::LowDynamic)
			PushToGPU();
	}

	void GPUBuffer::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VmaAllocationCreateInfo alloc_info, [[maybe_unused]] std::string_view debug_name)
	{
		MLX_PROFILE_FUNCTION();
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		#ifdef DEBUG
			m_debug_name = debug_name;
			std::string alloc_name{ debug_name };
			if(usage & VK_BUFFER_USAGE_INDEX_BUFFER_BIT)
				alloc_name.append("_index_buffer");
			else if(usage & VK_BUFFER_USAGE_VERTEX_BUFFER_BIT)
				alloc_name.append("_vertex_buffer");
			else if(usage & VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)
				alloc_name.append("_uniform_buffer");
			else
				alloc_name.append("_buffer");
			m_allocation = RenderCore::Get().GetAllocator().CreateBuffer(&bufferInfo, &alloc_info, m_buffer, alloc_name.c_str());
		#else
			m_allocation = RenderCore::Get().GetAllocator().CreateBuffer(&bufferInfo, &alloc_info, m_buffer, nullptr);
		#endif
		if(alloc_info.flags != 0)
			RenderCore::Get().GetAllocator().MapMemory(m_allocation, &p_map);
		m_size = size;
	}

	bool GPUBuffer::CopyFrom(const GPUBuffer& buffer) noexcept
	{
		MLX_PROFILE_FUNCTION();
		if(!(m_usage & VK_BUFFER_USAGE_TRANSFER_DST_BIT))
		{
			Error("Vulkan: buffer cannot be the destination of a copy because it does not have the correct usage flag");
			return false;
		}
		if(!(buffer.m_usage & VK_BUFFER_USAGE_TRANSFER_SRC_BIT))
		{
			Error("Vulkan: buffer cannot be the source of a copy because it does not have the correct usage flag");
			return false;
		}

		VkCommandBuffer cmd = kvfCreateCommandBuffer(RenderCore::Get().GetDevice());
		kvfBeginCommandBuffer(cmd, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
		kvfCopyBufferToBuffer(cmd, m_buffer, buffer.Get(), m_size);
		kvfEndCommandBuffer(cmd);
		VkFence fence = kvfCreateFence(RenderCore::Get().GetDevice());
		kvfSubmitSingleTimeCommandBuffer(RenderCore::Get().GetDevice(), cmd, KVF_GRAPHICS_QUEUE, fence);
		kvfDestroyFence(RenderCore::Get().GetDevice(), fence);
		kvfDestroyCommandBuffer(RenderCore::Get().GetDevice(), cmd);
		return true;
	}

	void GPUBuffer::PushToGPU() noexcept
	{
		MLX_PROFILE_FUNCTION();
		VmaAllocationCreateInfo alloc_info{};
		alloc_info.usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;

		GPUBuffer new_buffer;
		new_buffer.m_usage = (this->m_usage & 0xFFFFFFFC) | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

		#ifdef DEBUG
			std::string new_name = m_debug_name + "_gpu";
			new_buffer.CreateBuffer(m_size, new_buffer.m_usage, alloc_info, new_name);
		#else
			new_buffer.CreateBuffer(m_size, new_buffer.m_usage, alloc_info, {});
		#endif

		if(new_buffer.CopyFrom(*this))
			Swap(new_buffer);
		new_buffer.Destroy();
		DebugLog("Vulkan: pushed buffer to GPU memory");
	}

	void GPUBuffer::Destroy() noexcept
	{
		MLX_PROFILE_FUNCTION();
		if(m_buffer == VK_NULL_HANDLE)
			return;
		RenderCore::Get().GetAllocator().UnmapMemory(m_allocation);
		#ifdef DEBUG
			std::string alloc_name{ m_debug_name };
			if(m_usage & VK_BUFFER_USAGE_INDEX_BUFFER_BIT)
				alloc_name.append("_index_buffer");
			else if(m_usage & VK_BUFFER_USAGE_VERTEX_BUFFER_BIT)
				alloc_name.append("_vertex_buffer");
			else if(m_usage & VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)
				alloc_name.append("_uniform_buffer");
			else
				alloc_name.append("_buffer");
			RenderCore::Get().GetAllocator().DestroyBuffer(m_allocation, m_buffer, alloc_name.c_str());
		#else
			RenderCore::Get().GetAllocator().DestroyBuffer(m_allocation, m_buffer, nullptr);
		#endif
		m_buffer = VK_NULL_HANDLE;
	}

	void GPUBuffer::Swap(GPUBuffer& buffer) noexcept
	{
		MLX_PROFILE_FUNCTION();
		std::swap(m_buffer, buffer.m_buffer);
		std::swap(m_allocation, buffer.m_allocation);
		std::swap(m_size, buffer.m_size);
		std::swap(m_offset, buffer.m_offset);
		std::swap(p_map, buffer.p_map);
		std::swap(m_usage, buffer.m_usage);
	}

	void VertexBuffer::SetData(CPUBuffer data)
	{
		MLX_PROFILE_FUNCTION();
		if(data.GetSize() > m_size)
		{
			Error("Vulkan: trying to store to much data in a vertex buffer (% bytes in % bytes)", data.GetSize(), m_size);
			return;
		}
		if(data.Empty())
		{
			Warning("Vulkan: cannot set empty data in a vertex buffer");
			return;
		}
		GPUBuffer staging;
		#ifdef DEBUG
			staging.Init(BufferType::Staging, data.GetSize(), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, data, m_debug_name);
		#else
			staging.Init(BufferType::Staging, data.GetSize(), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, data, {});
		#endif
		CopyFrom(staging);
		staging.Destroy();
	}

	void IndexBuffer::SetData(CPUBuffer data)
	{
		MLX_PROFILE_FUNCTION();
		if(data.GetSize() > m_size)
		{
			Error("Vulkan: trying to store to much data in an index buffer (% bytes in % bytes)", data.GetSize(), m_size);
			return;
		}
		if(data.Empty())
		{
			Warning("Vulkan: cannot set empty data in an index buffer");
			return;
		}
		GPUBuffer staging;
		#ifdef DEBUG
			staging.Init(BufferType::Staging, data.GetSize(), VK_BUFFER_USAGE_INDEX_BUFFER_BIT, data, m_debug_name);
		#else
			staging.Init(BufferType::Staging, data.GetSize(), VK_BUFFER_USAGE_INDEX_BUFFER_BIT, data, {});
		#endif
		CopyFrom(staging);
		staging.Destroy();
	}

	void UniformBuffer::Init(std::uint32_t size, [[maybe_unused]] std::string_view debug_name)
	{
		MLX_PROFILE_FUNCTION();
		for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			#ifdef DEBUG
				m_buffers[i].Init(BufferType::HighDynamic, size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, {}, std::string{ debug_name } + '_' + std::to_string(i));
			#else
				m_buffers[i].Init(BufferType::HighDynamic, size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, {}, {});
			#endif
			m_maps[i] = m_buffers[i].GetMap();
			if(m_maps[i] == nullptr)
				FatalError("Vulkan: unable to map a uniform buffer");
		}
	}

	void UniformBuffer::SetData(CPUBuffer data, std::size_t frame_index)
	{
		MLX_PROFILE_FUNCTION();
		if(data.GetSize() != m_buffers[frame_index].GetSize())
		{
			Error("Vulkan: invalid data size to update to a uniform buffer, % != %", data.GetSize(), m_buffers[frame_index].GetSize());
			return;
		}
		if(m_maps[frame_index] != nullptr)
			std::memcpy(m_maps[frame_index], data.GetData(), data.GetSize());
	}

	void UniformBuffer::Destroy() noexcept
	{
		MLX_PROFILE_FUNCTION();
		for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			m_buffers[i].Destroy();
	}
}
