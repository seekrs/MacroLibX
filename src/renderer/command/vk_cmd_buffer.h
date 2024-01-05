/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_cmd_buffer.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:25:42 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/05 23:10:01 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_VK_CMD_BUFFER__
#define __MLX_VK_CMD_BUFFER__

#include <renderer/core/cmd_resource.h>
#include <mlx_profile.h>
#include <volk.h>
#include <renderer/core/vk_fence.h>
#include <unordered_set>

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
			void submitIdle() noexcept;
			inline void waitForExecution() noexcept { _fence.waitAndReset(); _state = state::ready; }
			inline void reset() noexcept { vkResetCommandBuffer(_cmd_buffer, 0); }
			void endRecord();

			void bindVertexBuffer(Buffer& buffer) const noexcept;
			void bindIndexBuffer(Buffer& buffer) const noexcept;
			void copyBuffer(Buffer& dst, Buffer& src) const noexcept;
			void copyBufferToImage(Buffer& buffer, Image& image) const noexcept;
			void copyImagetoBuffer(Image& image, Buffer& buffer) const noexcept;
			void transitionImageLayout(Image& image, VkImageLayout new_layout) const noexcept;

			inline bool isInit() const noexcept { return _state != state::uninit; }
			inline bool isReadyToBeUsed() const noexcept { return _state == state::ready; }
			inline bool isRecording() const noexcept { return _state == state::recording; }
			inline bool hasBeenSubmitted() const noexcept { return _state == state::submitted; }
			inline state getCurrentState() const noexcept { return _state; }

			inline VkCommandBuffer& operator()() noexcept { return _cmd_buffer; }
			inline VkCommandBuffer& get() noexcept { return _cmd_buffer; }
			inline Fence& getFence() noexcept { return _fence; }

		private:
			std::unordered_set<CmdResource> _resources;
			Fence _fence;
			VkCommandBuffer _cmd_buffer = VK_NULL_HANDLE;
			class CmdPool* _pool = nullptr;
			state _state = state::uninit;
			kind _type;
	};
}

#endif // __MLX_VK_CMD_BUFFER__
