/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 16:56:35 by maldavid          #+#    #+#             */
/*   Updated: 2022/10/05 18:42:22 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MICRO_LIB_X_H__
#define __MICRO_LIB_X_H__

#ifdef __cplusplus
extern "C" {
#endif

void mlx_init();
void mlx_new_window(int w, int h, const char* title);
int mlx_loop();

int mlx_mouse_show();
int mlx_mouse_hide();
int mlx_mouse_move(void* win_ptr, int x, int y);
int mlx_mouse_get_pos(void* win_ptr, int* x, int* y);

#ifdef __cplusplus
}
#endif

#endif
