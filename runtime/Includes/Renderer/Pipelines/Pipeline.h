/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pipeline.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/18 21:23:52 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/28 22:15:38 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __PIPELINE__
#define __PIPELINE__

#include <Renderer/Command/CommandBuffer.h>

namespace mlx
{
	class GraphicPipeline
	{
		public:
			void init(class Renderer& renderer);
			void Destroy() noexcept;

			inline void BindPipeline(CommandBuffer& command_buffer) noexcept { vkCmdBindPipeline(command_buffer.Get(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphics_pipeline); }

			inline const VkPipeline& GetPipeline() const noexcept { return m_graphics_pipeline; }
			inline const VkPipelineLayout& GetPipelineLayout() const noexcept { return m_pipeline_layout; }

		private:
			VkPipeline m_graphics_pipeline = VK_NULL_HANDLE;
			VkPipelineLayout m_pipeline_layout = VK_NULL_HANDLE;
	};
}

#endif
