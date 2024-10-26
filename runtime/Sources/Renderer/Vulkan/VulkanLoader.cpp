#include <PreCompiled.h>
#include <Renderer/Vulkan/VulkanLoader.h>
#include <Renderer/RenderCore.h>

#ifdef MLX_PLAT_WINDOWS
	__declspec(dllimport) HMODULE __stdcall LoadLibraryA(LPCSTR);
	__declspec(dllimport) FARPROC __stdcall GetProcAddress(HMODULE, LPCSTR);
	__declspec(dllimport) int __stdcall FreeLibrary(HMODULE);
	using LibModule = HMODULE;
#else
	using LibModule = Handle;
#endif

#if defined(MLX_COMPILER_GCC)
	#define DISABLE_GCC_PEDANTIC_WARNINGS \
		_Pragma("GCC diagnostic push") \
		_Pragma("GCC diagnostic ignored \"-Wpedantic\"")
	#define RESTORE_GCC_PEDANTIC_WARNINGS \
		_Pragma("GCC diagnostic pop")
#else
	#define DISABLE_GCC_PEDANTIC_WARNINGS
	#define RESTORE_GCC_PEDANTIC_WARNINGS
#endif

namespace mlx
{
	namespace Internal
	{
		static inline PFN_vkVoidFunction vkGetInstanceProcAddrStub(Handle context, const char* name)
		{
			PFN_vkVoidFunction function = RenderCore::Get().vkGetInstanceProcAddr(static_cast<VkInstance>(context), name);
			if(!function)
				FatalError("Vulkan Loader: could not load '%'", name);
			//DebugLog("Vulkan Loader: loaded %", name);
			return function;
		}

		static inline PFN_vkVoidFunction vkGetDeviceProcAddrStub(Handle context, const char* name)
		{
			PFN_vkVoidFunction function = RenderCore::Get().vkGetDeviceProcAddr(static_cast<VkDevice>(context), name);
			if(!function)
				FatalError("Vulkan Loader: could not load '%'", name);
			//DebugLog("Vulkan Loader: loaded %", name);
			return function;
		}

		static inline LibModule LoadLib(const char* libname)
		{
			#ifdef MLX_PLAT_WINDOWS
				return LoadLibraryA(libname);
			#else
				return dlopen(libname, RTLD_NOW | RTLD_LOCAL);
			#endif
		}

		static inline void* GetSymbol(LibModule module, const char* name)
		{
			#ifdef MLX_PLAT_WINDOWS
				return (void*)(void(*)(void))GetProcAddress(module, name);
			#else
				return dlsym(module, name);
			#endif
		}
	}

	VulkanLoader::VulkanLoader()
	{
		#if defined(MLX_PLAT_WINDOWS)
			std::array libnames{
				"vulkan-1.dll"
			};
		#elif defined(MLX_PLAT_MACOS)
			std::array libnames{
				"libvulkan.dylib",
				"libvulkan.1.dylib",
				"libMoltenVK.dylib",
				"vulkan.framework/vulkan",
				"MoltenVK.framework/MoltenVK",
				"/usr/local/lib/libvulkan.dylib",
			};
		#else
			std::array libnames{
				"libvulkan.so.1",
				"libvulkan.so"
			};
		#endif

		for(auto libname : libnames)
		{
			p_module = Internal::LoadLib(libname);
			if(p_module != nullptr)
			{
				DISABLE_GCC_PEDANTIC_WARNINGS
				RenderCore::Get().vkGetInstanceProcAddr = reinterpret_cast<PFN_vkGetInstanceProcAddr>(Internal::GetSymbol(p_module, "vkGetInstanceProcAddr"));
				RESTORE_GCC_PEDANTIC_WARNINGS
				if(RenderCore::Get().vkGetInstanceProcAddr)
				{
					DebugLog("Vulkan Loader: libvulkan loaded using '%'", libname);
					break;
				}
			}
		}
		if(!p_module || !RenderCore::Get().vkGetInstanceProcAddr)
			FatalError("Vulkan Loader: failed to load libvulkan");
		LoadGlobalFunctions(nullptr, Internal::vkGetInstanceProcAddrStub);
	}

	void VulkanLoader::LoadInstance(VkInstance instance)
	{
		LoadInstanceFunctions(instance, Internal::vkGetInstanceProcAddrStub);
	}

	void VulkanLoader::LoadDevice(VkDevice device)
	{
		LoadDeviceFunctions(device, Internal::vkGetDeviceProcAddrStub);
	}

	void VulkanLoader::LoadGlobalFunctions(void* context, PFN_vkVoidFunction (*load)(void*, const char*)) noexcept
	{
		#define MLX_VULKAN_GLOBAL_FUNCTION(fn) RenderCore::Get().fn = reinterpret_cast<PFN_##fn>(load(context, #fn));
			#include <Renderer/Vulkan/VulkanDefs.h>
		#undef MLX_VULKAN_GLOBAL_FUNCTION
		DebugLog("Vulkan Loader: global functions loaded");
	}

	void VulkanLoader::LoadInstanceFunctions(void* context, PFN_vkVoidFunction (*load)(void*, const char*)) noexcept
	{
		#define MLX_VULKAN_INSTANCE_FUNCTION(fn) RenderCore::Get().fn = reinterpret_cast<PFN_##fn>(load(context, #fn));
			#include <Renderer/Vulkan/VulkanDefs.h>
		#undef MLX_VULKAN_INSTANCE_FUNCTION
		DebugLog("Vulkan Loader: instance functions loaded");
	}

	void VulkanLoader::LoadDeviceFunctions(void* context, PFN_vkVoidFunction (*load)(void*, const char*)) noexcept
	{
		#define MLX_VULKAN_DEVICE_FUNCTION(fn) RenderCore::Get().fn = reinterpret_cast<PFN_##fn>(load(context, #fn));
			#include <Renderer/Vulkan/VulkanDefs.h>
		#undef MLX_VULKAN_DEVICE_FUNCTION
		DebugLog("Vulkan Loader: device functions loaded");
	}

	VulkanLoader::~VulkanLoader()
	{
		#ifdef MLX_PLAT_WINDOWS
			FreeLibrary(p_module);
		#else
			dlclose(p_module);
		#endif
		p_module = nullptr;
		DebugLog("Vulkan Loader: libvulkan unloaded");
	}
}
