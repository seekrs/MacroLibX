/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_cmd_buffer.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bonsthie <bonsthie@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:25:42 by maldavid          #+#    #+#             */
/*   Updated: 2024/08/08 17:46:00 by bonsthie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_VK_CMD_BUFFER__
#define __MLX_VK_CMD_BUFFER__

#include <mlx_profile.h>
#include <volk.h>
#include <renderer/core/vk_fence.h>
#include <vector>
#include <algorithm>

namespace mlx
{
	class Buffer;
	class Image;

	class CmdBuffer
	{
		public:
			enum class state
			{
				uninit = 0, // buffer not initialized or destroyed
				ready,      // buffer ready to be used after having been submitted
				idle,       // buffer has recorded informations but has not been submitted
				recording,  // buffer is currently recording
				submitted,  // buffer has been submitted
			};

			enum class kind
			{
				single_time = 0,
				long_time
			};

		public:
			void init(kind type, class CmdManager* manager);
			void init(kind type, class CmdPool* pool);
			void destroy() noexcept;

			void beginRecord(VkCommandBufferUsageFlags usage = 0);
			void submit(class Semaphore* semaphores) noexcept;
			void submitIdle(bool shouldWaitForExecution = true) noexcept; // TODO : handle `shouldWaitForExecution` as false by default (needs to modify CmdResources lifetimes to do so)
			void updateSubmitState() noexcept;
			inline void waitForExecution() noexcept { _fence.wait(); updateSubmitState(); _state = state::ready; }
			inline void reset() noexcept { vkResetCommandBuffer(_cmd_buffer, 0); }
			void endRecord();

			void bindVertexBuffer(Buffer& buffer) noexcept;
			void bindIndexBuffer(Buffer& buffer) noexcept;
			void copyBuffer(Buffer& dst, Buffer& src) noexcept;
			void copyBufferToImage(Buffer& buffer, Image& image) noexcept;
			void copyImagetoBuffer(Image& image, Buffer& buffer) noexcept;
			void transitionImageLayout(Image& image, VkImageLayout new_layout) noexcept;

			inline bool isInit() const noexcept { return _state != state::uninit; }
			inline bool isReadyToBeUsed() const noexcept { return _state == state::ready; }
			inline bool isRecording() const noexcept { return _state == state::recording; }
			inline bool hasBeenSubmitted() const noexcept { return _state == state::submitted; }
			inline state getCurrentState() const noexcept { return _state; }

			inline VkCommandBuffer& operator()() noexcept { return _cmd_buffer; }
			inline VkCommandBuffer& get() noexcept { return _cmd_buffer; }
			inline Fence& getFence() noexcept { return _fence; }

		private:
			void preTransferBarrier() noexcept;
			void postTransferBarrier() noexcept;

		private:
			std::vector<class CmdResource*> _cmd_resources;
			Fence _fence;
			VkCommandBuffer _cmd_buffer = VK_NULL_HANDLE;
			class CmdPool* _pool = nullptr;
			state _state = state::uninit;
			kind _type;
	};
}

#endif // __MLX_VK_CMD_BUFFER__
