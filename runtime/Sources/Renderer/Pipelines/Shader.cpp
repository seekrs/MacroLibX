#include <PreCompiled.h>
#include <Renderer/Pipelines/Shader.h>
#include <Renderer/RenderCore.h>

namespace Scop
{
	Shader::Shader(const std::vector<std::uint8_t>& bytecode, ShaderType type, ShaderLayout layout) : m_bytecode(bytecode), m_layout(std::move(layout))
	{
		switch(type)
		{
			case ShaderType::Vertex : m_stage = VK_SHADER_STAGE_VERTEX_BIT; break;
			case ShaderType::Fragment : m_stage = VK_SHADER_STAGE_FRAGMENT_BIT; break;
			case ShaderType::Compute : m_stage = VK_SHADER_STAGE_COMPUTE_BIT; break;

			default : FatalError("wtf"); break;
		}
		m_module = kvfCreateShaderModule(RenderCore::Get().GetDevice(), m_bytecode.data(), m_bytecode.size() * 4);
		DebugLog("Vulkan : shader module created");

		GeneratePipelineLayout(m_layout);
	}

	void Shader::GeneratePipelineLayout(ShaderLayout layout)
	{
		for(auto& [n, set] : layout.set_layouts)
		{
			std::vector<VkDescriptorSetLayoutBinding> bindings(set.binds.size());
			for(std::size_t i = 0; i < set.binds.size(); i++)
			{
				bindings[i].binding = set.binds[i].first;
				bindings[i].descriptorCount = 1;
				bindings[i].descriptorType = set.binds[i].second;
				bindings[i].pImmutableSamplers = nullptr;
				bindings[i].stageFlags = m_stage;
			}
			m_set_layouts.emplace_back(kvfCreateDescriptorSetLayout(RenderCore::Get().GetDevice(), bindings.data(), bindings.size()));
			DebugLog("Vulkan : descriptor set layout created");
			m_pipeline_layout_part.set_layouts.push_back(m_set_layouts.back());
		}

		std::size_t i = 0;
		std::vector<VkPushConstantRange> push_constants(layout.push_constants.size());
		m_pipeline_layout_part.push_constants.resize(layout.push_constants.size());
		for(auto& pc : layout.push_constants)
		{
			VkPushConstantRange push_constant_range = {};
			push_constant_range.offset = pc.offset;
			push_constant_range.size = pc.size;
			push_constant_range.stageFlags = m_stage;
			push_constants[i] = push_constant_range;
			m_pipeline_layout_part.push_constants[i] = push_constant_range;
			i++;
		}
	}

	Shader::~Shader()
	{
		kvfDestroyShaderModule(RenderCore::Get().GetDevice(), m_module);
		DebugLog("Vulkan : shader module destroyed");
		for(auto& layout : m_set_layouts)
		{
			kvfDestroyDescriptorSetLayout(RenderCore::Get().GetDevice(), layout);
			DebugLog("Vulkan : descriptor set layout destroyed");
		}
	}

	std::shared_ptr<Shader> LoadShaderFromFile(const std::filesystem::path& filepath, ShaderType type, ShaderLayout layout)
	{
		std::ifstream stream(filepath, std::ios::binary);
		if(!stream.is_open())
			FatalError("Renderer : unable to open a spirv shader file, %", filepath);
		std::vector<std::uint32_t> data;
		stream.seekg(0);
		std::uint32_t part = 0;
		while(stream.read(reinterpret_cast<char*>(&part), sizeof(part)))
			data.push_back(part);
		stream.close();

		std::shared_ptr<Shader> shader = std::make_shared<Shader>(data, type, layout);
		DebugLog("Vulkan : shader loaded %", filepath);
		return shader;
	}
}
