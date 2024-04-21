/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/18 17:14:45 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/14 16:34:20 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __RENDERER__
#define __RENDERER__

#include <vector>
#include <memory>

#include <renderer/buffers/vk_ubo.h>
#include <renderer/core/vk_surface.h>
#include <renderer/core/render_core.h>
#include <renderer/core/vk_semaphore.h>
#include <renderer/pipeline/pipeline.h>
#include <renderer/command/cmd_manager.h>
#include <renderer/swapchain/vk_swapchain.h>
#include <renderer/renderpass/vk_render_pass.h>
#include <renderer/renderpass/vk_framebuffer.h>
#include <renderer/descriptors/vk_descriptor_set.h>
#include <renderer/descriptors/vk_descriptor_pool.h>
#include <renderer/descriptors/vk_descriptor_set_layout.h>

#include <core/errors.h>
#include <mlx_profile.h>

#include <glm/glm.hpp>

namespace mlx
{
	struct Vertex
	{
		glm::vec2 pos;
		glm::vec4 color;
		glm::vec2 uv;

		Vertex(glm::vec2 _pos, glm::vec4 _color, glm::vec2 _uv) : pos(std::move(_pos)), color(std::move(_color)), uv(std::move(_uv)) {}

		static VkVertexInputBindingDescription getBindingDescription()
		{
			VkVertexInputBindingDescription bindingDescription{};
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(Vertex);
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			return bindingDescription;
		}

		static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions()
		{
			std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions;

			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[0].offset = offsetof(Vertex, pos);

			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
			attributeDescriptions[1].offset = offsetof(Vertex, color);

			attributeDescriptions[2].binding = 0;
			attributeDescriptions[2].location = 2;
			attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[2].offset = offsetof(Vertex, uv);

			return attributeDescriptions;
		}
	};

	class Renderer
	{
		public:
			Renderer() = default;

			void init(class Texture* render_target);

			bool beginFrame();
			void endFrame();

			void destroy();

			inline class MLX_Window* getWindow() { return _window; }
			inline void setWindow(class MLX_Window* window) { _window = window; }

			inline Surface& getSurface() noexcept { return _surface; }
			inline CmdPool& getCmdPool() noexcept { return _cmd.getCmdPool(); }
			inline UBO* getUniformBuffer() noexcept { return _uniform_buffer.get(); }
			inline SwapChain& getSwapChain() noexcept { return _swapchain; }
			inline Semaphore& getSemaphore(int i) noexcept { return _semaphores[i]; }
			inline RenderPass& getRenderPass() noexcept { return _pass; }
			inline GraphicPipeline& getPipeline() noexcept { return _pipeline; }
			inline CmdBuffer& getCmdBuffer(int i) noexcept { return _cmd.getCmdBuffer(i); }
			inline CmdBuffer& getActiveCmdBuffer() noexcept { return _cmd.getCmdBuffer(_current_frame_index); }
			inline FrameBuffer& getFrameBuffer(int i) noexcept { return _framebuffers[i]; }
			inline DescriptorSet& getVertDescriptorSet() noexcept { return _vert_set; }
			inline DescriptorSet& getFragDescriptorSet() noexcept { return _frag_set; }
			inline DescriptorSetLayout& getVertDescriptorSetLayout() noexcept { return _vert_layout; }
			inline DescriptorSetLayout& getFragDescriptorSetLayout() noexcept { return _frag_layout; }
			inline std::uint32_t getActiveImageIndex() noexcept { return _current_frame_index; }
			inline std::uint32_t getImageIndex() noexcept { return _image_index; }

			constexpr inline void requireFrameBufferResize() noexcept { _framebuffer_resized = true; }

			~Renderer() = default;

		private:
			void recreateRenderData();

		private:
			GraphicPipeline _pipeline;
			CmdManager _cmd;
			RenderPass _pass;
			Surface _surface;
			SwapChain _swapchain;
			std::array<Semaphore, MAX_FRAMES_IN_FLIGHT> _semaphores;
			std::vector<FrameBuffer> _framebuffers;

			DescriptorSetLayout _vert_layout;
			DescriptorSetLayout _frag_layout;

			DescriptorSet _vert_set;
			DescriptorSet _frag_set;

			std::unique_ptr<UBO> _uniform_buffer;

			class MLX_Window* _window = nullptr;
			class Texture* _render_target = nullptr;

			std::uint32_t _current_frame_index = 0;
			std::uint32_t _image_index = 0;
			bool _framebuffer_resized = false;
	};
}

#endif
