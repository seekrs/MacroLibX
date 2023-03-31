/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/18 17:14:45 by maldavid          #+#    #+#             */
/*   Updated: 2023/03/31 17:59:09 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __RENDERER__
#define __RENDERER__

#include <array>
#include <vector>
#include <memory>

#include <renderer/pixel_put.h>
#include <renderer/buffers/vk_ubo.h>
#include <renderer/core/vk_surface.h>
#include <renderer/core/render_core.h>
#include <renderer/core/vk_semaphore.h>
#include <renderer/pipeline/pipeline.h>
#include <renderer/command/vk_cmd_pool.h>
#include <renderer/command/vk_cmd_buffer.h>
#include <renderer/swapchain/vk_swapchain.h>
#include <renderer/swapchain/vk_render_pass.h>
#include <renderer/descriptors/vk_descriptor_set.h>
#include <renderer/descriptors/vk_descriptor_pool.h>
#include <renderer/descriptors/vk_descriptor_set_layout.h>

#include <core/errors.h>

#include <glm/glm.hpp>

namespace mlx
{
	struct Vertex
	{
		glm::vec2 pos;
		glm::vec3 color;
		glm::vec2 uv;

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
			attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
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

            void init();

            bool beginFrame();
            void endFrame();

			void destroy();
            
			inline class MLX_Window* getWindow() { return _window; }
            inline void setWindow(class MLX_Window* window) { _window = window; }

            inline Surface& getSurface() noexcept { return _surface; }
            inline CmdPool& getCmdPool() noexcept { return _cmd_pool; }
			inline UBO* getUniformBuffer() noexcept { return _uniform_buffer.get(); }
            inline SwapChain& getSwapChain() noexcept { return _swapchain; }
            inline Semaphore& getSemaphore() noexcept { return _semaphore; }
            inline RenderPass& getRenderPass() noexcept { return _pass; }
            inline CmdBuffer& getCmdBuffer(int i) noexcept { return _cmd_buffers[i]; }
			inline GraphicPipeline& getPipeline() noexcept { return _pipeline; }
            inline CmdBuffer& getActiveCmdBuffer() noexcept { return _cmd_buffers[_active_image_index]; }
			inline DescriptorSet& getVertDescriptorSet() noexcept { return _vert_set; }
			inline DescriptorSet& getFragDescriptorSet() noexcept { return _frag_set; }
			inline DescriptorSetLayout& getVertDescriptorSetLayout() noexcept { return _vert_layout; }
			inline DescriptorSetLayout& getFragDescriptorSetLayout() noexcept { return _frag_layout; }
			inline PixelPutPipeline& getPixelPutPipeline() noexcept { return _pixel_put_pipeline; }
            inline uint32_t getActiveImageIndex() noexcept { return _active_image_index; }
            inline uint32_t getImageIndex() noexcept { return _image_index; }

            constexpr inline void requireFrameBufferResize(int index) noexcept { _framebufferResized = true; }

			~Renderer() = default;

		private:
			PixelPutPipeline _pixel_put_pipeline;
			GraphicPipeline _pipeline;
            RenderPass _pass;
            Surface _surface;
            CmdPool _cmd_pool;
            SwapChain _swapchain;
            Semaphore _semaphore;

			DescriptorPool _desc_pool;
			
			DescriptorSetLayout _vert_layout;
			DescriptorSetLayout _frag_layout;
			
			DescriptorSet _vert_set;
			DescriptorSet _frag_set;
            
			std::array<CmdBuffer, MAX_FRAMES_IN_FLIGHT> _cmd_buffers;
			std::unique_ptr<UBO> _uniform_buffer = nullptr;

			class MLX_Window* _window = nullptr;

            uint32_t _active_image_index = 0;
            uint32_t _image_index = 0;
            bool _framebufferResized = false;
	};
}

#endif
