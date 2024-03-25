/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 17:55:21 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/25 16:16:07 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../includes/mlx.h"

typedef struct
{
	void* mlx;
	void* win;
	void* logo_png;
	void* logo_jpg;
	void* logo_bmp;
	void* img;
} mlx_t;

//void* img = NULL;

int update(void* param)
{
	static int i = 0;
	mlx_t* mlx = (mlx_t*)param;

	if(i == 200)
		mlx_clear_window(mlx->mlx, mlx->win);
/*
	if(img)
		mlx_destroy_image(mlx->mlx,img);
	img = mlx_new_image(mlx->mlx, 800, 800);
	mlx_set_image_pixel(mlx->mlx, img, 4, 4, 0xFF00FF00);
	mlx_put_image_to_window(mlx->mlx, mlx->win, img, 0, 0);
*/
	if(i >= 250)
		mlx_set_font_scale(mlx->mlx, mlx->win, "default", 16.f);
	else
		mlx_set_font_scale(mlx->mlx, mlx->win, "default", 6.f);
	mlx_string_put(mlx->mlx, mlx->win, 160, 120, 0xFFFF2066, "this text should be hidden");

	mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->logo_png, 100, 100);
	mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->logo_jpg, 210, 150);
	mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->logo_bmp, 220, 40);
	mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->img, 150, 60);

	mlx_set_font(mlx->mlx, mlx->win, "default");
	mlx_string_put(mlx->mlx, mlx->win, 20, 50, 0xFFFFFFFF, "that's a text");

	int color = 0;
	for(int j = 0; j < 400; j++)
	{
		mlx_pixel_put(mlx->mlx, mlx->win, j, j, 0xFFFF0000 + color);
		mlx_pixel_put(mlx->mlx, mlx->win, 399 - j, j, 0xFF0000FF);
		color += (color < 255);
	}

	i++;
	return 0;
}

void* create_image(mlx_t* mlx)
{
	unsigned char pixel[4];
	void* img = mlx_new_image(mlx->mlx, 100, 100);
	for(int i = 0, j = 0, k = 0; i < (100 * 100) * 4; i += 4, j++)
	{
		if(j >= 100)
		{
			j = 0;
			k++;
		}
		if(i < 10000 || i > 20000)
		{
			pixel[0] = i;
			pixel[1] = j;
			pixel[2] = k;
			pixel[3] = 0x99;
			mlx_set_image_pixel(mlx->mlx, img, j, k, *((int *)pixel));
		}
	}
	return img;
}

int key_hook(int key, void* param)
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
			mlx_mouse_show();
		break;
		case 11 : // (H)ide
			mlx_mouse_hide();
		break;
		case 6 : // (C)lear
			mlx_clear_window(mlx->mlx, mlx->win);
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
	int w;
	int h;
	int dummy;

	mlx.mlx = mlx_init();
	mlx.win = mlx_new_window(mlx.mlx, 400, 400, "My window");

	mlx_set_fps_goal(mlx.mlx, 60);

	mlx_on_event(mlx.mlx, mlx.win, MLX_KEYDOWN, key_hook, &mlx);
	mlx_on_event(mlx.mlx, mlx.win, MLX_WINDOW_EVENT, window_hook, &mlx);

	mlx.logo_png = mlx_png_file_to_image(mlx.mlx, "42_logo.png", &dummy, &dummy);
	mlx.logo_bmp = mlx_bmp_file_to_image(mlx.mlx, "42_logo.bmp", &dummy, &dummy);
	mlx.logo_jpg = mlx_jpg_file_to_image(mlx.mlx, "42_logo.jpg", &dummy, &dummy);

	mlx_pixel_put(mlx.mlx, mlx.win, 200, 10, 0xFFFF00FF);
	mlx_put_image_to_window(mlx.mlx, mlx.win, mlx.logo_png, 10, 190);

	mlx.img = create_image(&mlx);


	mlx_string_put(mlx.mlx, mlx.win, 0, 10, 0xFFFFFF00, "fps:");
	mlx_string_put(mlx.mlx, mlx.win, 0, 20, 0xFFFFFFFF, "fps:");

	mlx_set_font_scale(mlx.mlx, mlx.win, "font.ttf", 16.f);
	mlx_string_put(mlx.mlx, mlx.win, 20, 20, 0xFF0020FF, "that text will disappear");

	mlx_loop_hook(mlx.mlx, update, &mlx);
	mlx_loop(mlx.mlx);

	mlx_get_screens_size(mlx.mlx, mlx.win, &w, &h);
	printf("screen size : %dx%d\n", w, h);

	mlx_destroy_image(mlx.mlx, mlx.logo_png);
	mlx_destroy_image(mlx.mlx, mlx.logo_jpg);
	mlx_destroy_image(mlx.mlx, mlx.logo_bmp);
	mlx_destroy_image(mlx.mlx, mlx.img);
	mlx_destroy_window(mlx.mlx, mlx.win);
	mlx_destroy_display(mlx.mlx);
	
	return 0;
}
