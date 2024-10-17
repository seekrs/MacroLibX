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
			Error("Vulkan : cannot transition descriptor image layout, unkown image type");
	}

	void DescriptorPool::Init() noexcept
	{
		VkDescriptorPoolSize pool_sizes[] = {
			{ VK_DESCRIPTOR_TYPE_SAMPLER, MAX_SETS_PER_POOL },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, MAX_SETS_PER_POOL },
			{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, MAX_SETS_PER_POOL },
			{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, MAX_SETS_PER_POOL },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, MAX_SETS_PER_POOL },
			{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, MAX_SETS_PER_POOL },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, MAX_SETS_PER_POOL },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, MAX_SETS_PER_POOL },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, MAX_SETS_PER_POOL },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, MAX_SETS_PER_POOL },
			{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, MAX_SETS_PER_POOL }
		};

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = sizeof(pool_sizes) / sizeof(pool_sizes[0]);
		poolInfo.pPoolSizes = pool_sizes;
		poolInfo.maxSets = MAX_SETS_PER_POOL;
		poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		for(std::size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			kvfCheckVk(RenderCore::Get().vkCreateDescriptorPool(RenderCore::Get().GetDevice(), &poolInfo, nullptr, &m_pools[i]));
		m_allocation_count = 0;
	}

	void DescriptorPool::Destroy() noexcept
	{
		for(std::size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			if(m_pools[i] == VK_NULL_HANDLE)
				continue;
			RenderCore::Get().vkDestroyDescriptorPool(RenderCore::Get().GetDevice(), m_pools[i], nullptr);
			m_pools[i] = VK_NULL_HANDLE;
		}
		m_allocation_count = 0;
	}

	VkDescriptorSet DescriptorPool::AllocateDescriptorSet(std::uint32_t frame_index, VkDescriptorSetLayout layout)
	{
		VkDescriptorSet set;
		VkDescriptorSetAllocateInfo alloc_info = {};
		alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		alloc_info.descriptorPool = m_pools[frame_index];
		alloc_info.descriptorSetCount = 1;
		alloc_info.pSetLayouts = &layout;
		kvfCheckVk(RenderCore::Get().vkAllocateDescriptorSets(RenderCore::Get().GetDevice(), &alloc_info, &set));
		m_allocation_count++;
		return set;
	}

	void DescriptorPool::ResetPoolFromFrameIndex(std::size_t frame_index)
	{
		Assert(frame_index < MAX_FRAMES_IN_FLIGHT, "invalid frame index");
		RenderCore::Get().vkResetDescriptorPool(RenderCore::Get().GetDevice(), m_pools[frame_index], 0);
	}

	void DescriptorPoolManager::ResetPoolsFromFrameIndex(std::size_t frame_index)
	{
		for(auto& pool : m_pools)
			pool.ResetPoolFromFrameIndex(frame_index);
	}

	DescriptorPool& DescriptorPoolManager::GetAvailablePool()
	{
		for(auto& pool : m_pools)
		{
			if(pool.GetNumberOfSetsAllocated() < MAX_SETS_PER_POOL)
				return pool;
		}
		m_pools.emplace_front().Init();
		return m_pools.front();
	}

	void DescriptorPoolManager::Destroy()
	{
		#pragma omp parallel for
		for(auto& pool : m_pools)
			pool.Destroy();
	}

	DescriptorSet::DescriptorSet(DescriptorPoolManager& pools_manager, const ShaderSetLayout& layout, VkDescriptorSetLayout vklayout, ShaderType shader_type)
	: m_set_layout(vklayout), p_pools_manager(&pools_manager)
	{
		MLX_PROFILE_FUNCTION();
		for(auto& [binding, type] : layout.binds)
		{
			m_descriptors.emplace_back();
			m_descriptors.back().type = type;
			m_descriptors.back().shader_type = shader_type;
			m_descriptors.back().binding = binding;
		}
		for(std::size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			m_set[i] = p_pools_manager->GetAvailablePool().AllocateDescriptorSet(i, vklayout);
	}

	DescriptorSet::DescriptorSet(DescriptorPoolManager& pools_manager, VkDescriptorSetLayout layout, const std::vector<Descriptor>& descriptors)
	: m_descriptors(descriptors), m_set_layout(layout), p_pools_manager(&pools_manager)
	{
		MLX_PROFILE_FUNCTION();
		for(std::size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			m_set[i] = p_pools_manager->GetAvailablePool().AllocateDescriptorSet(i, layout);
	}

	void DescriptorSet::SetImage(std::size_t i, std::uint32_t binding, class Image& image)
	{
		MLX_PROFILE_FUNCTION();
		Verify(m_set[i] != VK_NULL_HANDLE, "invalid descriptor");
		auto it = std::find_if(m_descriptors.begin(), m_descriptors.end(), [=](Descriptor descriptor)
		{
			return binding == descriptor.binding;
		});
		if(it == m_descriptors.end())
		{
			Warning("Vulkan : cannot update descriptor set image; invalid binding");
			return;
		}
		if(it->type != VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
		{
			Error("Vulkan : trying to bind an image to the wrong descriptor");
			return;
		}
		it->image_ptr = &image;
	}

	void DescriptorSet::SetStorageBuffer(std::size_t i, std::uint32_t binding, class GPUBuffer& buffer)
	{
		MLX_PROFILE_FUNCTION();
		Verify(m_set[i] != VK_NULL_HANDLE, "invalid descriptor");
		auto it = std::find_if(m_descriptors.begin(), m_descriptors.end(), [=](Descriptor descriptor)
		{
			return binding == descriptor.binding;
		});
		if(it == m_descriptors.end())
		{
			Warning("Vulkan : cannot update descriptor set buffer; invalid binding");
			return;
		}
		if(it->type != VK_DESCRIPTOR_TYPE_STORAGE_BUFFER)
		{
			Error("Vulkan : trying to bind a buffer to the wrong descriptor");
			return;
		}
		it->storage_buffer_ptr = &buffer;
	}

	void DescriptorSet::SetUniformBuffer(std::size_t i, std::uint32_t binding, class GPUBuffer& buffer)
	{
		MLX_PROFILE_FUNCTION();
		Verify(m_set[i] != VK_NULL_HANDLE, "invalid descriptor");
		auto it = std::find_if(m_descriptors.begin(), m_descriptors.end(), [=](Descriptor descriptor)
		{
			return binding == descriptor.binding;
		});
		if(it == m_descriptors.end())
		{
			Warning("Vulkan : cannot update descriptor set buffer; invalid binding");
			return;
		}
		if(it->type != VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
		{
			Error("Vulkan : trying to bind a buffer to the wrong descriptor");
			return;
		}
		it->uniform_buffer_ptr = &buffer;
	}

	void DescriptorSet::Update(std::size_t i, VkCommandBuffer cmd) noexcept
	{
		MLX_PROFILE_FUNCTION();
		Verify(m_set[i] != VK_NULL_HANDLE, "invalid descriptor");
		std::vector<VkWriteDescriptorSet> writes;
		std::vector<VkDescriptorBufferInfo> buffer_infos;
		std::vector<VkDescriptorImageInfo> image_infos;
		for(auto& descriptor : m_descriptors)
		{
			if(descriptor.image_ptr)
			{
				TransitionImageToCorrectLayout(*descriptor.image_ptr, cmd);
				VkDescriptorImageInfo info{};
				info.sampler = descriptor.image_ptr->GetSampler();
				info.imageLayout = descriptor.image_ptr->GetLayout();
				info.imageView = descriptor.image_ptr->GetImageView();
				image_infos.push_back(info);
				writes.push_back(kvfWriteImageToDescriptorSet(RenderCore::Get().GetDevice(), m_set[i], &image_infos.back(), descriptor.binding));
			}
			else if(descriptor.uniform_buffer_ptr)
			{
				VkDescriptorBufferInfo info{};
				info.buffer = descriptor.uniform_buffer_ptr->Get();
				info.offset = descriptor.uniform_buffer_ptr->GetOffset();
				info.range = VK_WHOLE_SIZE;
				buffer_infos.push_back(info);
				writes.push_back(kvfWriteUniformBufferToDescriptorSet(RenderCore::Get().GetDevice(), m_set[i], &buffer_infos.back(), descriptor.binding));
			}
			else if(descriptor.storage_buffer_ptr)
			{
				VkDescriptorBufferInfo info{};
				info.buffer = descriptor.storage_buffer_ptr->Get();
				info.offset = descriptor.storage_buffer_ptr->GetOffset();
				info.range = VK_WHOLE_SIZE;
				buffer_infos.push_back(info);
				writes.push_back(kvfWriteStorageBufferToDescriptorSet(RenderCore::Get().GetDevice(), m_set[i], &buffer_infos.back(), descriptor.binding));
			}
		}
		RenderCore::Get().vkUpdateDescriptorSets(RenderCore::Get().GetDevice(), writes.size(), writes.data(), 0, nullptr);
	}

	void DescriptorSet::Reallocate(std::size_t frame_index) noexcept
	{
		MLX_PROFILE_FUNCTION();
		m_set[frame_index] = p_pools_manager->GetAvailablePool().AllocateDescriptorSet(frame_index, m_set_layout);
	}
}
