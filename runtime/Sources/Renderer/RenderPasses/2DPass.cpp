#include <PreCompiled.h>
#include <Renderer/RenderPasses/2DPass.h>
#include <Renderer/Pipelines/Graphics.h>
#include <Renderer/ViewerData.h>
#include <Renderer/Renderer.h>
#include <Graphics/Scene.h>
#include <Maths/Mat4.h>

namespace mlx
{
	struct SpriteData
	{
		Vec4f color;
		Vec4f position;
	};

	void Render2DPass::Init()
	{
		MLX_PROFILE_FUNCTION();
		ShaderLayout vertex_shader_layout(
			{
				{ 0,
					ShaderSetLayout({ 
						{ 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER }
					})
				}
			}, { ShaderPushConstantLayout({ 0, sizeof(SpriteData) }) }
		);
		std::vector<std::uint8_t> vertex_shader_code = {
			#include <Embedded/2DVertex.spv.h>
		};
		p_vertex_shader = std::make_shared<Shader>(vertex_shader_code, ShaderType::Vertex, std::move(vertex_shader_layout));
		ShaderLayout fragment_shader_layout(
			{
				{ 1,
					ShaderSetLayout({ 
						{ 0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER }
					})
				}
			}, {}
		);
		std::vector<std::uint8_t> fragment_shader_code = {
			#include <Embedded/2DFragment.spv.h>
		};
		p_fragment_shader = std::make_shared<Shader>(fragment_shader_code, ShaderType::Fragment, std::move(fragment_shader_layout));

		func::function<void(const EventBase&)> functor = [this](const EventBase& event)
		{
			if(event.What() == Event::ResizeEventCode)
				m_pipeline.Destroy();
			if(event.What() == Event::DescriptorPoolResetEventCode)
			{
				p_texture_set->Reallocate();
				p_viewer_data_set->Reallocate();
				for(std::size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
				{
					p_viewer_data_set->SetUniformBuffer(i, 0, p_viewer_data_buffer->Get(i));
					p_viewer_data_set->Update(i);
				}
			}
		};
		EventBus::RegisterListener({ functor, "__MlxRender2DPass" });

		p_viewer_data_set = std::make_shared<DescriptorSet>(p_vertex_shader->GetShaderLayout().set_layouts[0].second, p_vertex_shader->GetPipelineLayout().set_layouts[0], ShaderType::Vertex);
		p_texture_set = std::make_shared<DescriptorSet>(p_fragment_shader->GetShaderLayout().set_layouts[0].second, p_fragment_shader->GetPipelineLayout().set_layouts[0], ShaderType::Fragment);

		p_viewer_data_buffer = std::make_shared<UniformBuffer>();
		p_viewer_data_buffer->Init(sizeof(ViewerData), "mlx_2d_pass_viewer_data");
		for(std::size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			p_viewer_data_set->SetUniformBuffer(i, 0, p_viewer_data_buffer->Get(i));
			p_viewer_data_set->Update(i);
		}
	}

	void Render2DPass::Pass(Scene& scene, Renderer& renderer, Texture& render_target)
	{
		MLX_PROFILE_FUNCTION();
		if(m_pipeline.GetPipeline() == VK_NULL_HANDLE)
		{
			GraphicPipelineDescriptor pipeline_descriptor;
			pipeline_descriptor.vertex_shader = p_vertex_shader;
			pipeline_descriptor.fragment_shader = p_fragment_shader;
			pipeline_descriptor.color_attachments = { &render_target };
			pipeline_descriptor.depth = &scene.GetDepth();
			pipeline_descriptor.clear_color_attachments = false;
			m_pipeline.Init(pipeline_descriptor);
		}

		std::uint32_t frame_index = renderer.GetCurrentFrameIndex();

		ViewerData viewer_data;
		viewer_data.projection_matrix = Mat4f::Ortho(0.0f, render_target.GetWidth(), render_target.GetHeight(), 0.0f);
		static CPUBuffer buffer(sizeof(ViewerData));
		std::memcpy(buffer.GetData(), &viewer_data, buffer.GetSize());
		p_viewer_data_buffer->SetData(buffer, frame_index);

		VkCommandBuffer cmd = renderer.GetActiveCommandBuffer();
		m_pipeline.BindPipeline(cmd, 0, {});
		for(auto sprite : scene.GetSprites())
		{
			SpriteData sprite_data;
			sprite_data.position = Vec4f{ sprite->GetPosition(), 1.0f };
			sprite_data.color = sprite->GetColor();
			if(!sprite->IsSetInit())
				sprite->UpdateDescriptorSet(*p_texture_set);
			Verify((bool)sprite->GetTexture(), "a sprite has no texture attached");
			sprite->GetTexture()->Update(cmd);
			sprite->Bind(frame_index, cmd);
			std::array<VkDescriptorSet, 2> sets = { p_viewer_data_set->GetSet(frame_index), sprite->GetSet(frame_index) };
			mlx::RenderCore::Get().vkCmdBindDescriptorSets(cmd, m_pipeline.GetPipelineBindPoint(), m_pipeline.GetPipelineLayout(), 0, sets.size(), sets.data(), 0, nullptr);
			mlx::RenderCore::Get().vkCmdPushConstants(cmd, m_pipeline.GetPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(SpriteData), &sprite_data);
			sprite->GetMesh()->Draw(cmd, renderer.GetDrawCallsCounterRef(), renderer.GetPolygonDrawnCounterRef());
		}
		m_pipeline.EndPipeline(cmd);
	}

	void Render2DPass::Destroy()
	{
		MLX_PROFILE_FUNCTION();
		m_pipeline.Destroy();
		p_vertex_shader.reset();
		p_fragment_shader.reset();
		p_viewer_data_set.reset();
		p_viewer_data_buffer->Destroy();
		p_texture_set.reset();
	}
}
