/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 17:55:21 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/01 17:50:46 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../includes/mlx.h"

typedef struct
{
	void	*mlx;
	void	*win;
	void	*logo;
}	t_mlx;

int	update(t_mlx *mlx)
{
	static int	i = 0;
	int			j;

	mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->logo, 100, 100);
	j = 0;
	while (j < 400)
	{
		mlx_pixel_put(mlx->mlx, mlx->win, j, j, 0xFF0000FF);
		mlx_pixel_put(mlx->mlx, mlx->win, 399 - j, j, 0xFF0000FF);
		j++;
	}
	i++;
	if (i == 5000)
		mlx_clear_window(mlx->mlx, mlx->win);
	if (i > 10000)
		mlx_loop_end(mlx->mlx);
	return (0);
}

int	main(void)
{
	t_mlx	mlx;
	int		w;
	int		h;
	
	mlx.mlx = mlx_init();
	mlx.win = mlx_new_window(mlx.mlx, 400, 400, "My window");
	mlx.logo = mlx_png_file_to_image(mlx.mlx, "42_logo.png", &w, &h);
	mlx_pixel_put(mlx.mlx, mlx.win, 200, 10, 0xFFFF00FF);
	mlx_loop_hook(mlx.mlx, update, &mlx);
	mlx_loop(mlx.mlx);
	mlx_destroy_image(mlx.mlx, mlx.logo);
	mlx_destroy_window(mlx.mlx, mlx.win);
	mlx_destroy_display(mlx.mlx);
	return (0);
}
