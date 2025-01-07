#include <stdio.h>
#include <math.h>
#include "../includes/mlx.h"
#include "../includes/mlx_extended.h"

typedef struct
{
	mlx_context mlx;
	mlx_window win;
	mlx_image logo_png;
	mlx_image logo_jpg;
	mlx_image logo_bmp;
	mlx_image img;
} mlx_t;

#define CIRCLE_RADIUS 50
#define CIRCLE_DIAMETER (CIRCLE_RADIUS + CIRCLE_RADIUS)

static mlx_color pixels_circle[CIRCLE_DIAMETER * CIRCLE_DIAMETER] = { 0 };

#define THRESHOLD 200

void update(void* param)
{
	static int i = 0;
	mlx_t* mlx = (mlx_t*)param;

	if(i > THRESHOLD)
	{
		mlx_clear_window(mlx->mlx, mlx->win, (mlx_color){ .rgba = 0x334D4DFF });
		mlx_put_transformed_image_to_window(mlx->mlx, mlx->win, mlx->logo_bmp, 220, 40, 0.5f, 0.5f, i);
	}

	if(i >= THRESHOLD + THRESHOLD / 4)
		mlx_set_font_scale(mlx->mlx, "default", 16.f);
	else
		mlx_set_font_scale(mlx->mlx, "default", 6.f);

	mlx_string_put(mlx->mlx, mlx->win, 160, 120, (mlx_color){ .rgba = 0xFF2066FF }, "this text should be behind");

	mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->logo_png, 100, 100);
	mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->img, 150, 60);

	mlx_set_font(mlx->mlx, "default");
	mlx_string_put(mlx->mlx, mlx->win, 20, 50, (mlx_color){ .rgba = 0xFFFFFFFF }, "that's a text");

	uint32_t color = 0;
	for(int j = 0; j < 400; j++)
	{
		mlx_pixel_put(mlx->mlx, mlx->win, j, j, (mlx_color){ .rgba = 0x0000FFFF + (color << 24) });
		mlx_pixel_put(mlx->mlx, mlx->win, 399 - j, j, (mlx_color){ .rgba = 0x0000FFFF });
		color += (color < 255);
	}

	if(i < THRESHOLD)
		mlx_put_transformed_image_to_window(mlx->mlx, mlx->win, mlx->logo_jpg, 210, 150, 0.5f, 2.0f, 0.0f);
	else
		mlx_put_transformed_image_to_window(mlx->mlx, mlx->win, mlx->logo_jpg, 210, 150, fabs(sin(i / 100.0f)), fabs(cos(i / 100.0f) * 2.0f), 0.0f);
	mlx_set_font_scale(mlx->mlx, "default", 8.f);
	mlx_string_put(mlx->mlx, mlx->win, 210, 175, (mlx_color){ .rgba = 0xFFAF2BFF }, "hidden");

	mlx_pixel_put_region(mlx->mlx, mlx->win, 200, 170, CIRCLE_DIAMETER, CIRCLE_DIAMETER, pixels_circle);

	i++; // Will overflow and I don't care
}

mlx_image create_image(mlx_t* mlx)
{
	mlx_image img = mlx_new_image(mlx->mlx, 100, 100);
	for(int i = 0, j = 0, k = 0; i < (100 * 100) * 4; i += 4, j++)
	{
		if(j >= 100)
		{
			j = 0;
			k++;
		}
		if(i < 10000 || i > 20000)
		{
			mlx_color pixel = {
				.r = (uint8_t)k,
				.g = (uint8_t)j,
				.b = (uint8_t)i,
				.a = 0x99
			};
			mlx_set_image_pixel(mlx->mlx, img, j, k, pixel);
		}
	}
	return img;
}

