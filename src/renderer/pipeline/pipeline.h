/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/18 21:23:52 by maldavid          #+#    #+#             */
/*   Updated: 2022/12/19 00:27:29 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __PIPELINE__
#define __PIPELINE__

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
