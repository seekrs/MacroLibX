#include <PreCompiled.h>

#include <Renderer/Image.h>
#include <Renderer/Enums.h>
#include <Renderer/Buffer.h>
#include <Renderer/Descriptor.h>
#include <Renderer/RenderCore.h>
#include <Maths/Constants.h>

namespace mlx
{
	constexpr std::size_t MAX_SETS_PER_POOL = MAX_FRAMES_IN_FLIGHT * 1024;

	void TransitionImageToCorrectLayout(Image& image, VkCommandBuffer cmd)
	{
		MLX_PROFILE_FUNCTION();
		if(!image.IsInit())
			return;
		if(image.GetType() == ImageType::Color)
			image.TransitionLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, cmd);
		else
			Error("Vulkan: cannot transition descriptor image layout, unkown image type");
	}

	void DescriptorPool::Init() noexcept
	{
		MLX_PROFILE_FUNCTION();
		VkDescriptorPoolSize pool_sizes[] = {
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, MAX_SETS_PER_POOL },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, MAX_SETS_PER_POOL },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, MAX_SETS_PER_POOL }
		};

		VkDescriptorPoolCreateInfo pool_info{};
		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.poolSizeCount = sizeof(pool_sizes) / sizeof(pool_sizes[0]);
		pool_info.pPoolSizes = pool_sizes;
		pool_info.maxSets = MAX_SETS_PER_POOL;
		pool_info.flags = 0;
		kvfCheckVk(RenderCore::Get().vkCreateDescriptorPool(RenderCore::Get().GetDevice(), &pool_info, nullptr, &m_pool));
		m_allocation_count = 0;
	}

	void DescriptorPool::Destroy() noexcept
	{
		MLX_PROFILE_FUNCTION();
		if(m_pool == VK_NULL_HANDLE)
			return;
		for(auto& set : m_free_sets)
			kvfDestroyDescriptorSetLayout(RenderCore::Get().GetDevice(), set->m_set_layout);
		for(auto& set : m_used_sets)
			kvfDestroyDescriptorSetLayout(RenderCore::Get().GetDevice(), set->m_set_layout);
		RenderCore::Get().vkDestroyDescriptorPool(RenderCore::Get().GetDevice(), m_pool, nullptr);
		m_pool = VK_NULL_HANDLE;
		m_allocation_count = 0;
		m_free_sets.clear();
		m_used_sets.clear();
	}

	std::shared_ptr<DescriptorSet> DescriptorPool::RequestDescriptorSet(const ShaderSetLayout& layout, ShaderType shader_type)
	{
		MLX_PROFILE_FUNCTION();
		auto it = std::find_if(m_free_sets.begin(), m_free_sets.end(), [&](std::shared_ptr<DescriptorSet> set)
		{
			return shader_type == set->GetShaderType() && layout == set->GetShaderLayout();
		});
		if(it != m_free_sets.end())
		{
			std::shared_ptr<DescriptorSet> set = *it;
			m_free_sets.erase(it);
			m_used_sets.push_back(set);
			return set;
		}

		std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> vulkan_sets;

		VkShaderStageFlagBits vulkan_shader_stage;
		switch(shader_type)
		{
			case ShaderType::Vertex: vulkan_shader_stage = VK_SHADER_STAGE_VERTEX_BIT; break;
			case ShaderType::Fragment: vulkan_shader_stage = VK_SHADER_STAGE_FRAGMENT_BIT; break;

			default: FatalError("wtf"); vulkan_shader_stage = VK_SHADER_STAGE_VERTEX_BIT; /* Just to shut up warnings */ break;
		}

		std::vector<VkDescriptorSetLayoutBinding> bindings(layout.binds.size());
		for(std::size_t i = 0; i < layout.binds.size(); i++)
		{
			bindings[i].binding = layout.binds[i].first;
			bindings[i].descriptorCount = 1;
			bindings[i].descriptorType = layout.binds[i].second;
			bindings[i].pImmutableSamplers = nullptr;
			bindings[i].stageFlags = vulkan_shader_stage;
		}
		VkDescriptorSetLayout vulkan_layout = kvfCreateDescriptorSetLayout(RenderCore::Get().GetDevice(), bindings.data(), bindings.size());

		for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			VkDescriptorSetAllocateInfo alloc_info = {};
			alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			alloc_info.descriptorPool = m_pool;
			alloc_info.descriptorSetCount = 1;
			alloc_info.pSetLayouts = &vulkan_layout;
			VkDescriptorSet vulkan_set;
			kvfCheckVk(RenderCore::Get().vkAllocateDescriptorSets(RenderCore::Get().GetDevice(), &alloc_info, &vulkan_set));
			m_allocation_count++;
			vulkan_sets[i] = vulkan_set;
		}

		std::shared_ptr<DescriptorSet> set(new DescriptorSet(*this, vulkan_layout, layout, std::move(vulkan_sets), shader_type));
		m_used_sets.push_back(set);
		return set;
	}

	void DescriptorPool::ReturnDescriptorSet(std::shared_ptr<class DescriptorSet> set)
	{
		auto it = std::find_if(m_used_sets.begin(), m_used_sets.end(), [&](std::shared_ptr<DescriptorSet> rhs_set)
		{
			return set == rhs_set;
		});
		if(it == m_used_sets.end())
			return;
		m_used_sets.erase(it);
		m_free_sets.push_back(set);
	}

	DescriptorPool& DescriptorPoolManager::GetAvailablePool()
	{
		MLX_PROFILE_FUNCTION();
		for(auto& pool : m_pools)
		{
			if(pool.GetNumberOfSetsAllocated() < MAX_SETS_PER_POOL)
				return pool;
		}
		m_pools.emplace_back().Init();
		return m_pools.back();
	}

	void DescriptorPoolManager::Destroy()
	{
		MLX_PROFILE_FUNCTION();
		for(auto& pool : m_pools)
			pool.Destroy();
		m_pools.clear();
	}

	DescriptorSet::DescriptorSet(DescriptorPool& pool, VkDescriptorSetLayout vulkan_layout, const ShaderSetLayout& layout, std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> vulkan_sets, ShaderType shader_type) :
		m_shader_layout(layout),
		m_sets(std::move(vulkan_sets)),
		m_set_layout(vulkan_layout),
		m_shader_type(shader_type),
		m_pool(pool)
	{
		MLX_PROFILE_FUNCTION();
		for(auto& [binding, type] : layout.binds)
		{
			m_descriptors.emplace_back();
			m_descriptors.back().type = type;
			m_descriptors.back().binding = binding;
		}
	}

	void DescriptorSet::SetImage(std::size_t i, std::uint32_t binding, class Image& image)
	{
		MLX_PROFILE_FUNCTION();
		Verify(m_sets[i] != VK_NULL_HANDLE, "invalid descriptor");
		auto it = std::find_if(m_descriptors.begin(), m_descriptors.end(), [=](Descriptor descriptor)
		{
			return binding == descriptor.binding;
		});
		if(it == m_descriptors.end())
		{
			Warning("Vulkan: cannot update descriptor set image; invalid binding");
			return;
		}
		if(it->type != VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
		{
			Error("Vulkan: trying to bind an image to the wrong descriptor");
			return;
		}
		it->image_ptr = &image;
	}

	void DescriptorSet::SetStorageBuffer(std::size_t i, std::uint32_t binding, class GPUBuffer& buffer)
	{
		MLX_PROFILE_FUNCTION();
		Verify(m_sets[i] != VK_NULL_HANDLE, "invalid descriptor");
		auto it = std::find_if(m_descriptors.begin(), m_descriptors.end(), [=](Descriptor descriptor)
		{
			return binding == descriptor.binding;
		});
		if(it == m_descriptors.end())
		{
			Warning("Vulkan: cannot update descriptor set buffer; invalid binding");
			return;
		}
		if(it->type != VK_DESCRIPTOR_TYPE_STORAGE_BUFFER)
		{
			Error("Vulkan: trying to bind a buffer to the wrong descriptor");
			return;
		}
		it->storage_buffer_ptr = &buffer;
	}

	void DescriptorSet::SetUniformBuffer(std::size_t i, std::uint32_t binding, class GPUBuffer& buffer)
	{
		MLX_PROFILE_FUNCTION();
		Verify(m_sets[i] != VK_NULL_HANDLE, "invalid descriptor");
		auto it = std::find_if(m_descriptors.begin(), m_descriptors.end(), [=](Descriptor descriptor)
		{
			return binding == descriptor.binding;
		});
		if(it == m_descriptors.end())
		{
			Warning("Vulkan: cannot update descriptor set buffer; invalid binding");
			return;
		}
		if(it->type != VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
		{
			Error("Vulkan: trying to bind a buffer to the wrong descriptor");
			return;
		}
		it->uniform_buffer_ptr = &buffer;
	}

	void DescriptorSet::Update(std::size_t i, VkCommandBuffer cmd) noexcept
	{
		MLX_PROFILE_FUNCTION();
		Verify(m_sets[i] != VK_NULL_HANDLE, "invalid descriptor");

		std::size_t image_count = 0;
		std::size_t buffer_count = 0;

		for(auto& descriptor : m_descriptors)
		{
			if(descriptor.image_ptr)
				image_count++;
			else if(descriptor.uniform_buffer_ptr || descriptor.storage_buffer_ptr)
				buffer_count++;
			else
				FatalError("unknown descriptor data");
		}

		std::vector<VkWriteDescriptorSet> writes(m_descriptors.size());
		std::vector<VkDescriptorBufferInfo> buffer_infos(buffer_count);
		std::vector<VkDescriptorImageInfo> image_infos(image_count);
		std::size_t buffer_index = 0;
		std::size_t image_index = 0;
		std::size_t write_index = 0;

		for(auto& descriptor : m_descriptors)
		{
			if(descriptor.image_ptr)
			{
				TransitionImageToCorrectLayout(*descriptor.image_ptr, cmd);
				VkDescriptorImageInfo info{};
				info.sampler = descriptor.image_ptr->GetSampler();
				info.imageLayout = descriptor.image_ptr->GetLayout();
				info.imageView = descriptor.image_ptr->GetImageView();
				image_infos[image_index] = std::move(info);
				writes[write_index] = kvfWriteImageToDescriptorSet(RenderCore::Get().GetDevice(), m_sets[i], &image_infos[image_index], descriptor.binding);
				image_index++;
			}
			else if(descriptor.uniform_buffer_ptr)
			{
				VkDescriptorBufferInfo info{};
				info.buffer = descriptor.uniform_buffer_ptr->Get();
				info.offset = descriptor.uniform_buffer_ptr->GetOffset();
				info.range = VK_WHOLE_SIZE;
				buffer_infos[buffer_index] = std::move(info);
				writes[write_index] = kvfWriteUniformBufferToDescriptorSet(RenderCore::Get().GetDevice(), m_sets[i], &buffer_infos[buffer_index], descriptor.binding);
				buffer_index++;
			}
			else if(descriptor.storage_buffer_ptr)
			{
				VkDescriptorBufferInfo info{};
				info.buffer = descriptor.storage_buffer_ptr->Get();
				info.offset = descriptor.storage_buffer_ptr->GetOffset();
				info.range = VK_WHOLE_SIZE;
				buffer_infos[buffer_index] = std::move(info);
				writes[write_index] = kvfWriteStorageBufferToDescriptorSet(RenderCore::Get().GetDevice(), m_sets[i], &buffer_infos[buffer_index], descriptor.binding);
				buffer_index++;
			}
			write_index++;
		}
		RenderCore::Get().vkUpdateDescriptorSets(RenderCore::Get().GetDevice(), writes.size(), writes.data(), 0, nullptr);
	}

	void DescriptorSet::ReturnDescriptorSetToPool()
	{
		m_pool.ReturnDescriptorSet(shared_from_this());
	}
}
