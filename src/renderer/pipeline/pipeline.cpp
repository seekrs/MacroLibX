/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/18 21:27:38 by maldavid          #+#    #+#             */
/*   Updated: 2023/01/25 16:19:33 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipeline.h"
#include <renderer/renderer.h>
#include <renderer/core/render_core.h>

namespace mlx
{
	const std::vector<uint32_t> vertex_shader = {
		0x07230203,0x00010000,0x0008000a,0x00000029,0x00000000,0x00020011,0x00000001,0x0006000b,
		0x00000001,0x4c534c47,0x6474732e,0x3035342e,0x00000000,0x0003000e,0x00000000,0x00000001,
		0x0009000f,0x00000000,0x00000004,0x6e69616d,0x00000000,0x0000000d,0x00000019,0x00000025,
		0x00000027,0x00030003,0x00000002,0x000001c2,0x00040005,0x00000004,0x6e69616d,0x00000000,
		0x00060005,0x0000000b,0x505f6c67,0x65567265,0x78657472,0x00000000,0x00060006,0x0000000b,
		0x00000000,0x505f6c67,0x7469736f,0x006e6f69,0x00070006,0x0000000b,0x00000001,0x505f6c67,
		0x746e696f,0x657a6953,0x00000000,0x00070006,0x0000000b,0x00000002,0x435f6c67,0x4470696c,
		0x61747369,0x0065636e,0x00070006,0x0000000b,0x00000003,0x435f6c67,0x446c6c75,0x61747369,
		0x0065636e,0x00030005,0x0000000d,0x00000000,0x00040005,0x00000011,0x7274614d,0x00007869,
		0x00050006,0x00000011,0x00000000,0x6a6f7270,0x00000000,0x00040005,0x00000013,0x7274616d,
		0x00007869,0x00050005,0x00000019,0x6f506e69,0x69746973,0x00006e6f,0x00050005,0x00000025,
		0x67617266,0x6f6c6f43,0x00000072,0x00040005,0x00000027,0x6f436e69,0x00726f6c,0x00050048,
		0x0000000b,0x00000000,0x0000000b,0x00000000,0x00050048,0x0000000b,0x00000001,0x0000000b,
		0x00000001,0x00050048,0x0000000b,0x00000002,0x0000000b,0x00000003,0x00050048,0x0000000b,
		0x00000003,0x0000000b,0x00000004,0x00030047,0x0000000b,0x00000002,0x00040048,0x00000011,
		0x00000000,0x00000005,0x00050048,0x00000011,0x00000000,0x00000023,0x00000000,0x00050048,
		0x00000011,0x00000000,0x00000007,0x00000010,0x00030047,0x00000011,0x00000002,0x00040047,
		0x00000013,0x00000022,0x00000000,0x00040047,0x00000013,0x00000021,0x00000000,0x00040047,
		0x00000019,0x0000001e,0x00000000,0x00040047,0x00000025,0x0000001e,0x00000000,0x00040047,
		0x00000027,0x0000001e,0x00000001,0x00020013,0x00000002,0x00030021,0x00000003,0x00000002,
		0x00030016,0x00000006,0x00000020,0x00040017,0x00000007,0x00000006,0x00000004,0x00040015,
		0x00000008,0x00000020,0x00000000,0x0004002b,0x00000008,0x00000009,0x00000001,0x0004001c,
		0x0000000a,0x00000006,0x00000009,0x0006001e,0x0000000b,0x00000007,0x00000006,0x0000000a,
		0x0000000a,0x00040020,0x0000000c,0x00000003,0x0000000b,0x0004003b,0x0000000c,0x0000000d,
		0x00000003,0x00040015,0x0000000e,0x00000020,0x00000001,0x0004002b,0x0000000e,0x0000000f,
		0x00000000,0x00040018,0x00000010,0x00000007,0x00000004,0x0003001e,0x00000011,0x00000010,
		0x00040020,0x00000012,0x00000002,0x00000011,0x0004003b,0x00000012,0x00000013,0x00000002,
		0x00040020,0x00000014,0x00000002,0x00000010,0x00040017,0x00000017,0x00000006,0x00000002,
		0x00040020,0x00000018,0x00000001,0x00000017,0x0004003b,0x00000018,0x00000019,0x00000001,
		0x0004002b,0x00000006,0x0000001b,0x00000000,0x0004002b,0x00000006,0x0000001c,0x3f800000,
		0x00040020,0x00000021,0x00000003,0x00000007,0x00040017,0x00000023,0x00000006,0x00000003,
		0x00040020,0x00000024,0x00000003,0x00000023,0x0004003b,0x00000024,0x00000025,0x00000003,
		0x00040020,0x00000026,0x00000001,0x00000023,0x0004003b,0x00000026,0x00000027,0x00000001,
		0x00050036,0x00000002,0x00000004,0x00000000,0x00000003,0x000200f8,0x00000005,0x00050041,
		0x00000014,0x00000015,0x00000013,0x0000000f,0x0004003d,0x00000010,0x00000016,0x00000015,
		0x0004003d,0x00000017,0x0000001a,0x00000019,0x00050051,0x00000006,0x0000001d,0x0000001a,
		0x00000000,0x00050051,0x00000006,0x0000001e,0x0000001a,0x00000001,0x00070050,0x00000007,
		0x0000001f,0x0000001d,0x0000001e,0x0000001b,0x0000001c,0x00050091,0x00000007,0x00000020,
		0x00000016,0x0000001f,0x00050041,0x00000021,0x00000022,0x0000000d,0x0000000f,0x0003003e,
		0x00000022,0x00000020,0x0004003d,0x00000023,0x00000028,0x00000027,0x0003003e,0x00000025,
		0x00000028,0x000100fd,0x00010038
	};

	const std::vector<uint32_t> fragment_shader = {
		0x07230203,0x00010000,0x0008000a,0x00000013,0x00000000,0x00020011,0x00000001,0x0006000b,
		0x00000001,0x4c534c47,0x6474732e,0x3035342e,0x00000000,0x0003000e,0x00000000,0x00000001,
		0x0007000f,0x00000004,0x00000004,0x6e69616d,0x00000000,0x00000009,0x0000000c,0x00030010,
		0x00000004,0x00000007,0x00030003,0x00000002,0x000001c2,0x00090004,0x415f4c47,0x735f4252,
		0x72617065,0x5f657461,0x64616873,0x6f5f7265,0x63656a62,0x00007374,0x00040005,0x00000004,
		0x6e69616d,0x00000000,0x00050005,0x00000009,0x4374756f,0x726f6c6f,0x00000000,0x00050005,
		0x0000000c,0x67617266,0x6f6c6f43,0x00000072,0x00040047,0x00000009,0x0000001e,0x00000000,
		0x00040047,0x0000000c,0x0000001e,0x00000000,0x00020013,0x00000002,0x00030021,0x00000003,
		0x00000002,0x00030016,0x00000006,0x00000020,0x00040017,0x00000007,0x00000006,0x00000004,
		0x00040020,0x00000008,0x00000003,0x00000007,0x0004003b,0x00000008,0x00000009,0x00000003,
		0x00040017,0x0000000a,0x00000006,0x00000003,0x00040020,0x0000000b,0x00000001,0x0000000a,
		0x0004003b,0x0000000b,0x0000000c,0x00000001,0x0004002b,0x00000006,0x0000000e,0x3f800000,
		0x00050036,0x00000002,0x00000004,0x00000000,0x00000003,0x000200f8,0x00000005,0x0004003d,
		0x0000000a,0x0000000d,0x0000000c,0x00050051,0x00000006,0x0000000f,0x0000000d,0x00000000,
		0x00050051,0x00000006,0x00000010,0x0000000d,0x00000001,0x00050051,0x00000006,0x00000011,
		0x0000000d,0x00000002,0x00070050,0x00000007,0x00000012,0x0000000f,0x00000010,0x00000011,
		0x0000000e,0x0003003e,0x00000009,0x00000012,0x000100fd,0x00010038
	};

	void GraphicPipeline::init(Renderer& renderer)
    {
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = vertex_shader.size() * sizeof(uint32_t);
		createInfo.pCode = vertex_shader.data();
		VkShaderModule vshader;
		if(vkCreateShaderModule(Render_Core::get().getDevice().get(), &createInfo, nullptr, &vshader) != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to create a vertex shader module");

		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = fragment_shader.size() * sizeof(uint32_t);
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

		std::vector<VkPipelineShaderStageCreateInfo> stages = {vertShaderStageInfo, fragShaderStageInfo};

		auto bindingDescription = Vertex::getBindingDescription();
        auto attributeDescriptions = Vertex::getAttributeDescriptions();

		VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo{};
		vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputStateCreateInfo.vertexBindingDescriptionCount = 1;
		vertexInputStateCreateInfo.pVertexBindingDescriptions = &bindingDescription;
		vertexInputStateCreateInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
		vertexInputStateCreateInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

		VkDynamicState states[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

		constexpr size_t statesCount = sizeof(states) / sizeof(VkDynamicState);
		VkPipelineDynamicStateCreateInfo dynamicStates{};
		dynamicStates.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicStates.dynamicStateCount = statesCount;
		dynamicStates.pDynamicStates = states;

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)renderer.getSwapChain()._swapChainExtent.width;
        viewport.height = (float)renderer.getSwapChain()._swapChainExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = renderer.getSwapChain()._swapChainExtent;

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
        colorBlendAttachment.blendEnable = VK_FALSE;

        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f;
        colorBlending.blendConstants[1] = 0.0f;
        colorBlending.blendConstants[2] = 0.0f;
        colorBlending.blendConstants[3] = 0.0f;

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 1;
        pipelineLayoutInfo.pSetLayouts = &renderer.getDescriptorSetLayout().get();

        if(vkCreatePipelineLayout(Render_Core::get().getDevice().get(), &pipelineLayoutInfo, nullptr, &_pipelineLayout) != VK_SUCCESS)
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
        pipelineInfo.layout = _pipelineLayout;
        pipelineInfo.renderPass = renderer.getRenderPass().get();
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

        if(vkCreateGraphicsPipelines(Render_Core::get().getDevice().get(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &_graphicsPipeline) != VK_SUCCESS)
            core::error::report(e_kind::fatal_error, "Vulkan : failed to create a graphics pipeline");

		vkDestroyShaderModule(Render_Core::get().getDevice().get(), fshader, nullptr);
        vkDestroyShaderModule(Render_Core::get().getDevice().get(), vshader, nullptr);
    }

    void GraphicPipeline::destroy() noexcept
    {
        vkDestroyPipeline(Render_Core::get().getDevice().get(), _graphicsPipeline, nullptr);
		vkDestroyPipelineLayout(Render_Core::get().getDevice().get(), _pipelineLayout, nullptr);
    }

}
