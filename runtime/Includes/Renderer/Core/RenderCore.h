/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RenderCore.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 19:16:32 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/27 22:55:43 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_RENDER_CORE__
#define __MLX_RENDER_CORE__

#include <Renderer/Command/SingleTimeCmdManager.h>
#include <Renderer/Descriptors/DescriptorPoolManager.h>
#include <Renderer/Descriptors/DescriptorPool.h>
#include <Renderer/Core/Queues.h>
#include <Renderer/Core/Device.h>
#include <Renderer/Core/Instance.h>
#include <Renderer/Core/ValidationLayers.h>
#include <Renderer/Core/memory.h>

#include <Utils/Singleton.h>

namespace mlx
{
	const char* VerbaliseVkResult(VkResult result);
	VkPipelineStageFlags AccessFlagsToPipelineStage(VkAccessFlags access_flags, VkPipelineStageFlags stage_flags);

	#ifdef DEBUG
		constexpr const bool enable_validation_layers = true;
	#else
		constexpr const bool enable_validation_layers = false;
	#endif

	const std::vector<const char*> validation_layers = { "VK_LAYER_KHRONOS_validation" };

	constexpr const int MAX_FRAMES_IN_FLIGHT = 3;
	constexpr const int MAX_SETS_PER_POOL = 512;
	constexpr const int NUMBER_OF_UNIFORM_BUFFERS = 1; // change this if for wathever reason more than one uniform buffer is needed

	class RenderCore : public Singleton<RenderCore>
	{
		friend class Singleton<RenderCore>;

		public:
			void Init();
			void Destroy();

			inline bool IsInit() const noexcept { return m_is_init; }
			inline Instance& GetInstance() noexcept { return m_instance; }
			inline Device& GetDevice() noexcept { return m_device; }
			inline Queues& GetQueue() noexcept { return m_queues; }
			inline GPUallocator& GetAllocator() noexcept { return m_allocator; }
			inline ValidationLayers& GetLayers() noexcept { return m_layers; }
			inline CommandBuffer& GetSingleTimeCmdBuffer() noexcept { return m_cmd_manager.GetCmdBuffer(); }
			inline SingleTimeCmdManager& GetSingleTimeCmdManager() noexcept { return m_cmd_manager; }
			inline DescriptorPool& GetDescriptorPool() { return m_pool_manager.GetAvailablePool(); }

		private:
			RenderCore() = default;
			~RenderCore() = default;

		private:
			ValidationLayers m_layers;
			SingleTimeCmdManager m_cmd_manager;
			Queues m_queues;
			DescriptorPoolManager m_pool_manager;
			Device m_device;
			Instance m_instance;
			GPUallocator m_allocator;
			bool m_is_init = false;
	};
}

#endif // __MLX_RENDER_CORE__
