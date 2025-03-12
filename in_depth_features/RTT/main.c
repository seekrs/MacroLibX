#include <math.h>
#include <stddef.h>
#include "../../includes/mlx.h"
#include "../../includes/mlx_extended.h"

typedef struct
{
	mlx_context mlx;
	mlx_window win;
	mlx_image render_target;
	mlx_window render_target_window;
} mlx_t;

void update(void* param)
{
	mlx_t* mlx = (mlx_t*)param;

	mlx_clear_window(mlx->mlx, mlx->win, (mlx_color){ .rgba = 0x334D4DFF });

	mlx_string_put(mlx->mlx, mlx->win, 160, 120, (mlx_color){ .rgba = 0xFF2066FF }, "text");
	mlx_string_put(mlx->mlx, mlx->win, 20, 50, (mlx_color){ .rgba = 0xFFFFFFFF }, "that's a text");

	for(int j = 0, color = 0; j < 400; j++)
	{
		mlx_pixel_put(mlx->mlx, mlx->win, j, j, (mlx_color){ .rgba = 0xFF0000FF + (color << 8) });
		mlx_pixel_put(mlx->mlx, mlx->win, 399 - j, j, (mlx_color){ .rgba = 0x0000FFFF });
		color += (color < 255);
	}

	for(int j = 0; j < 20; j++)
	{
		for(int k = 0; k < 20; k++)
			mlx_pixel_put(mlx->mlx, mlx->win, 220 + j, 160 + k, (mlx_color){ .rgba = 0xFF0000FF });
	}

	mlx_string_put(mlx->mlx, mlx->render_target_window, 20, 20, (mlx_color){ .rgba = 0xAF2BFFFF }, "yippeeee");
	for(int j = 0, color = 0; j < 200; j++)
	{
		mlx_pixel_put(mlx->mlx, mlx->render_target_window, j, j, (mlx_color){ .rgba = 0xFF0000FF + (color << 8) });
		mlx_pixel_put(mlx->mlx, mlx->render_target_window, 199 - j, j, (mlx_color){ .rgba = 0x0000FFFF });
		color += (color < 255);
	}

	static int i = 0;
	i++;
	mlx_put_transformed_image_to_window(mlx->mlx, mlx->win, mlx->render_target, 5, 100, 1.0f, 1.0f, i);
}

void key_hook(int key, void* param)
{
	mlx_t* mlx = (mlx_t*)param;
	if(key == 41)
		mlx_loop_end(mlx->mlx);
}

void window_hook(int event, void* param)
{
	if(event == 0)
		mlx_loop_end(((mlx_t*)param)->mlx);
}

int main(void)
{
	mlx_t mlx;

	mlx.mlx = mlx_init();

	mlx_set_fps_goal(mlx.mlx, 60);

	mlx_window_create_info info = { 0 };
	info.title = "My window";
	info.width = 400;
	info.height = 400;
	info.is_resizable = true;
	mlx.win = mlx_new_window(mlx.mlx, &info);

	mlx.render_target = mlx_new_image(mlx.mlx, 200, 200);
	info.render_target = mlx.render_target;
	info.title = NULL;
	info.width = 200;
	info.height = 200;
	mlx.render_target_window = mlx_new_window(mlx.mlx, &info);
	mlx_clear_window(mlx.mlx, mlx.render_target_window, (mlx_color){ .rgba = 0xC16868FF });

	mlx_on_event(mlx.mlx, mlx.win, MLX_KEYDOWN, key_hook, &mlx);
	mlx_on_event(mlx.mlx, mlx.win, MLX_WINDOW_EVENT, window_hook, &mlx);

	mlx_add_loop_hook(mlx.mlx, update, &mlx);
	mlx_loop(mlx.mlx);

	mlx_destroy_window(mlx.mlx, mlx.win);
	
	mlx_destroy_window(mlx.mlx, mlx.render_target_window);
	mlx_destroy_image(mlx.mlx, mlx.render_target);
	
	mlx_destroy_context(mlx.mlx);

	return 0;
}
