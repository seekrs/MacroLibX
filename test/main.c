/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 17:55:21 by maldavid          #+#    #+#             */
/*   Updated: 2023/01/25 15:20:35 by maldavid         ###   ########.fr       */
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
