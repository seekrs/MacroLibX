#include <PreCompiled.h>

#define KVF_IMPLEMENTATION
#ifdef DEBUG
	#define KVF_ENABLE_VALIDATION_LAYERS
#endif

#include <kvf.h>

#include <Renderer/RenderCore.h>
#include <Renderer/Vulkan/VulkanLoader.h>
#include <Core/SDLManager.h>
#include <Platform/Window.h>
#include <Maths/Mat4.h>

namespace mlx
{
	static std::unique_ptr<VulkanLoader> loader;

	void ErrorCallback(const char* message) noexcept
	{
		FatalError(message, 0, "", "");
		std::cout << std::endl;
	}

	void ValidationErrorCallback(const char* message) noexcept
	{
		Error(message, 0, "", "");
		std::cout << std::endl;
	}

	void ValidationWarningCallback(const char* message) noexcept
	{
		Warning(message, 0, "", "");
		std::cout << std::endl;
	}

	void RenderCore::Init() noexcept
	{
		loader = std::make_unique<VulkanLoader>();

		kvfSetErrorCallback(&ErrorCallback);
		kvfSetValidationErrorCallback(&ValidationErrorCallback);
		kvfSetValidationWarningCallback(&ValidationWarningCallback);

		//kvfAddLayer("VK_LAYER_MESA_overlay");

		Window window(1, 1, "", true);
		std::vector<const char*> instance_extensions = window.GetRequiredVulkanInstanceExtentions();
		#ifdef MLX_PLAT_MACOS
			instance_extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
		#endif

		m_instance = kvfCreateInstance(instance_extensions.data(), instance_extensions.size());
		DebugLog("Vulkan : instance created");

		loader->LoadInstance(m_instance);

		VkSurfaceKHR surface = window.CreateVulkanSurface(m_instance);

		m_physical_device = kvfPickGoodDefaultPhysicalDevice(m_instance, surface);

		// just for style
		VkPhysicalDeviceProperties props;
		vkGetPhysicalDeviceProperties(m_physical_device, &props);
		DebugLog("Vulkan : physical device picked '%'", props.deviceName);

		const char* device_extensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
		VkPhysicalDeviceFeatures features{};
		vkGetPhysicalDeviceFeatures(m_physical_device, &features);
		m_device = kvfCreateDevice(m_physical_device, device_extensions, sizeof(device_extensions) / sizeof(device_extensions[0]), &features);
		DebugLog("Vulkan : logical device created");

		vkDestroySurfaceKHR(m_instance, surface, nullptr);
		window.Destroy();
	}

	void RenderCore::Destroy() noexcept
	{
		WaitDeviceIdle();
		kvfDestroyDevice(m_device);
		DebugLog("Vulkan : logical device destroyed");
		kvfDestroyInstance(m_instance);
		DebugLog("Vulkan : instance destroyed");
		loader.reset();
	}
}
