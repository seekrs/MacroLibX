/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_core.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 19:16:32 by maldavid          #+#    #+#             */
/*   Updated: 2022/12/18 03:42:51 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_RENDER_CORE__
#define __MLX_RENDER_CORE__

#include <volk.h>
#include <array>
#include <memory>

#include "vk_queues.h"
#include "vk_device.h"
#include "vk_surface.h"
#include "vk_instance.h"
#include "vk_semaphore.h"

#include <renderer/command/vk_cmd_pool.h>
#include <renderer/command/vk_cmd_buffer.h>
#include <renderer/swapchain/vk_swapchain.h>
#include <renderer/swapchain/vk_render_pass.h>

#include <utils/singleton.h>
#include <core/errors.h>

namespace mlx
{
    namespace RCore
    {
        void checkVk(VkResult result);
    }

    constexpr const int MAX_FRAMES_IN_FLIGHT = 2;

    class Render_Core : public Singleton<Render_Core>
    {
        public:
            Render_Core();

            void init();

			void destroyCommandBuffers();
            void destroy();

            bool beginFrame();
            void endFrame();

            inline std::shared_ptr<class MLX_Window> getWindow() { return _window; }
            inline void setWindow(std::shared_ptr<class MLX_Window> window) { _window = window; }

            inline Instance&  getInstance()		   { return _instance; }
            inline Device&    getDevice()		   { return _device; }
            inline Surface&   getSurface()		   { return _surface; }
            inline Queues&    getQueue()		   { return _queues; }
            inline CmdPool&   getCmdPool()		   { return _cmd_pool; }
            inline SwapChain& getSwapChain()	   { return _swapchain; }
            inline Semaphore& getSemaphore()	   { return _semaphore; }
            inline RenderPass& getRenderPass()	   { return _pass; }
            inline CmdBuffer& getCmdBuffer(int i)  { return _cmd_buffers[i]; }
            inline CmdBuffer& getActiveCmdBuffer() { return _cmd_buffers[_active_image_index]; }
            inline uint32_t getActiveImageIndex()  { return _active_image_index; }
            inline uint32_t getImageIndex()        { return _image_index; }

            constexpr inline void requireFrameBufferResize(int index) noexcept { _framebufferResized = true; }

        private:
            Device _device;
            Queues _queues;
            Surface _surface;
            RenderPass _pass;
            CmdPool _cmd_pool;
            Instance _instance;
            SwapChain _swapchain;
            Semaphore _semaphore;
            std::array<CmdBuffer, MAX_FRAMES_IN_FLIGHT> _cmd_buffers;

			std::shared_ptr<class MLX_Window> _window;

            bool _framebufferResized = false;

            uint32_t _active_image_index = 0;
            uint32_t _image_index = 0;
            bool _is_init = false;
    };
}

#endif // __MLX_RENDER_CORE__
