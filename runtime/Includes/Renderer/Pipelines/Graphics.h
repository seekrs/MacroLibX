#ifndef __MLX_GRAPHICS_PIPELINE__
#define __MLX_GRAPHICS_PIPELINE__

#include <Renderer/Image.h>
#include <Utils/NonOwningPtr.h>
#include <Renderer/Pipelines/Shader.h>
#include <Renderer/Pipelines/Pipeline.h>

namespace mlx
{
	struct GraphicPipelineDescriptor
	{
		std::shared_ptr<Shader> vertex_shader;
		std::shared_ptr<Shader> fragment_shader;
		std::vector<NonOwningPtr<Texture>> color_attachments;
		NonOwningPtr<class Renderer> renderer = nullptr;
		bool clear_color_attachments = true;
		bool no_vertex_inputs = false;
	};

	class GraphicPipeline : public Pipeline
	{
		public:
			GraphicPipeline() = default;

			void Init(const GraphicPipelineDescriptor& descriptor, std::string_view debug_name);
			bool BindPipeline(VkCommandBuffer cmd, std::size_t framebuffer_index, std::array<float, 4> clear) noexcept;
			void EndPipeline(VkCommandBuffer cmd) noexcept override;
			void Destroy() noexcept;

			[[nodiscard]] inline VkPipeline GetPipeline() const override { return m_pipeline; }
			[[nodiscard]] inline VkPipelineLayout GetPipelineLayout() const override { return m_pipeline_layout; }
			[[nodiscard]] inline VkPipelineBindPoint GetPipelineBindPoint() const override { return VK_PIPELINE_BIND_POINT_GRAPHICS; }

			~GraphicPipeline() = default;

		private:
			void CreateFramebuffers(const std::vector<NonOwningPtr<Texture>>& render_targets, bool clear_attachments);
			void TransitionAttachments(VkCommandBuffer cmd = VK_NULL_HANDLE);

			// Private override to remove access
			bool BindPipeline(VkCommandBuffer) noexcept override { return false; };

		private:
			std::vector<NonOwningPtr<Texture>> m_attachments;
			std::vector<VkFramebuffer> m_framebuffers;
			std::vector<VkClearValue> m_clears;
			#ifdef DEBUG
				std::string m_debug_name;
			#endif
			std::shared_ptr<Shader> p_vertex_shader;
			std::shared_ptr<Shader> p_fragment_shader;
			VkRenderPass m_renderpass = VK_NULL_HANDLE;
			VkPipeline m_pipeline = VK_NULL_HANDLE;
			VkPipelineLayout m_pipeline_layout = VK_NULL_HANDLE;
			NonOwningPtr<class Renderer> p_renderer;
	};
}

#endif
