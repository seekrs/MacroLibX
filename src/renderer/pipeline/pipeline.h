/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/18 21:23:52 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/14 17:04:28 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __PIPELINE__
#define __PIPELINE__

#include <mlx_profile.h>
#include <volk.h>
#include <renderer/command/vk_cmd_buffer.h>

namespace mlx
{
	class GraphicPipeline
	{
		public:
			void init(class Renderer& renderer);
			void destroy() noexcept;

			inline void bindPipeline(CmdBuffer& command_buffer) noexcept { vkCmdBindPipeline(command_buffer.get(), VK_PIPELINE_BIND_POINT_GRAPHICS, _graphics_pipeline); }

			inline const VkPipeline& getPipeline() const noexcept { return _graphics_pipeline; }
			inline const VkPipelineLayout& getPipelineLayout() const noexcept { return _pipeline_layout; }

		private:
			VkPipeline _graphics_pipeline = VK_NULL_HANDLE;
			VkPipelineLayout _pipeline_layout = VK_NULL_HANDLE;
	};
}

#endif
