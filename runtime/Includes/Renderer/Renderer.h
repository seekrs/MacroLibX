/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Renderer.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/18 17:14:45 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/23 22:25:13 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __RENDERER__
#define __RENDERER__

#include <Renderer/Vertex.h>
#include <Renderer/Buffers/UniformBuffer.h>
#include <Renderer/Core/Surface.h>
#include <Renderer/Core/RenderCore.h>
#include <Renderer/Core/Semaphore.h>
#include <Renderer/Pipeline/Pipeline.h>
#include <Renderer/Command/CommandManager.h>
#include <Renderer/Swapchain/Swapchain.h>
#include <Renderer/Renderpass/RenderPass.h>
#include <Renderer/Renderpass/FrameBuffer.h>
#include <Renderer/Descriptors/DescriptorSet.h>
#include <Renderer/Descriptors/DescriptorPool.h>
#include <Renderer/Descriptors/DescriptorSetLayout.h>

namespace mlx
{
	class Renderer
	{
		public:
			Renderer() = default;

			void Init(NonOwningPtr<class Texture> render_target);

			bool BeginFrame();
			void EndFrame();

			void Destroy();

			inline NonOwningPtr<class Window> GetWindow() { return m_window; }
			inline void SetWindow(NonOwningPtr<class Window> window) { m_window = window; }

			inline Surface& GetSurface() noexcept { return m_surface; }
			inline CmdPool& GetCmdPool() noexcept { return m_cmd.GetCmdPool(); }
			inline NonOwningPtr<UniformBuffer> GetUniformBuffer() noexcept { return m_uniform_buffer.get(); }
			inline SwapChain& GetSwapChain() noexcept { return m_swapchain; }
			inline Semaphore& GetSemaphore(int i) noexcept { return m_semaphores[i]; }
			inline RenderPass& GetRenderPass() noexcept { return m_pass; }
			inline GraphicPipeline& GetPipeline() noexcept { return m_pipeline; }
			inline CmdBuffer& GetCmdBuffer(int i) noexcept { return m_cmd.GetCmdBuffer(i); }
			inline CmdBuffer& GetActiveCmdBuffer() noexcept { return m_cmd.GetCmdBuffer(m_current_frame_index); }
			inline FrameBuffer& GetFrameBuffer(int i) noexcept { return m_framebuffers[i]; }
			inline DescriptorSet& GetVertDescriptorSet() noexcept { return m_vert_set; }
			inline DescriptorSet& GetFragDescriptorSet() noexcept { return m_frag_set; }
			inline std::uint32_t GetActiveImageIndex() noexcept { return m_current_frame_index; }
			inline std::uint32_t GetImageIndex() noexcept { return m_image_index; }

			constexpr inline void RequireFrameBufferResize() noexcept { m_framebuffer_resized = true; }

			~Renderer() = default;

		private:
			void RecreateRenderData();

		private:
			GraphicPipeline m_pipeline;
			CmdManager m_cmd;
			RenderPass m_pass;
			Surface m_surface;
			SwapChain m_swapchain;
			std::array<Semaphore, MAX_FRAMES_IN_FLIGHT> m_semaphores;
			std::vector<FrameBuffer> m_framebuffers;

			DescriptorSet m_vert_set;
			DescriptorSet m_frag_set;

			std::unique_ptr<UniformBuffer> m_uniform_buffer;

			NonOwningPtr<class Window> m_window;
			NonOwningPtr<class Texture> m_render_target;

			std::uint32_t m_current_frame_index = 0;
			std::uint32_t m_image_index = 0;
			bool m_framebuffer_resized = false;
	};
}

#endif
