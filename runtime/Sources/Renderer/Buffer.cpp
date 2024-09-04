#include <PreCompiled.h>
#include <Renderer/RenderCore.h>
#include <Renderer/Buffer.h>

namespace mlx
{
	void GPUBuffer::Init(BufferType type, VkDeviceSize size, VkBufferUsageFlags usage, CPUBuffer data)
	{
		VmaAllocationCreateInfo alloc_info{};
		alloc_info.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
		alloc_info.usage = VMA_MEMORY_USAGE_AUTO;

		if(type == BufferType::Constant)
		{
			if(data.Empty())
			{
				Warning("Vulkan : trying to create constant buffer without data (constant buffers cannot be modified after creation)");
				return;
			}
			m_usage = usage | VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		}
		else if(type == BufferType::HighDynamic)
			m_usage = usage;
		else // LowDynamic or Staging
			m_usage = usage | VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

		if(type == BufferType::Staging && data.Empty())
			Warning("Vulkan : trying to create staging buffer without data (wtf?)");

		CreateBuffer(size, m_usage, alloc_info);

		if(!data.Empty())
		{
			if(p_map != nullptr)
				std::memcpy(p_map, data.GetData(), data.GetSize());
		}
		if(type == BufferType::Constant || type == BufferType::LowDynamic)
			PushToGPU();
	}

	void GPUBuffer::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VmaAllocationCreateInfo alloc_info)
	{
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		m_allocation = RenderCore::Get().GetAllocator().CreateBuffer(&bufferInfo, &alloc_info, m_buffer, nullptr);
		if(alloc_info.flags != 0)
			RenderCore::Get().GetAllocator().MapMemory(m_allocation, &p_map);
	}

	bool GPUBuffer::CopyFrom(const GPUBuffer& buffer) noexcept
	{
		if(!(m_usage & VK_BUFFER_USAGE_TRANSFER_DST_BIT))
		{
			Error("Vulkan : buffer cannot be the destination of a copy because it does not have the correct usage flag");
			return false;
		}
		if(!(buffer.m_usage & VK_BUFFER_USAGE_TRANSFER_SRC_BIT))
		{
			Error("Vulkan : buffer cannot be the source of a copy because it does not have the correct usage flag");
			return false;
		}

		VkCommandBuffer cmd = kvfCreateCommandBuffer(RenderCore::Get().GetDevice());
		kvfBeginCommandBuffer(cmd, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
		kvfCopyBufferToBuffer(cmd, m_buffer, buffer.Get(), m_size);
		kvfEndCommandBuffer(cmd);
		VkFence fence = kvfCreateFence(RenderCore::Get().GetDevice());
		kvfSubmitSingleTimeCommandBuffer(RenderCore::Get().GetDevice(), cmd, KVF_GRAPHICS_QUEUE, fence);
		kvfWaitForFence(RenderCore::Get().GetDevice(), fence);	
		kvfDestroyFence(RenderCore::Get().GetDevice(), fence);
		return true;
	}

	void GPUBuffer::PushToGPU() noexcept
	{
		VmaAllocationCreateInfo alloc_info{};
		alloc_info.usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;

		GPUBuffer new_buffer;
		new_buffer.m_usage = (this->m_usage & 0xFFFFFFFC) | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		new_buffer.CreateBuffer(m_size, new_buffer.m_usage, alloc_info);

		if(new_buffer.CopyFrom(*this))
			Swap(new_buffer);
		new_buffer.Destroy();
		DebugLog("Vulkan : pushed buffer to GPU memory");
	}

	void GPUBuffer::Destroy() noexcept
	{
		if(m_buffer == VK_NULL_HANDLE)
			return;
		RenderCore::Get().GetAllocator().UnmapMemory(m_allocation);
		RenderCore::Get().GetAllocator().DestroyBuffer(m_allocation, m_buffer);
		m_buffer = VK_NULL_HANDLE;
	}

	void GPUBuffer::Swap(GPUBuffer& buffer) noexcept
	{
		std::swap(m_buffer, buffer.m_buffer);
		std::swap(m_allocation, buffer.m_allocation);
		std::swap(m_size, buffer.m_size);
		std::swap(m_offset, buffer.m_offset);
		std::swap(p_map, buffer.p_map);
		std::swap(m_usage, buffer.m_usage);
	}

	void VertexBuffer::SetData(CPUBuffer data)
	{
		if(data.GetSize() > m_size)
		{
			Error("Vulkan : trying to store to much data in a vertex buffer (% bytes in % bytes)", data.GetSize(), m_size);
			return;
		}
		if(data.Empty())
		{
			Warning("Vulkan : cannot set empty data in a vertex buffer");
			return;
		}
		GPUBuffer staging;
		staging.Init(BufferType::Staging, data.GetSize(), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, data);
		CopyFrom(staging);
		staging.Destroy();
	}

	void IndexBuffer::SetData(CPUBuffer data)
	{
		if(data.GetSize() > m_size)
		{
			Error("Vulkan : trying to store to much data in an index buffer (% bytes in % bytes)", data.GetSize(), m_size);
			return;
		}
		if(data.Empty())
		{
			Warning("Vulkan : cannot set empty data in an index buffer");
			return;
		}
		GPUBuffer staging;
		staging.Init(BufferType::Staging, data.GetSize(), VK_BUFFER_USAGE_INDEX_BUFFER_BIT, data);
		CopyFrom(staging);
		staging.Destroy();
	}

	void UniformBuffer::Init(std::uint32_t size)
	{
		for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			m_buffers[i].Init(BufferType::HighDynamic, size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, {});
			m_maps[i] = m_buffers[i].GetMap();
			if(m_maps[i] == nullptr)
				FatalError("Vulkan : unable to map a uniform buffer");
		}
	}

	void UniformBuffer::SetData(CPUBuffer data, std::size_t frame_index)
	{
		if(data.GetSize() != m_buffers[frame_index].GetSize())
		{
			Error("Vulkan : invalid data size to update to a uniform buffer, % != %", data.GetSize(), m_buffers[frame_index].GetSize());
			return;
		}
		if(m_maps[frame_index] != nullptr)
			std::memcpy(m_maps[frame_index], data.GetData(), data.GetSize());
	}

	void UniformBuffer::Destroy() noexcept
	{
		for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			m_buffers[i].Destroy();
	}
}