void key_hook(int key, void* param)
{
	int x;
	int y;
	mlx_t* mlx = (mlx_t*)param;

	mlx_mouse_get_pos(mlx->mlx, &x, &y);
	switch(key)
	{
		case 41 : // ESCAPE
			mlx_loop_end(mlx->mlx);
		break;
		case 22 : // (S)how
			mlx_mouse_show(mlx->mlx);
		break;
		case 11 : // (H)ide
			mlx_mouse_hide(mlx->mlx);
		break;
		case 6 : // (C)lear
			mlx_clear_window(mlx->mlx, mlx->win, (mlx_color){ .rgba = 0x334D4DFF });
		break;
		case 79 : // RIGHT KEY
			mlx_mouse_move(mlx->mlx, mlx->win, x + 10, y);
		break;
		case 80 : // LEFT KEY
			mlx_mouse_move(mlx->mlx, mlx->win, x - 10, y);
		break;
		case 81 : // UP KEY
			mlx_mouse_move(mlx->mlx, mlx->win, x, y + 10);
		break;
		case 82 : // DOWN KEY
			mlx_mouse_move(mlx->mlx, mlx->win, x, y - 10);
		break;

		default : break;
	}
}

void window_hook(int event, void* param)
{
	if(event == 0)
		mlx_loop_end(((mlx_t*)param)->mlx);
}

#include <stdlib.h>

int main(void)
{
	mlx_t mlx;
	int w;
	int h;
	int dummy;

	int i = 0;
	for(int j = 0; j < CIRCLE_DIAMETER; j++)
	{
		for(int k = 0; k < CIRCLE_DIAMETER; k++, i++)
		{
			if((CIRCLE_RADIUS - j) * (CIRCLE_RADIUS - j) + (CIRCLE_RADIUS - k) * (CIRCLE_RADIUS - k) < CIRCLE_RADIUS * CIRCLE_RADIUS)
				pixels_circle[i] = (mlx_color){ .rgba = 0xA10000FF + ((j * k * i) << 8) };
		}
	}

	mlx.mlx = mlx_init();

	mlx_window_create_info info = { 0 };
	info.title = "My window";
	info.width = 400;
	info.height = 400;
	info.is_resizable = true;
	mlx.win = mlx_new_window(mlx.mlx, &info);

	mlx_get_screen_size(mlx.mlx, mlx.win, &w, &h);
	printf("screen size : %dx%d\n", w, h);

	mlx_set_fps_goal(mlx.mlx, 60);

	mlx_on_event(mlx.mlx, mlx.win, MLX_KEYDOWN, key_hook, &mlx);
	mlx_on_event(mlx.mlx, mlx.win, MLX_WINDOW_EVENT, window_hook, &mlx);

	mlx.logo_bmp = mlx_new_image_from_file(mlx.mlx, "42_logo.bmp", &dummy, &dummy);
	mlx.logo_png = mlx_new_image_from_file(mlx.mlx, "42_logo.png", &dummy, &dummy);
	mlx.logo_jpg = mlx_new_image_from_file(mlx.mlx, "42_logo.jpg", &dummy, &dummy);

	mlx_pixel_put(mlx.mlx, mlx.win, 200, 10, (mlx_color){ .rgba = 0xFF00FFFF });
	mlx_put_image_to_window(mlx.mlx, mlx.win, mlx.logo_png, 0, 0);

	mlx.img = create_image(&mlx);

	mlx_set_font_scale(mlx.mlx, "font.ttf", 16.f);
	mlx_string_put(mlx.mlx, mlx.win, 20, 20, (mlx_color){ .rgba = 0x0020FFFF }, "that text will disappear");

	mlx_add_loop_hook(mlx.mlx, update, &mlx);
	mlx_loop(mlx.mlx);

	mlx_destroy_image(mlx.mlx, mlx.logo_png);
	mlx_destroy_image(mlx.mlx, mlx.logo_jpg);
	mlx_destroy_image(mlx.mlx, mlx.logo_bmp);
	mlx_destroy_image(mlx.mlx, mlx.img);
	mlx_destroy_window(mlx.mlx, mlx.win);
	
	mlx_destroy_context(mlx.mlx);

	return 0;
}
