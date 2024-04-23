/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pipeline.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/18 21:27:38 by maldavid          #+#    #+#             */
/*   Updated: 2024/04/23 22:24:13 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <PreCompiled.h>

#include <Renderer/Pipelines/Pipeline.h>
#include <Renderer/Renderer.h>
#include <Renderer/Core/RenderCore.h>
#include <Renderer/Descriptors/DescriptorSetLayout.h>

namespace mlx
{
	/**
			#version 450 core

			layout(location = 0) in vec2 aPos;
			layout(location = 1) in vec4 aColor;
			layout(location = 2) in vec2 aUV;

			layout(set = 0, binding = 0) uniform uProjection
			{
				mat4 mat;
			} uProj;

			layout(push_constant) uniform uModelPushConstant
			{
				vec2 vec;
			} uTranslate;

			out gl_PerVertex
			{
				vec4 gl_Position;
			};

			layout(location = 0) out struct
			{
				vec4 Color;
				vec2 UV;
			} Out;

			void main()
			{
				Out.Color = aColor;
				Out.UV = aUV;
				vec2 pos = aPos + uTranslate.vec;
				gl_Position = uProj.mat * vec4(pos.x, pos.y, 0.0, 1.0);
			}
	*/
	const std::vector<std::uint32_t> vertex_shader = {	// precompiled vertex shader
		0x07230203,0x00010000,0x0008000b,0x0000003b,0x00000000,0x00020011,0x00000001,0x0006000b,
		0x00000001,0x4c534c47,0x6474732e,0x3035342e,0x00000000,0x0003000e,0x00000000,0x00000001,
		0x000a000f,0x00000000,0x00000004,0x6e69616d,0x00000000,0x0000000b,0x0000000f,0x00000015,
		0x0000001b,0x00000026,0x00030003,0x00000002,0x000001c2,0x00040005,0x00000004,0x6e69616d,
		0x00000000,0x00030005,0x00000009,0x00000000,0x00050006,0x00000009,0x00000000,0x6f6c6f43,
		0x00000072,0x00040006,0x00000009,0x00000001,0x00005655,0x00030005,0x0000000b,0x0074754f,
		0x00040005,0x0000000f,0x6c6f4361,0x0000726f,0x00030005,0x00000015,0x00565561,0x00030005,
		0x0000001a,0x00736f70,0x00040005,0x0000001b,0x736f5061,0x00000000,0x00070005,0x0000001d,
		0x646f4d75,0x75506c65,0x6f436873,0x6174736e,0x0000746e,0x00040006,0x0000001d,0x00000000,
		0x00636576,0x00050005,0x0000001f,0x61725475,0x616c736e,0x00006574,0x00060005,0x00000024,
		0x505f6c67,0x65567265,0x78657472,0x00000000,0x00060006,0x00000024,0x00000000,0x505f6c67,
		0x7469736f,0x006e6f69,0x00030005,0x00000026,0x00000000,0x00050005,0x00000028,0x6f725075,
		0x7463656a,0x006e6f69,0x00040006,0x00000028,0x00000000,0x0074616d,0x00040005,0x0000002a,
		0x6f725075,0x0000006a,0x00040047,0x0000000b,0x0000001e,0x00000000,0x00040047,0x0000000f,
		0x0000001e,0x00000001,0x00040047,0x00000015,0x0000001e,0x00000002,0x00040047,0x0000001b,
		0x0000001e,0x00000000,0x00050048,0x0000001d,0x00000000,0x00000023,0x00000000,0x00030047,
		0x0000001d,0x00000002,0x00050048,0x00000024,0x00000000,0x0000000b,0x00000000,0x00030047,
		0x00000024,0x00000002,0x00040048,0x00000028,0x00000000,0x00000005,0x00050048,0x00000028,
		0x00000000,0x00000023,0x00000000,0x00050048,0x00000028,0x00000000,0x00000007,0x00000010,
		0x00030047,0x00000028,0x00000002,0x00040047,0x0000002a,0x00000022,0x00000000,0x00040047,
		0x0000002a,0x00000021,0x00000000,0x00020013,0x00000002,0x00030021,0x00000003,0x00000002,
		0x00030016,0x00000006,0x00000020,0x00040017,0x00000007,0x00000006,0x00000004,0x00040017,
		0x00000008,0x00000006,0x00000002,0x0004001e,0x00000009,0x00000007,0x00000008,0x00040020,
		0x0000000a,0x00000003,0x00000009,0x0004003b,0x0000000a,0x0000000b,0x00000003,0x00040015,
		0x0000000c,0x00000020,0x00000001,0x0004002b,0x0000000c,0x0000000d,0x00000000,0x00040020,
		0x0000000e,0x00000001,0x00000007,0x0004003b,0x0000000e,0x0000000f,0x00000001,0x00040020,
		0x00000011,0x00000003,0x00000007,0x0004002b,0x0000000c,0x00000013,0x00000001,0x00040020,
		0x00000014,0x00000001,0x00000008,0x0004003b,0x00000014,0x00000015,0x00000001,0x00040020,
		0x00000017,0x00000003,0x00000008,0x00040020,0x00000019,0x00000007,0x00000008,0x0004003b,
		0x00000014,0x0000001b,0x00000001,0x0003001e,0x0000001d,0x00000008,0x00040020,0x0000001e,
		0x00000009,0x0000001d,0x0004003b,0x0000001e,0x0000001f,0x00000009,0x00040020,0x00000020,
		0x00000009,0x00000008,0x0003001e,0x00000024,0x00000007,0x00040020,0x00000025,0x00000003,
		0x00000024,0x0004003b,0x00000025,0x00000026,0x00000003,0x00040018,0x00000027,0x00000007,
		0x00000004,0x0003001e,0x00000028,0x00000027,0x00040020,0x00000029,0x00000002,0x00000028,
		0x0004003b,0x00000029,0x0000002a,0x00000002,0x00040020,0x0000002b,0x00000002,0x00000027,
		0x00040015,0x0000002e,0x00000020,0x00000000,0x0004002b,0x0000002e,0x0000002f,0x00000000,
		0x00040020,0x00000030,0x00000007,0x00000006,0x0004002b,0x0000002e,0x00000033,0x00000001,
		0x0004002b,0x00000006,0x00000036,0x00000000,0x0004002b,0x00000006,0x00000037,0x3f800000,
		0x00050036,0x00000002,0x00000004,0x00000000,0x00000003,0x000200f8,0x00000005,0x0004003b,
		0x00000019,0x0000001a,0x00000007,0x0004003d,0x00000007,0x00000010,0x0000000f,0x00050041,
		0x00000011,0x00000012,0x0000000b,0x0000000d,0x0003003e,0x00000012,0x00000010,0x0004003d,
		0x00000008,0x00000016,0x00000015,0x00050041,0x00000017,0x00000018,0x0000000b,0x00000013,
		0x0003003e,0x00000018,0x00000016,0x0004003d,0x00000008,0x0000001c,0x0000001b,0x00050041,
		0x00000020,0x00000021,0x0000001f,0x0000000d,0x0004003d,0x00000008,0x00000022,0x00000021,
		0x00050081,0x00000008,0x00000023,0x0000001c,0x00000022,0x0003003e,0x0000001a,0x00000023,
		0x00050041,0x0000002b,0x0000002c,0x0000002a,0x0000000d,0x0004003d,0x00000027,0x0000002d,
		0x0000002c,0x00050041,0x00000030,0x00000031,0x0000001a,0x0000002f,0x0004003d,0x00000006,
		0x00000032,0x00000031,0x00050041,0x00000030,0x00000034,0x0000001a,0x00000033,0x0004003d,
		0x00000006,0x00000035,0x00000034,0x00070050,0x00000007,0x00000038,0x00000032,0x00000035,
		0x00000036,0x00000037,0x00050091,0x00000007,0x00000039,0x0000002d,0x00000038,0x00050041,
		0x00000011,0x0000003a,0x00000026,0x0000000d,0x0003003e,0x0000003a,0x00000039,0x000100fd,
		0x00010038
	};

	/**
			#version 450 core

			layout(location = 0) out vec4 fColor;

			layout(set = 1, binding = 0) uniform sampler2D sTexture;

			layout(location = 0) in struct
			{
				vec4 Color;
				vec2 UV;
			} In;

			void main()
			{
				vec4 process_color = In.Color * texture(sTexture, In.UV.st);
				if(process_color.w == 0)
					discard;
				fColor = process_color;
			}
	*/ 
	const std::vector<std::uint32_t> fragment_shader = {	// pre compiled fragment shader
		0x07230203,0x00010000,0x0008000b,0x0000002c,0x00000000,0x00020011,0x00000001,0x0006000b,
		0x00000001,0x4c534c47,0x6474732e,0x3035342e,0x00000000,0x0003000e,0x00000000,0x00000001,
		0x0007000f,0x00000004,0x00000004,0x6e69616d,0x00000000,0x0000000d,0x0000002a,0x00030010,
		0x00000004,0x00000007,0x00030003,0x00000002,0x000001c2,0x00040005,0x00000004,0x6e69616d,
		0x00000000,0x00060005,0x00000009,0x636f7270,0x5f737365,0x6f6c6f63,0x00000072,0x00030005,
		0x0000000b,0x00000000,0x00050006,0x0000000b,0x00000000,0x6f6c6f43,0x00000072,0x00040006,
		0x0000000b,0x00000001,0x00005655,0x00030005,0x0000000d,0x00006e49,0x00050005,0x00000016,
		0x78655473,0x65727574,0x00000000,0x00040005,0x0000002a,0x6c6f4366,0x0000726f,0x00040047,
		0x0000000d,0x0000001e,0x00000000,0x00040047,0x00000016,0x00000022,0x00000001,0x00040047,
		0x00000016,0x00000021,0x00000000,0x00040047,0x0000002a,0x0000001e,0x00000000,0x00020013,
		0x00000002,0x00030021,0x00000003,0x00000002,0x00030016,0x00000006,0x00000020,0x00040017,
		0x00000007,0x00000006,0x00000004,0x00040020,0x00000008,0x00000007,0x00000007,0x00040017,
		0x0000000a,0x00000006,0x00000002,0x0004001e,0x0000000b,0x00000007,0x0000000a,0x00040020,
		0x0000000c,0x00000001,0x0000000b,0x0004003b,0x0000000c,0x0000000d,0x00000001,0x00040015,
		0x0000000e,0x00000020,0x00000001,0x0004002b,0x0000000e,0x0000000f,0x00000000,0x00040020,
		0x00000010,0x00000001,0x00000007,0x00090019,0x00000013,0x00000006,0x00000001,0x00000000,
		0x00000000,0x00000000,0x00000001,0x00000000,0x0003001b,0x00000014,0x00000013,0x00040020,
		0x00000015,0x00000000,0x00000014,0x0004003b,0x00000015,0x00000016,0x00000000,0x0004002b,
		0x0000000e,0x00000018,0x00000001,0x00040020,0x00000019,0x00000001,0x0000000a,0x00040015,
		0x0000001e,0x00000020,0x00000000,0x0004002b,0x0000001e,0x0000001f,0x00000003,0x00040020,
		0x00000020,0x00000007,0x00000006,0x0004002b,0x00000006,0x00000023,0x00000000,0x00020014,
		0x00000024,0x00040020,0x00000029,0x00000003,0x00000007,0x0004003b,0x00000029,0x0000002a,
		0x00000003,0x00050036,0x00000002,0x00000004,0x00000000,0x00000003,0x000200f8,0x00000005,
		0x0004003b,0x00000008,0x00000009,0x00000007,0x00050041,0x00000010,0x00000011,0x0000000d,
		0x0000000f,0x0004003d,0x00000007,0x00000012,0x00000011,0x0004003d,0x00000014,0x00000017,
		0x00000016,0x00050041,0x00000019,0x0000001a,0x0000000d,0x00000018,0x0004003d,0x0000000a,
		0x0000001b,0x0000001a,0x00050057,0x00000007,0x0000001c,0x00000017,0x0000001b,0x00050085,
		0x00000007,0x0000001d,0x00000012,0x0000001c,0x0003003e,0x00000009,0x0000001d,0x00050041,
		0x00000020,0x00000021,0x00000009,0x0000001f,0x0004003d,0x00000006,0x00000022,0x00000021,
		0x000500b4,0x00000024,0x00000025,0x00000022,0x00000023,0x000300f7,0x00000027,0x00000000,
		0x000400fa,0x00000025,0x00000026,0x00000027,0x000200f8,0x00000026,0x000100fc,0x000200f8,
		0x00000027,0x0004003d,0x00000007,0x0000002b,0x00000009,0x0003003e,0x0000002a,0x0000002b,
		0x000100fd,0x00010038
	};

	void GraphicPipeline::Init(Renderer& renderer)
    {
		VkShaderModuleCreateInfo create_info{};
		create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		create_info.codeSize = vertex_shader.size() * sizeof(std::uint32_t);
		create_info.pCode = vertex_shader.data();
		VkShaderModule vshader;
		if(vkCreateShaderModule(RenderCore::Get().GetDevice().Get(), &create_info, nullptr, &vshader) != VK_SUCCESS)
			FatalError("Vulkan : failed to create a vertex shader module");

		VkPushConstantRange push_constant;
		push_constant.offset = 0;
		push_constant.size = sizeof(glm::vec2);
		push_constant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

		create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		create_info.codeSize = fragment_shader.size() * sizeof(std::uint32_t);
		create_info.pCode = fragment_shader.data();
		VkShaderModule fshader;
		if(vkCreateShaderModule(RenderCore::Get().GetDevice().Get(), &create_info, nullptr, &fshader) != VK_SUCCESS)
			FatalError("Vulkan : failed to create a fragment shader module");

		VkPipelineShaderStageCreateInfo vert_shader_stage_info{};
		vert_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vert_shader_stage_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vert_shader_stage_info.module = vshader;
		vert_shader_stage_info.pName = "main";

		VkPipelineShaderStageCreateInfo frag_shader_stage_info{};
		frag_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		frag_shader_stage_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		frag_shader_stage_info.module = fshader;
		frag_shader_stage_info.pName = "main";

		std::array<VkPipelineShaderStageCreateInfo, 2> stages = { vert_shader_stage_info, frag_shader_stage_info };

		auto binding_description = Vertex::GetBindingDescription();
		auto attribute_descriptions = Vertex::GetAttributeDescriptions();

		VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info{};
		vertex_input_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertex_input_state_create_info.vertexBindingDescriptionCount = 1;
		vertex_input_state_create_info.pVertexBindingDescriptions = &binding_description;
		vertex_input_state_create_info.vertexAttributeDescriptionCount = static_cast<std::uint32_t>(attribute_descriptions.size());
		vertex_input_state_create_info.pVertexAttributeDescriptions = attribute_descriptions.data();

		VkPipelineInputAssemblyStateCreateInfo input_assembly{};
		input_assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		input_assembly.primitiveRestartEnable = VK_FALSE;

		VkDynamicState states[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

		constexpr std::size_t states_count = sizeof(states) / sizeof(VkDynamicState);
		VkPipelineDynamicStateCreateInfo dynamic_states{};
		dynamic_states.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamic_states.dynamicStateCount = states_count;
		dynamic_states.pDynamicStates = states;

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)renderer.GetFrameBuffer(0).GetWidth();
		viewport.height = (float)renderer.GetFrameBuffer(0).GetHeight();
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = { renderer.GetFrameBuffer(0).GetWidth(), renderer.GetFrameBuffer(0).GetHeight()};

		VkPipelineViewportStateCreateInfo viewport_state{};
		viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewport_state.viewportCount = 1;
		viewport_state.pViewports = &viewport;
		viewport_state.scissorCount = 1;
		viewport_state.pScissors = &scissor;

		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = VK_CULL_MODE_NONE;
		rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rasterizer.depthBiasEnable = VK_FALSE;

		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		VkPipelineColorBlendAttachmentState color_blend_attachment{};
		color_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		color_blend_attachment.blendEnable = VK_TRUE;
		color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		color_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD;
		color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		color_blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD;

		VkPipelineColorBlendStateCreateInfo color_blending{};
		color_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		color_blending.logicOpEnable = VK_FALSE;
		color_blending.logicOp = VK_LOGIC_OP_COPY;
		color_blending.attachmentCount = 1;
		color_blending.pAttachments = &color_blend_attachment;
		color_blending.blendConstants[0] = 1.0f;
		color_blending.blendConstants[1] = 1.0f;
		color_blending.blendConstants[2] = 1.0f;
		color_blending.blendConstants[3] = 1.0f;

		VkDescriptorSetLayout layouts[] = {
			renderer.GetVertDescriptorSet().GetLayout(),
			renderer.GetFragDescriptorSet().GetLayout()
		};

		VkPipelineLayoutCreateInfo pipeline_layout_info{};
		pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipeline_layout_info.setLayoutCount = 2;
		pipeline_layout_info.pSetLayouts = layouts;
		pipeline_layout_info.pushConstantRangeCount = 1;
		pipeline_layout_info.pPushConstantRanges = &push_constant;

		if(vkCreatePipelineLayout(RenderCore::Get().GetDevice().Get(), &pipeline_layout_info, nullptr, &m_pipeline_layout) != VK_SUCCESS)
			FatalError("Vulkan : failed to create a graphics pipeline layout");

		VkGraphicsPipelineCreateInfo pipeline_info{};
		pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipeline_info.stageCount = stages.size();
		pipeline_info.pStages = stages.data();
		pipeline_info.pVertexInputState = &vertex_input_state_create_info;
		pipeline_info.pInputAssemblyState = &input_assembly;
		pipeline_info.pViewportState = &viewport_state;
		pipeline_info.pRasterizationState = &rasterizer;
		pipeline_info.pMultisampleState = &multisampling;
		pipeline_info.pColorBlendState = &color_blending;
		pipeline_info.pDynamicState = &dynamic_states;
		pipeline_info.layout = m_pipeline_layout;
		pipeline_info.renderPass = renderer.GetRenderPass().Get();
		pipeline_info.subpass = 0;
		pipeline_info.basePipelineHandle = VK_NULL_HANDLE;

		VkResult res = vkCreateGraphicsPipelines(RenderCore::Get().GetDevice().Get(), VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &m_graphics_pipeline);
		if(res != VK_SUCCESS)
			FatalError("Vulkan : failed to create a graphics pipeline, %", VerbaliseVkResult(res));
		DebugLog("Vulkan : created new graphic pipeline");

		vkDestroyShaderModule(RenderCore::Get().GetDevice().Get(), fshader, nullptr);
		vkDestroyShaderModule(RenderCore::Get().GetDevice().Get(), vshader, nullptr);
	}

	void GraphicPipeline::Destroy() noexcept
	{
		vkDestroyPipeline(RenderCore::Get().GetDevice().Get(), m_graphics_pipeline, nullptr);
		vkDestroyPipelineLayout(RenderCore::Get().GetDevice().Get(), m_pipeline_layout, nullptr);
		m_graphics_pipeline = VK_NULL_HANDLE;
		DebugLog("Vulkan : destroyed a graphics pipeline");
	}
}
