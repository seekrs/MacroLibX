#include <stdio.h>
#include <stddef.h>
#include "../../includes/mlx.h"
#include "../../includes/mlx_extended.h"

#include <vulkan/vulkan_core.h>

typedef VkInstance (*PFN_mlx_get_vk_instance)(mlx_context mlx);

int main(void)
{
	mlx_context mlx = mlx_init();

	mlx_window_create_info info = { 0 };
	info.title = "My window";
	info.width = 400;
	info.height = 400;
	info.is_resizable = true;
	mlx_window win = mlx_new_window(mlx, &info);

	PFN_mlx_get_vk_instance mlx_get_vk_instance = (PFN_mlx_get_vk_instance)mlx_get_proc_addr(mlx, "mlx_get_vk_instance");
	printf("%p\n", mlx_get_vk_instance(mlx));

	mlx_destroy_window(mlx, win);
	mlx_destroy_context(mlx);

	return 0;
}
