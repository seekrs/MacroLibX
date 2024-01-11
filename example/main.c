/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 17:55:21 by maldavid          #+#    #+#             */
/*   Updated: 2024/01/11 15:03:14 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include "../includes/mlx.h"

typedef struct s_mlx
{
	void	*mlx;
	void	*win;
	void	*logo;
	void	*img;
}	t_mlx;

int	update(void *param)
{
	static int	i = 0;
	int			j;
	int			k;
	t_mlx		*mlx;

	mlx = (t_mlx *)param;
	mlx_string_put(mlx->mlx, mlx->win, 160, 120, 0xFFFF2066, "this text should be hidden");
	mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->logo, 100, 100);
	mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->img, 150, 60);
	if (i == 0)
		mlx_set_font_scale(mlx->mlx, mlx->win, "font.ttf", 16.f);
	mlx_string_put(mlx->mlx, mlx->win, 20, 50, 0xFFFFFFFF, "that's a text");
	j = 0;
	k = 0;
	while (j++ < 400)
	{
		mlx_pixel_put(mlx->mlx, mlx->win, j, j, 0xFFFF0000 + k);
		mlx_pixel_put(mlx->mlx, mlx->win, 399 - j, j, 0xFF0000FF);
		k += (k < 255);
	}
	if (++i == 5000)
		mlx_clear_window(mlx->mlx, mlx->win);
	if (i == 7000)
		mlx_set_font_scale(mlx->mlx, mlx->win, "default", 16.f);
	return (0);
}

void	*create_image(t_mlx *mlx)
{
	unsigned char	pixel[4];
	int				i[3];
	void			*img;

	memset(i, 0, sizeof(int) * 3);
	img = mlx_new_image(mlx->mlx, 100, 100);
	while (i[0] < (100 * 100) * 4)
	{
		if (i[0] < 10000 || i[0] > 20000)
		{
			pixel[0] = i[0];
			pixel[1] = i[1];
			pixel[2] = i[2];
			pixel[3] = 0x99;
			mlx_set_image_pixel(mlx->mlx, img, i[1], i[2], *((int *)pixel));
		}
		i[0] += 4;
		i[1]++;
		if (i[1] >= 100)
		{
			i[1] = 0;
			i[2]++;
		}
	}
	return (img);
}

int	key_hook(int key, void *param)
{
	if (key == 41)
		mlx_loop_end(((t_mlx *)param)->mlx);
	return (0);
}

int	window_hook(int event, void *param)
{
	if (event == 0)
		mlx_loop_end(((t_mlx *)param)->mlx);
	return (0);
}

int	main(int argc, char **argv)
{
	t_mlx	mlx;
	void	*img;
	int		w;
	int		h;

	(void)argc;
	(void)argv;
	mlx.mlx = mlx_init();
	mlx.win = mlx_new_window(mlx.mlx, 400, 400, "My window");
	mlx_on_event(mlx.mlx, mlx.win, MLX_KEYDOWN, key_hook, &mlx);
	mlx_on_event(mlx.mlx, mlx.win, MLX_WINDOW_EVENT, window_hook, &mlx);
	mlx.logo = mlx_png_file_to_image(mlx.mlx, "42_logo.png", &w, &h);
	mlx_pixel_put(mlx.mlx, mlx.win, 200, 10, 0xFFFF00FF);
	mlx_put_image_to_window(mlx.mlx, mlx.win, mlx.logo, 10, 190);
	mlx.img = create_image(&mlx);
	mlx_string_put(mlx.mlx, mlx.win, 20, 20, 0xFF0020FF, \
			"that text will disappear");
	mlx_loop_hook(mlx.mlx, update, &mlx);
	mlx_loop(mlx.mlx);
	mlx_destroy_image(mlx.mlx, mlx.logo);
	mlx_destroy_image(mlx.mlx, mlx.img);
	mlx_destroy_window(mlx.mlx, mlx.win);
	mlx_destroy_display(mlx.mlx);
	return (0);
}
