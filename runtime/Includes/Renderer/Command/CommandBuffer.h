/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandBuffer.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:25:42 by maldavid          #+#    #+#             */
/*   Updated: 2024/07/05 13:37:54 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_VK_CMD_BUFFER__
#define __MLX_VK_CMD_BUFFER__

#include <Renderer/Core/Fence.h>
#include <Renderer/Enums.h>

namespace mlx
{
	class Buffer;
	class Image;

	class CommandBuffer
	{
		public:
			void Init(CommandBufferType type, NonOwningPtr<class CommandManager> manager);
			void Init(CommandBufferType type, NonOwningPtr<class CommandPool> pool);
			void Destroy() noexcept;

			void BeginRecord(VkCommandBufferUsageFlags usage = 0);
			void Submit(NonOwningPtr<class Semaphore> signal, NonOwningPtr<class Semaphore> wait) noexcept;
			void SubmitIdle(bool shouldWaitForExecution = true) noexcept; // TODO : handle `shouldWaitForExecution` as false by default (needs to modify CmdResources lifetimes to do so)
			void UpdateSubmitState() noexcept;
			inline void WaitForExecution() noexcept { m_fence.Wait(); UpdateSubmitState(); m_state = CommandBufferState::Ready; }
			inline void Reset() noexcept { vkResetCommandBuffer(m_cmd_buffer, 0); }
			void EndRecord();

			void BindVertexBuffer(Buffer& buffer) noexcept;
			void BindIndexBuffer(Buffer& buffer) noexcept;
			void CopyBuffer(Buffer& dst, Buffer& src) noexcept;
			void CopyBufferToImage(Buffer& buffer, Image& image) noexcept;
			void CopyImagetoBuffer(Image& image, Buffer& buffer) noexcept;
			void TransitionImageLayout(Image& image, VkImageLayout new_layout) noexcept;

			inline bool IsInit() const noexcept { return m_state != CommandBufferState::Uninit; }
			inline bool IsReadyToBeUsed() const noexcept { return m_state == CommandBufferState::Ready; }
			inline bool IsRecording() const noexcept { return m_state == CommandBufferState::Recording; }
			inline bool HasBeenSubmitted() const noexcept { return m_state == CommandBufferState::Submitted; }
			inline CommandBufferState GetCurrentState() const noexcept { return m_state; }

			inline VkCommandBuffer& operator()() noexcept { return m_cmd_buffer; }
			inline VkCommandBuffer& Get() noexcept { return m_cmd_buffer; }
			inline Fence& GetFence() noexcept { return m_fence; }

		private:
			void PreTransferBarrier() noexcept;
			void PostTransferBarrier() noexcept;

		private:
			std::vector<NonOwningPtr<class CommandResource>> m_cmd_resources;
			Fence m_fence;
			VkCommandBuffer m_cmd_buffer = VK_NULL_HANDLE;
			NonOwningPtr<class CmdPool> m_pool;
			CommandBufferState m_state = CommandBufferState::Uninit;
			CommandBufferType m_type;
	};
}

#endif // __MLX_VK_CMD_BUFFER__
