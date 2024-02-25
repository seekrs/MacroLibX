/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_core.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 19:16:32 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/20 08:17:58 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_RENDER_CORE__
#define __MLX_RENDER_CORE__

#include <mlx_profile.h>
#include <volk.h>
#include <optional>

#include <renderer/command/single_time_cmd_manager.h>
#include <renderer/descriptors/descriptor_pool_manager.h>
#include <renderer/descriptors/vk_descriptor_pool.h>
#include "vk_queues.h"
#include "vk_device.h"
#include "vk_instance.h"
#include "vk_validation_layers.h"
#include "memory.h"

#include <utils/singleton.h>
#include <core/errors.h>

namespace mlx
{
	namespace RCore
	{
		std::optional<std::uint32_t> findMemoryType(std::uint32_t typeFilter, VkMemoryPropertyFlags properties, bool error = true);
		const char* verbaliseResultVk(VkResult result);
		VkPipelineStageFlags accessFlagsToPipelineStage(VkAccessFlags accessFlags, VkPipelineStageFlags stageFlags);
	}

	#ifdef DEBUG
		constexpr const bool enableValidationLayers = true;
	#else
		constexpr const bool enableValidationLayers = false;
	#endif

	const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

	constexpr const int MAX_FRAMES_IN_FLIGHT = 3;
	constexpr const int MAX_SETS_PER_POOL = 512;
	constexpr const int NUMBER_OF_UNIFORM_BUFFERS = 1; // change this if for wathever reason more than one uniform buffer is needed

	class Render_Core : public Singleton<Render_Core>
	{
		friend class Singleton<Render_Core>;

		public:
			void init();
			void destroy();

			inline bool isInit() const noexcept { return _is_init; }
			inline Instance& getInstance() noexcept { return _instance; }
			inline Device& getDevice() noexcept { return _device; }
			inline Queues& getQueue() noexcept { return _queues; }
			inline GPUallocator& getAllocator() noexcept { return _allocator; }
			inline ValidationLayers& getLayers() noexcept { return _layers; }
			inline CmdBuffer& getSingleTimeCmdBuffer() noexcept { return _cmd_manager.getCmdBuffer(); }
			inline SingleTimeCmdManager& getSingleTimeCmdManager() noexcept { return _cmd_manager; }
			inline DescriptorPool& getDescriptorPool() { return _pool_manager.getAvailablePool(); }

		private:
			Render_Core() = default;
			~Render_Core() = default;

		private:
			ValidationLayers _layers;
			SingleTimeCmdManager _cmd_manager;
			Queues _queues;
			DescriptorPoolManager _pool_manager;
			Device _device;
			Instance _instance;
			GPUallocator _allocator;
			bool _is_init = false;
	};
}

#endif // __MLX_RENDER_CORE__
