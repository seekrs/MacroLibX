#include "../../includes/mlx.h"

typedef struct
{
	void* mlx;
	void* win;
	void* render_target;
	void* render_target_win;
} mlx_t;

int update(void* param)
{
	mlx_t* mlx = (mlx_t*)param;

	mlx_clear_window(mlx->mlx, mlx->win, 0xFF334D4D);

	mlx_string_put(mlx->mlx, mlx->win, 160, 120, 0xFFFF2066, "text");
	mlx_string_put(mlx->mlx, mlx->win, 20, 50, 0xFFFFFFFF, "that's a text");

	for(int j = 0, color = 0; j < 400; j++)
	{
		mlx_pixel_put(mlx->mlx, mlx->win, j, j, 0xFFFF0000 + color);
		mlx_pixel_put(mlx->mlx, mlx->win, 399 - j, j, 0xFF0000FF);
		color += (color < 255);
	}

	for(int j = 0; j < 20; j++)
	{
		for(int k = 0; k < 20; k++)
			mlx_pixel_put(mlx->mlx, mlx->win, 220 + j, 160 + k, 0xFFFF0000);
	}

	mlx_string_put(mlx->mlx, mlx->render_target_win, 20, 20, 0xFFAF2BFF, "yippeeee");
	for(int j = 0, color = 0; j < 200; j++)
	{
		mlx_pixel_put(mlx->mlx, mlx->render_target_win, j, j, 0xFFFF0000 + color);
		mlx_pixel_put(mlx->mlx, mlx->render_target_win, 199 - j, j, 0xFF0000FF);
		color += (color < 255);
	}

	mlx_transform_put_image_to_window(mlx->mlx, mlx->win, mlx->render_target, 5, 250, 0.5f, 33.0f);

	return 0;
}

int key_hook(int key, void* param)
{
	mlx_t* mlx = (mlx_t*)param;
	if(key == 41)
		mlx_loop_end(mlx->mlx);
	return 0;
}

int window_hook(int event, void* param)
{
	if(event == 0)
		mlx_loop_end(((mlx_t*)param)->mlx);
	return 0;
}

int main(void)
{
	mlx_t mlx;

	mlx.mlx = mlx_init();
	mlx.win = mlx_new_resizable_window(mlx.mlx, 400, 400, "My window");

	mlx.render_target = mlx_new_image(mlx.mlx, 200, 200);
	mlx.render_target_win = mlx_new_window(mlx.mlx, 200, 200, (char*)mlx.render_target);
	mlx_clear_window(mlx.mlx, mlx.render_target_win, 0xFFC16868);

	mlx_on_event(mlx.mlx, mlx.win, MLX_KEYDOWN, key_hook, &mlx);
	mlx_on_event(mlx.mlx, mlx.win, MLX_WINDOW_EVENT, window_hook, &mlx);

	mlx_loop_hook(mlx.mlx, update, &mlx);
	mlx_loop(mlx.mlx);

	mlx_destroy_window(mlx.mlx, mlx.win);
	
	mlx_destroy_window(mlx.mlx, mlx.render_target_win);
	mlx_destroy_image(mlx.mlx, mlx.render_target);

	mlx_destroy_display(mlx.mlx);

	return 0;
}
