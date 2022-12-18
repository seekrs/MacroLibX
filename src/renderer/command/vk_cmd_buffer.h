/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_cmd_buffer.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:25:42 by maldavid          #+#    #+#             */
/*   Updated: 2022/12/18 01:08:34 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_VK_CMD_BUFFER__
#define __MLX_VK_CMD_BUFFER__

#include <volk.h>

namespace mlx
{
	class CmdBuffer
	{
		public:
			void init();
			void destroy() noexcept;

			void beginRecord(VkCommandBufferUsageFlags usage = 0);
			void endRecord();

			inline VkCommandBuffer& operator()() noexcept { return _cmd_buffer; }
			inline VkCommandBuffer& get() noexcept { return _cmd_buffer; }

		private:
			VkCommandBuffer _cmd_buffer = VK_NULL_HANDLE;
			bool _is_recording = false;
	};
}

#endif // __MLX_VK_CMD_BUFFER__
