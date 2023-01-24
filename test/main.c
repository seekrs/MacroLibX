/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 17:55:21 by maldavid          #+#    #+#             */
/*   Updated: 2023/01/24 17:22:54 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../includes/mlx.h"

typedef struct
{
	void	*mlx;
	void	*win;
}	t_mlx;

int	update(t_mlx *mlx)
{
	static int	i = 0;

	printf("%d\n", i);
	mlx_pixel_put(mlx->mlx, mlx->win, 12, 50, 0xFF0000);
	mlx_pixel_put(mlx->mlx, mlx->win, 12, 60, 0x00FF00);
	mlx_pixel_put(mlx->mlx, mlx->win, 12, 70, 0x0000FF);
	mlx_pixel_put(mlx->mlx, mlx->win, 12, 80, 0xFF00FF);
	mlx_pixel_put(mlx->mlx, mlx->win, 12, 90, 0xFFFF00);
	i++;
	if (i > 20000)
		mlx_loop_end(mlx->mlx);
	return (0);
}

int	main(void)
{
	t_mlx	mlx;
	
	mlx.mlx = mlx_init();
	mlx.win = mlx_new_window(mlx.mlx, 400, 400, "My window");
	mlx_loop_hook(mlx.mlx, update, &mlx);
	mlx_loop(mlx.mlx);
	mlx_destroy_window(mlx.mlx, mlx.win);
	mlx_destroy_display(mlx.mlx);
	return (0);
}
