/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UniformBuffer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:45:52 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/23 14:25:17 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <PreCompiled.h>

#include <Renderer/Buffers/UniformBuffer.h>
#include <Renderer/Renderer.h>

namespace mlx
{
	void UniformBuffer::create(NonOwningPtr<Renderer> renderer, std::uint32_t size, [[maybe_unused]] const char* name)
	{
		MLX_PROFILE_FUNCTION();
		p_renderer = renderer;

		for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			#ifdef DEBUG
				std::string name_frame = name;
				name_frame.append(std::to_string(i));
				m_buffers[i].create(BufferType::HighDynamic, size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, name_frame.c_str());
			#else
				_buffers[i].Create(BufferType::HighDynamic, size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, nullptr);
			#endif
			m_buffers[i].MapMem(&_maps[i]);
			if(m_maps[i] == nullptr)
				FatalError("Vulkan : unable to map a uniform buffer");
		}
	}

	void UniformBuffer::SetData(std::uint32_t size, const void* data)
	{
		MLX_PROFILE_FUNCTION();
		std::memcpy(m_maps[p_renderer->GetActiveImageIndex()], data, static_cast<std::size_t>(size));
	}

	void UniformBuffer::SetDynamicData(std::uint32_t size, const void* data)
	{
		MLX_PROFILE_FUNCTION();
		std::memcpy(m_maps[p_renderer->GetActiveImageIndex()], data, static_cast<std::size_t>(size));
		m_buffers[p_renderer->GetActiveImageIndex()].Flush();
	}

	unsigned int UniformBuffer::GetSize() noexcept
	{
		return m_buffers[p_renderer->GetActiveImageIndex()].GetSize();
	}

	unsigned int UniformBuffer::GetOffset() noexcept
	{
		return m_buffers[p_renderer->GetActiveImageIndex()].GetOffset();
	}

	VkBuffer& UniformBuffer::operator()() noexcept
	{
		return m_buffers[p_renderer->GetActiveImageIndex()].Get();
	}

	VkBuffer& UniformBuffer::Get() noexcept
	{
		return m_buffers[p_renderer->GetActiveImageIndex()].Get();
	}

	void UniformBuffer::Destroy() noexcept
	{
		for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			m_buffers[i].Destroy();
	}
}
