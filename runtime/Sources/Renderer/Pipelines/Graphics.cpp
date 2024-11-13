#include <PreCompiled.h>
#include <Renderer/Pipelines/Graphics.h>
#include <Renderer/RenderCore.h>
#include <Renderer/Renderer.h>
#include <Renderer/Vertex.h>
#include <Core/EventBus.h>

namespace mlx
{
	void GraphicPipeline::Init(const GraphicPipelineDescriptor& descriptor, [[maybe_unused]] std::string_view debug_name)
	{
		MLX_PROFILE_FUNCTION();
		if(!descriptor.vertex_shader || !descriptor.fragment_shader)
			FatalError("Vulkan: invalid shaders");

		m_attachments = descriptor.color_attachments;
		p_vertex_shader = descriptor.vertex_shader;
		p_fragment_shader = descriptor.fragment_shader;
		p_renderer = descriptor.renderer;

		#ifdef DEBUG
			m_debug_name = debug_name;
		#endif

		std::vector<VkPushConstantRange> push_constants;
		std::vector<VkDescriptorSetLayout> set_layouts;
		push_constants.insert(push_constants.end(), p_vertex_shader->GetPipelineLayout().push_constants.begin(), p_vertex_shader->GetPipelineLayout().push_constants.end());
		push_constants.insert(push_constants.end(), p_fragment_shader->GetPipelineLayout().push_constants.begin(), p_fragment_shader->GetPipelineLayout().push_constants.end());
		set_layouts.insert(set_layouts.end(), p_vertex_shader->GetPipelineLayout().set_layouts.begin(), p_vertex_shader->GetPipelineLayout().set_layouts.end());
		set_layouts.insert(set_layouts.end(), p_fragment_shader->GetPipelineLayout().set_layouts.begin(), p_fragment_shader->GetPipelineLayout().set_layouts.end());
		m_pipeline_layout = kvfCreatePipelineLayout(RenderCore::Get().GetDevice(), set_layouts.data(), set_layouts.size(), push_constants.data(), push_constants.size());

		TransitionAttachments();
		CreateFramebuffers(m_attachments, descriptor.clear_color_attachments);

		KvfGraphicsPipelineBuilder* builder = kvfCreateGPipelineBuilder();
		kvfGPipelineBuilderAddShaderStage(builder, p_vertex_shader->GetShaderStage(), p_vertex_shader->GetShaderModule(), "main");
		kvfGPipelineBuilderAddShaderStage(builder, p_fragment_shader->GetShaderStage(), p_fragment_shader->GetShaderModule(), "main");
		kvfGPipelineBuilderSetInputTopology(builder, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
		kvfGPipelineBuilderSetCullMode(builder, VK_CULL_MODE_NONE, VK_FRONT_FACE_CLOCKWISE);
		kvfGPipelineBuilderDisableDepthTest(builder);
		kvfGPipelineBuilderSetPolygonMode(builder, VK_POLYGON_MODE_FILL, 1.0f);
		kvfGPipelineBuilderSetMultisampling(builder, VK_SAMPLE_COUNT_1_BIT);
		kvfGPipelineBuilderEnableAlphaBlending(builder);

		if(!descriptor.no_vertex_inputs)
		{
			VkVertexInputBindingDescription binding_description = Vertex::GetBindingDescription();
			auto attributes_description = Vertex::GetAttributeDescriptions();
			kvfGPipelineBuilderSetVertexInputs(builder, binding_description, attributes_description.data(), attributes_description.size());
		}

		m_pipeline = kvfCreateGraphicsPipeline(RenderCore::Get().GetDevice(), m_pipeline_layout, builder, m_renderpass);
		DebugLog("Vulkan: graphics pipeline created");
		kvfDestroyGPipelineBuilder(builder);

		#ifdef MLX_HAS_DEBUG_UTILS_FUNCTIONS
			VkDebugUtilsObjectNameInfoEXT name_info{};
			name_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
			name_info.objectType = VK_OBJECT_TYPE_PIPELINE;
			name_info.objectHandle = reinterpret_cast<std::uint64_t>(m_pipeline);
			name_info.pObjectName = m_debug_name.c_str();
			RenderCore::Get().vkSetDebugUtilsObjectNameEXT(RenderCore::Get().GetDevice(), &name_info);

			name_info.objectType = VK_OBJECT_TYPE_RENDER_PASS;
			name_info.objectHandle = reinterpret_cast<std::uint64_t>(m_renderpass);
			RenderCore::Get().vkSetDebugUtilsObjectNameEXT(RenderCore::Get().GetDevice(), &name_info);

			name_info.objectType = VK_OBJECT_TYPE_SHADER_MODULE;
			name_info.objectHandle = reinterpret_cast<std::uint64_t>(p_vertex_shader->GetShaderModule());
			RenderCore::Get().vkSetDebugUtilsObjectNameEXT(RenderCore::Get().GetDevice(), &name_info);

			name_info.objectHandle = reinterpret_cast<std::uint64_t>(p_fragment_shader->GetShaderModule());
			RenderCore::Get().vkSetDebugUtilsObjectNameEXT(RenderCore::Get().GetDevice(), &name_info);

			name_info.objectType = VK_OBJECT_TYPE_FRAMEBUFFER;
			for(VkFramebuffer fb : m_framebuffers)
			{
				name_info.objectHandle = reinterpret_cast<std::uint64_t>(fb);
				RenderCore::Get().vkSetDebugUtilsObjectNameEXT(RenderCore::Get().GetDevice(), &name_info);
			}
		#endif
	}

	bool GraphicPipeline::BindPipeline(VkCommandBuffer command_buffer, std::size_t framebuffer_index, std::array<float, 4> clear) noexcept
	{
		MLX_PROFILE_FUNCTION();
		TransitionAttachments(command_buffer);
		VkFramebuffer fb = m_framebuffers[framebuffer_index];
		VkExtent2D fb_extent = kvfGetFramebufferSize(fb);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = fb_extent.width;
		viewport.height = fb_extent.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		RenderCore::Get().vkCmdSetViewport(command_buffer, 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = fb_extent;
		RenderCore::Get().vkCmdSetScissor(command_buffer, 0, 1, &scissor);

		for(std::size_t i = 0; i < m_clears.size(); i++)
		{
			m_clears[i].color.float32[0] = clear[0];
			m_clears[i].color.float32[1] = clear[1];
			m_clears[i].color.float32[2] = clear[2];
			m_clears[i].color.float32[3] = clear[3];
		}

		kvfBeginRenderPass(m_renderpass, command_buffer, fb, fb_extent, m_clears.data(), m_clears.size());
		RenderCore::Get().vkCmdBindPipeline(command_buffer, GetPipelineBindPoint(), GetPipeline());
		return true;
	}

	void GraphicPipeline::EndPipeline(VkCommandBuffer command_buffer) noexcept
	{
		MLX_PROFILE_FUNCTION();
		RenderCore::Get().vkCmdEndRenderPass(command_buffer);
	}

	void GraphicPipeline::Destroy() noexcept
	{
		MLX_PROFILE_FUNCTION();
		p_vertex_shader.reset();
		p_fragment_shader.reset();
		for(auto& fb : m_framebuffers)
		{
			kvfDestroyFramebuffer(RenderCore::Get().GetDevice(), fb);
			DebugLog("Vulkan: framebuffer destroyed");
		}

		kvfDestroyPipelineLayout(RenderCore::Get().GetDevice(), m_pipeline_layout);
		m_pipeline_layout = VK_NULL_HANDLE;
		DebugLog("Vulkan: graphics pipeline layout destroyed");

		kvfDestroyRenderPass(RenderCore::Get().GetDevice(), m_renderpass);
		m_renderpass = VK_NULL_HANDLE;
		DebugLog("Vulkan: renderpass destroyed");

		kvfDestroyPipeline(RenderCore::Get().GetDevice(), m_pipeline);
		m_pipeline = VK_NULL_HANDLE;
		DebugLog("Vulkan: graphics pipeline destroyed");

		p_renderer = nullptr;
		m_clears.clear();
		m_attachments.clear();
		m_framebuffers.clear();
	}

	void GraphicPipeline::CreateFramebuffers(const std::vector<NonOwningPtr<Texture>>& render_targets, bool clear_attachments)
	{
		MLX_PROFILE_FUNCTION();
		std::vector<VkAttachmentDescription> attachments;
		std::vector<VkImageView> attachment_views;
		if(p_renderer)
		{
			attachments.push_back(kvfBuildSwapchainAttachmentDescription(p_renderer->GetSwapchain().Get(), clear_attachments));
			attachment_views.push_back(p_renderer->GetSwapchain().GetSwapchainImages()[0].GetImageView());
		}

		for(NonOwningPtr<Texture> image : render_targets)
		{
			attachments.push_back(kvfBuildAttachmentDescription((kvfIsDepthFormat(image->GetFormat()) ? KVF_IMAGE_DEPTH : KVF_IMAGE_COLOR), image->GetFormat(), image->GetLayout(), image->GetLayout(), clear_attachments, VK_SAMPLE_COUNT_1_BIT));
			attachment_views.push_back(image->GetImageView());
		}

		m_renderpass = kvfCreateRenderPass(RenderCore::Get().GetDevice(), attachments.data(), attachments.size(), GetPipelineBindPoint());
		m_clears.clear();
		m_clears.resize(attachments.size());
		DebugLog("Vulkan: renderpass created");

		if(p_renderer)
		{
			for(const Image& image : p_renderer->GetSwapchain().GetSwapchainImages())
			{
				attachment_views[0] = image.GetImageView();
				m_framebuffers.push_back(kvfCreateFramebuffer(RenderCore::Get().GetDevice(), m_renderpass, attachment_views.data(), attachment_views.size(), { .width = image.GetWidth(), .height = image.GetHeight() }));
				DebugLog("Vulkan: framebuffer created");
			}
		}
		for(NonOwningPtr<Texture> image : render_targets)
		{
			m_framebuffers.push_back(kvfCreateFramebuffer(RenderCore::Get().GetDevice(), m_renderpass, attachment_views.data(), attachment_views.size(), { .width = image->GetWidth(), .height = image->GetHeight() }));
			DebugLog("Vulkan: framebuffer created");
		}
	}

	void GraphicPipeline::TransitionAttachments(VkCommandBuffer cmd)
	{
		MLX_PROFILE_FUNCTION();
		for(NonOwningPtr<Texture> image : m_attachments)
		{
			if(!image->IsInit())
				continue;
			image->TransitionLayout(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, cmd);
		}
	}
}
