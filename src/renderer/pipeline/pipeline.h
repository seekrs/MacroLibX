/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/18 21:23:52 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/03 15:28:13 by maldavid         ###   ########.fr       */
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

			inline void bindPipeline(CmdBuffer& commandBuffer) noexcept { vkCmdBindPipeline(commandBuffer.get(), VK_PIPELINE_BIND_POINT_GRAPHICS, _graphicsPipeline); }

			inline const VkPipeline& getPipeline() const noexcept { return _graphicsPipeline; }
			inline const VkPipelineLayout& getPipelineLayout() const noexcept { return _pipelineLayout; }

		private:
			VkPipeline _graphicsPipeline = VK_NULL_HANDLE;
			VkPipelineCache _cache = VK_NULL_HANDLE;
			VkPipelineLayout _pipelineLayout = VK_NULL_HANDLE;
	};
}

#endif
