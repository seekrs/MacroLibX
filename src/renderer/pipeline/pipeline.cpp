/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/18 21:27:38 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/14 17:05:21 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipeline.h"
#include <renderer/renderer.h>
#include <renderer/core/render_core.h>
#include <renderer/descriptors/vk_descriptor_set_layout.h>

namespace mlx
{
	/**
			#version 450 core

			layout(location = 0) in vec2 aPos;
			layout(location = 1) in vec4 aColor;
			layout(location = 2) in vec2 aUV;

			layout(set = 0, binding = 0) uniform uProjection {
				mat4 mat;
			} uProj;

			layout(push_constant) uniform uModelPushConstant {
				vec2 vec;
			} uTranslate;

			out gl_PerVertex {
				vec4 gl_Position;
			};

			layout(location = 0) out struct {
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

			layout(location = 0) in struct {
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

	void GraphicPipeline::init(Renderer& renderer)
    {
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = vertex_shader.size() * sizeof(std::uint32_t);
		createInfo.pCode = vertex_shader.data();
		VkShaderModule vshader;
		if(vkCreateShaderModule(Render_Core::get().getDevice().get(), &createInfo, nullptr, &vshader) != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to create a vertex shader module");

		VkPushConstantRange push_constant;
		push_constant.offset = 0;
		push_constant.size = sizeof(glm::vec2);
		push_constant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = fragment_shader.size() * sizeof(std::uint32_t);
		createInfo.pCode = fragment_shader.data();
		VkShaderModule fshader;
		if(vkCreateShaderModule(Render_Core::get().getDevice().get(), &createInfo, nullptr, &fshader) != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to create a fragment shader module");

		VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
		vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertShaderStageInfo.module = vshader;
		vertShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
		fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragShaderStageInfo.module = fshader;
		fragShaderStageInfo.pName = "main";

		std::array<VkPipelineShaderStageCreateInfo, 2> stages = {vertShaderStageInfo, fragShaderStageInfo};

		auto bindingDescription = Vertex::getBindingDescription();
		auto attributeDescriptions = Vertex::getAttributeDescriptions();

		VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo{};
		vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputStateCreateInfo.vertexBindingDescriptionCount = 1;
		vertexInputStateCreateInfo.pVertexBindingDescriptions = &bindingDescription;
		vertexInputStateCreateInfo.vertexAttributeDescriptionCount = static_cast<std::uint32_t>(attributeDescriptions.size());
		vertexInputStateCreateInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		VkDynamicState states[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

		constexpr std::size_t statesCount = sizeof(states) / sizeof(VkDynamicState);
		VkPipelineDynamicStateCreateInfo dynamicStates{};
		dynamicStates.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicStates.dynamicStateCount = statesCount;
		dynamicStates.pDynamicStates = states;

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)renderer.getFrameBuffer(0).getWidth();
		viewport.height = (float)renderer.getFrameBuffer(0).getHeight();
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = { renderer.getFrameBuffer(0).getWidth(), renderer.getFrameBuffer(0).getHeight()};

		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.pViewports = &viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &scissor;

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

		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_TRUE;
		colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

		VkPipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY;
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = 1.0f;
		colorBlending.blendConstants[1] = 1.0f;
		colorBlending.blendConstants[2] = 1.0f;
		colorBlending.blendConstants[3] = 1.0f;

		VkDescriptorSetLayout layouts[] = {
			renderer.getVertDescriptorSetLayout().get(),
			renderer.getFragDescriptorSetLayout().get()
		};

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 2;
		pipelineLayoutInfo.pSetLayouts = layouts;
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &push_constant;

		if(vkCreatePipelineLayout(Render_Core::get().getDevice().get(), &pipelineLayoutInfo, nullptr, &_pipeline_layout) != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to create a graphics pipeline layout");

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = stages.size();
		pipelineInfo.pStages = stages.data();
		pipelineInfo.pVertexInputState = &vertexInputStateCreateInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = &dynamicStates;
		pipelineInfo.layout = _pipeline_layout;
		pipelineInfo.renderPass = renderer.getRenderPass().get();
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

		VkResult res = vkCreateGraphicsPipelines(Render_Core::get().getDevice().get(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &_graphics_pipeline);
		if(res != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to create a graphics pipeline, %s", RCore::verbaliseResultVk(res));
#ifdef DEBUG
		core::error::report(e_kind::message, "Vulkan : created new graphic pipeline");
#endif

		vkDestroyShaderModule(Render_Core::get().getDevice().get(), fshader, nullptr);
		vkDestroyShaderModule(Render_Core::get().getDevice().get(), vshader, nullptr);
	}

	void GraphicPipeline::destroy() noexcept
	{
		vkDestroyPipeline(Render_Core::get().getDevice().get(), _graphics_pipeline, nullptr);
		vkDestroyPipelineLayout(Render_Core::get().getDevice().get(), _pipeline_layout, nullptr);
		_graphics_pipeline = VK_NULL_HANDLE;
		#ifdef DEBUG
			core::error::report(e_kind::message, "Vulkan : destroyed a graphics pipeline");
		#endif
	}
}
