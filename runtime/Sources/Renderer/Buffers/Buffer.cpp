/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Buffer.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 18:55:57 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/23 14:20:13 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <PreCompiled.h>

#include <Renderer/Buffers/Buffer.h>
#include <Renderer/Command/CommandPool.h>
#include <Renderer/Command/CommandBuffer.h>
#include <Renderer/Core/RenderCore.h>

namespace mlx
{
	void Buffer::Create(BufferType type, VkDeviceSize size, VkBufferUsageFlags usage, const char* name, const void* data)
	{
		MLX_PROFILE_FUNCTION();
		m_usage = usage;
		if(type == BufferType::Constant || type == BufferType::LowDynamic)
		{
			if(data == nullptr && type == BufferType::Constant)
			{
				Warning("Vulkan : trying to create constant buffer without data (constant buffers cannot be modified after creation)");
				return;
			}
			m_usage |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		}

		VmaAllocationCreateInfo alloc_info{};
		alloc_info.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
		alloc_info.usage = VMA_MEMORY_USAGE_AUTO;

		CreateBuffer(m_usage, alloc_info, size, name);

		if(data != nullptr)
		{
			void* mapped = nullptr;
			MapMem(&mapped);
				std::memcpy(mapped, data, size);
			UnmapMem();
			if(type == BufferType::constant || type == BufferType::LowDynamic)
				PushToGPU();
		}
	}

	void Buffer::Destroy() noexcept
	{
		MLX_PROFILE_FUNCTION();
		if(m_is_mapped)
			UnmapMem();
		if(m_buffer != VK_NULL_HANDLE)
			RenderCore::Get().GetAllocator().DestroyBuffer(m_allocation, m_buffer);
		m_buffer = VK_NULL_HANDLE;
	}

	void Buffer::CreateBuffer(VkBufferUsageFlags usage, VmaAllocationCreateInfo info, VkDeviceSize size, [[maybe_unused]] const char* name)
	{
		MLX_PROFILE_FUNCTION();
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		#ifdef DEBUG
			m_name = name;
			std::string alloc_name = m_name;
			if(usage & VK_BUFFER_USAGE_INDEX_BUFFER_BIT)
				alloc_name.append("_index_buffer");
			else if(usage & VK_BUFFER_USAGE_VERTEX_BUFFER_BIT)
				alloc_name.append("_vertex_buffer");
			else if(!(usage & VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT))
				alloc_name.append("_buffer");
			m_allocation = RenderCore::Get().GetAllocator().CreateBuffer(&bufferInfo, &info, m_buffer, alloc_name.c_str());
		#else
			m_allocation = RenderCore::Get().GetAllocator().CreateBuffer(&bufferInfo, &info, m_buffer, nullptr);
		#endif
		m_size = size;
	}

	bool Buffer::CopyFromBuffer(const Buffer& buffer) noexcept
	{
		MLX_PROFILE_FUNCTION();
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

		CmdBuffer& cmd = RenderCore::Get().GetSingleTimeCmdBuffer();
		cmd.BeginRecord();

		cmd.CopyBuffer(*this, const_cast<Buffer&>(buffer));

		cmd.EndRecord();
		cmd.SubmitIdle();

		return true;
	}

	void Buffer::PushToGPU() noexcept
	{
		MLX_PROFILE_FUNCTION();
		VmaAllocationCreateInfo alloc_info{};
		alloc_info.usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;

		Buffer new_buffer;
		new_buffer.m_usage = (m_usage & 0xFFFFFFFC) | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		#ifdef DEBUG
			std::string new_name = m_name + "_GPU";
			new_buffer.CreateBuffer(new_buffer.m_usage, alloc_info, m_size, new_name.c_str());
		#else
			new_buffer.CreateBuffer(new_buffer.m_usage, alloc_info, m_size, nullptr);
		#endif

		if(new_buffer.CopyFromBuffer(*this)) // if the copy succeded we swap the buffers, otherwise the new one is deleted
			this->Swap(new_buffer);
		new_buffer.Destroy(); // destroying the old buffer as they have been swapped
	}

	void Buffer::Swap(Buffer& buffer) noexcept
	{
		std::swap(m_buffer, buffer.m_buffer);
		std::swap(m_allocation, buffer.m_allocation);
		std::swap(m_size, buffer.m_size);
		std::swap(m_offset, buffer.m_offset);
		#ifdef DEBUG
			std::swap(m_name, buffer.m_name);
		#endif
		std::swap(m_usage, buffer.m_usage);
		std::swap(m_is_mapped, buffer.m_is_mapped);
	}

	void Buffer::Flush(VkDeviceSize size, VkDeviceSize offset)
	{
		RenderCore::Get().GetAllocator().Flush(m_allocation, size, offset);
	}
}
