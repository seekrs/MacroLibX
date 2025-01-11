#include <PreCompiled.h>
#include <Renderer/RenderPasses/2DPass.h>
#include <Renderer/Pipelines/Graphics.h>
#include <Renderer/ViewerData.h>
#include <Renderer/Renderer.h>
#include <Graphics/Scene.h>
#include <Maths/Mat4.h>

namespace mlx
{
	struct DrawableData
	{
		Mat4f model_matrix;
		Vec4f color;
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
			}, { ShaderPushConstantLayout({ 0, sizeof(DrawableData) }) }
		);
		std::vector<std::uint8_t> vertex_shader_code = {
			#include <Embedded/Shader2DVertex.spv.h>
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
			#include <Embedded/Shader2DFragment.spv.h>
		};
		p_fragment_shader = std::make_shared<Shader>(fragment_shader_code, ShaderType::Fragment, std::move(fragment_shader_layout));

		std::function<void(const EventBase&)> functor = [this](const EventBase& event)
		{
			if(event.What() == Event::ResizeEventCode)
				m_pipeline.Destroy();
		};
		EventBus::RegisterListener({ functor, "mlx_2d_render_pass_" + std::to_string(reinterpret_cast<std::uintptr_t>(this)) });

		p_viewer_data_set = RenderCore::Get().GetDescriptorPoolManager().GetAvailablePool().RequestDescriptorSet(p_vertex_shader->GetShaderLayout().set_layouts[0].second, ShaderType::Vertex);
		p_texture_set = RenderCore::Get().GetDescriptorPoolManager().GetAvailablePool().RequestDescriptorSet(p_fragment_shader->GetShaderLayout().set_layouts[0].second, ShaderType::Fragment);

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
			pipeline_descriptor.clear_color_attachments = false;
			#ifdef DEBUG
				if(renderer.GetWindow())
					m_pipeline.Init(pipeline_descriptor, "mlx_2D_pass_" + renderer.GetWindow()->GetName());
				else
					m_pipeline.Init(pipeline_descriptor, "mlx_2D_pass");
			#else
				m_pipeline.Init(pipeline_descriptor, {});
			#endif
		}

		std::uint32_t frame_index = renderer.GetCurrentFrameIndex();

		ViewerData viewer_data;
		viewer_data.projection_matrix = Mat4f::Ortho(0.0f, render_target.GetWidth(), render_target.GetHeight(), 0.0f, -1.0f, 1.0f);
		static CPUBuffer buffer(sizeof(ViewerData));
		std::memcpy(buffer.GetData(), &viewer_data, buffer.GetSize());
		p_viewer_data_buffer->SetData(buffer, frame_index);

		VkCommandBuffer cmd = renderer.GetActiveCommandBuffer();

		const auto& drawables = scene.GetDrawables();

		for(auto& drawable : drawables)
		{
			// Check every textures and update modified ones to GPU before starting the render pass
			if(!drawable->IsSetInit())
				drawable->UpdateDescriptorSet(p_texture_set);
			drawable->Update(cmd);
		}

		m_pipeline.BindPipeline(cmd, 0, {});
		for(auto& drawable : drawables)
		{
			DrawableData drawable_data;
			drawable_data.color = drawable->GetColor();

			Mat4f rotation_matrix = Mat4f::Identity();
			rotation_matrix.ApplyTranslation(Vec3f{ -drawable->GetCenter(), 0.0f });
			rotation_matrix.ApplyRotation(drawable->GetRotation());
			rotation_matrix.ApplyTranslation(Vec3f{ drawable->GetCenter(), 0.0f });

			Mat4f translation_matrix = Mat4f::Identity().ApplyTranslation(Vec3f{ drawable->GetPosition(), 0.0f });
			Mat4f scale_matrix = Mat4f::Identity().ApplyScale(Vec3f{ drawable->GetScale(), 1.0f });

			drawable_data.model_matrix = Mat4f::Identity();
			drawable_data.model_matrix.ConcatenateTransform(rotation_matrix);
			drawable_data.model_matrix.ConcatenateTransform(scale_matrix);
			drawable_data.model_matrix.ConcatenateTransform(translation_matrix);

			drawable->Bind(frame_index, cmd);

			std::array<VkDescriptorSet, 2> sets = { p_viewer_data_set->GetSet(frame_index), drawable->GetSet(frame_index) };

			RenderCore::Get().vkCmdPushConstants(cmd, m_pipeline.GetPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(DrawableData), &drawable_data);
			RenderCore::Get().vkCmdBindDescriptorSets(cmd, m_pipeline.GetPipelineBindPoint(), m_pipeline.GetPipelineLayout(), 0, sets.size(), sets.data(), 0, nullptr);

			drawable->GetMesh()->Draw(cmd, renderer.GetDrawCallsCounterRef(), renderer.GetPolygonDrawnCounterRef());
		}
		m_pipeline.EndPipeline(cmd);
	}

	void Render2DPass::Destroy()
	{
		MLX_PROFILE_FUNCTION();
		m_pipeline.Destroy();
		p_vertex_shader.reset();
		p_fragment_shader.reset();
		p_viewer_data_set->ReturnDescriptorSetToPool();
		p_viewer_data_set.reset();
		p_viewer_data_buffer->Destroy();
		p_texture_set->ReturnDescriptorSetToPool();
		p_texture_set.reset();
	}
}
