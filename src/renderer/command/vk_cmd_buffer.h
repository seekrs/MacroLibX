/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_cmd_buffer.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:25:42 by maldavid          #+#    #+#             */
/*   Updated: 2023/12/15 20:19:42 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_VK_CMD_BUFFER__
#define __MLX_VK_CMD_BUFFER__

#include <volk.h>
#include <renderer/core/vk_fence.h>
#include <mlx_profile.h>

namespace mlx
{
	class CmdBuffer
	{
		public:
			void init(class CmdManager* manager);
			void init(class CmdPool* pool);
			void destroy() noexcept;

			void beginRecord(VkCommandBufferUsageFlags usage = 0);
			void submit(class Semaphore& semaphores) noexcept;
			void submitIdle() noexcept;
			inline void waitForExecution() noexcept { _fence.waitAndReset(); }
			inline void reset() noexcept { vkResetCommandBuffer(_cmd_buffer, 0); }
			inline bool isReadyToBeUsed() const noexcept { return _fence.isReady(); }
			void endRecord();

			inline bool isRecording() const noexcept { return _is_recording; }
			inline bool isInit() const noexcept { return _cmd_buffer != VK_NULL_HANDLE; }

			inline VkCommandBuffer& operator()() noexcept { return _cmd_buffer; }
			inline VkCommandBuffer& get() noexcept { return _cmd_buffer; }
			inline Fence& getFence() noexcept { return _fence; }

		private:
			Fence _fence;
			VkCommandBuffer _cmd_buffer = VK_NULL_HANDLE;
			class CmdPool* _pool = nullptr;
			bool _is_recording = false;
	};
}

#endif // __MLX_VK_CMD_BUFFER__
