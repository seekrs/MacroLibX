#include <PreCompiled.h>
#include <Renderer/RenderPasses/FinalPass.h>
#include <Renderer/Pipelines/Graphics.h>
#include <Renderer/Renderer.h>
#include <Graphics/Scene.h>
#include <Core/EventBus.h>

namespace mlx
{
	void FinalPass::Init()
	{
		MLX_PROFILE_FUNCTION();
		ShaderLayout vertex_shader_layout(
			{}, {}
		);
		std::vector<std::uint8_t> vertex_shader_code = {
			#include <Embedded/ShaderScreenVertex.spv.h>
		};
		p_vertex_shader = std::make_shared<Shader>(vertex_shader_code, ShaderType::Vertex, std::move(vertex_shader_layout));
		ShaderLayout fragment_shader_layout(
			{
				{ 0,
					ShaderSetLayout({
						{ 0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER }
					})
				}
			}, {}
		);
		std::vector<std::uint8_t> fragment_shader_code = {
			#include <Embedded/ShaderScreenFragment.spv.h>
		};
		p_fragment_shader = std::make_shared<Shader>(fragment_shader_code, ShaderType::Fragment, std::move(fragment_shader_layout));

		std::function<void(const EventBase&)> functor = [this](const EventBase& event)
		{
			if(event.What() == Event::ResizeEventCode)
				m_pipeline.Destroy();
		};
		EventBus::RegisterListener({ functor, "mlx_final_pass_" + std::to_string(reinterpret_cast<std::uintptr_t>(this)) });

		p_set = RenderCore::Get().GetDescriptorPoolManager().GetAvailablePool().RequestDescriptorSet(p_fragment_shader->GetShaderLayout().set_layouts[0].second, ShaderType::Fragment);
	}

	void FinalPass::Pass([[maybe_unused]] Scene& scene, Renderer& renderer, Texture& render_target, NonOwningPtr<class Texture> final_target)
	{
		MLX_PROFILE_FUNCTION();
		if(m_pipeline.GetPipeline() == VK_NULL_HANDLE)
		{
			GraphicPipelineDescriptor pipeline_descriptor;
			pipeline_descriptor.vertex_shader = p_vertex_shader;
			pipeline_descriptor.fragment_shader = p_fragment_shader;
			if(final_target)
				pipeline_descriptor.color_attachments = { final_target };
			else
				pipeline_descriptor.renderer = &renderer;
			pipeline_descriptor.no_vertex_inputs = true;
			#ifdef DEBUG
				if(final_target)
					m_pipeline.Init(pipeline_descriptor, "mlx_final_pass");
				else
					m_pipeline.Init(pipeline_descriptor, "mlx_final_pass_" + renderer.GetWindow()->GetName());
			#else
				m_pipeline.Init(pipeline_descriptor, {});
			#endif
		}

		VkCommandBuffer cmd = renderer.GetActiveCommandBuffer();

		p_set->SetImage(renderer.GetCurrentFrameIndex(), 0, render_target);
		p_set->Update(renderer.GetCurrentFrameIndex(), cmd);

		m_pipeline.BindPipeline(cmd, renderer.GetSwapchain().GetImageIndex(), { 0.0f, 0.0f, 0.0f, 1.0f });
			VkDescriptorSet set = p_set->GetSet(renderer.GetCurrentFrameIndex());
			RenderCore::Get().vkCmdBindDescriptorSets(cmd, m_pipeline.GetPipelineBindPoint(), m_pipeline.GetPipelineLayout(), 0, 1, &set, 0, nullptr);
			RenderCore::Get().vkCmdDraw(cmd, 3, 1, 0, 0);
			renderer.GetDrawCallsCounterRef()++;
			renderer.GetPolygonDrawnCounterRef()++;
		m_pipeline.EndPipeline(cmd);
	}

	void FinalPass::Destroy()
	{
		MLX_PROFILE_FUNCTION();
		m_pipeline.Destroy();
		p_vertex_shader.reset();
		p_fragment_shader.reset();
		p_set->ReturnDescriptorSetToPool();
		p_set.reset();
	}
}
