/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_extended.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <contact@kbz8.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 16:17:10 by maldavid          #+#    #+#             */
/*   Updated: 2025/01/08 12:37:15 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// MacroLibX official repo https://github.com/seekrs/MacroLibX
// MacroLibX official website https://macrolibx.kbz8.me/

#ifndef MACROLIB_X_EXTENDED_H
#define MACROLIB_X_EXTENDED_H

#include "mlx.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif


        /* Window related functions */


/**
 * @brief            Sets maximum window size
 *
 * @param mlx        Internal MLX application
 * @param win        Internal window to move
 * @param x          New x maximum size
 * @param y          New y maximum size
 */
MLX_API void mlx_set_window_max_size(mlx_context mlx, mlx_window win, int x, int y);

/**
 * @brief            Sets minimum window size
 *
 * @param mlx        Internal MLX application
 * @param win        Internal window to move
 * @param x          New x minimum size
 * @param y          New y minimum size
 */
MLX_API void mlx_set_window_min_size(mlx_context mlx, mlx_window win, int x, int y);

/**
 * @brief            Maximizes a window
 *
 * @param mlx        Internal MLX application
 * @param win        Internal window to move
 */
MLX_API void mlx_maximise_window(mlx_context mlx, mlx_window win);

/**
 * @brief            Minimizes a window
 *
 * @param mlx        Internal MLX application
 * @param win        Internal window to move
 */
MLX_API void mlx_minimize_window(mlx_context mlx, mlx_window win);

/**
 * @brief            Restore window to formal size
 *
 * @param mlx        Internal MLX application
 * @param win        Internal window to move
 */
MLX_API void mlx_restore_window(mlx_context mlx, mlx_window win);



        /* Pixels drawing related functions */


/**
 * @brief            Put an array of pixels in the window
 *
 * @param mlx        Internal MLX application
 * @param win        Internal window
 * @param x          X coordinate
 * @param y          Y coordinate
 * @param pixels     Array of pixels
 * @param pixels_number Number of pixels
 */
MLX_API void mlx_pixel_put_array(mlx_context mlx, mlx_window win, int x, int y, mlx_color* pixels, size_t pixels_number);

/**
 * @brief            Put a region of pixels in the window
 *
 * @param mlx        Internal MLX application
 * @param win        Internal window
 * @param x          X coordinate
 * @param y          Y coordinate
 * @param w          Width
 * @param h          Height
 * @param pixels     Array of pixels
 *
 * Note: it is responsability of the user to make sure the size of `pixels` is
 * big enough for the given region.
 */
MLX_API void mlx_pixel_put_region(mlx_context mlx, mlx_window win, int x, int y, int w, int h, mlx_color* pixels);



        /* Images related functions */


/**
 * @brief            Get image region
 *
 * @param mlx        Internal MLX application
 * @param img        Internal image
 * @param x          X coordinate in the image
 * @param y          Y coordinate in the image
 * @param w          Width of the region
 * @param y          Height of the region
 * @param dst        Array of pixels to copy to
 *
 * Note: it is responsability of the user to make sure the size of `dst` is
 * big enough for the given region.
 *
 * /!\ If you run into glitches when writing or reading pixels from images /!\
 * You need to add IMAGES_OPTIMIZED=false to your make mlx command
 * ```
 *     ~ git clone https://github.com/seekrs/MacroLibX.git
 *     ~ cd MacroLibX
 *     ~ make IMAGES_OPTIMIZED=false
 * ```
 */
MLX_API void mlx_get_image_region(mlx_context mlx, mlx_image image, int x, int y, int w, int h, mlx_color* dst);

/**
 * @brief            Set image region
 *
 * @param mlx        Internal MLX application
 * @param img        Internal image
 * @param x          X coordinate in the image
 * @param y          Y coordinate in the image
 * @param w          Width of the region
 * @param y          Height of the region
 * @param pixels     Array of pixels to copy from
 *
 * Note: it is responsability of the user to make sure the size of `pixels` is
 * big enough for the given region.
 *
 * /!\ If you run into glitches when writing or reading pixels from images /!\
 * You need to add IMAGES_OPTIMIZED=false to your make mlx command
 * ```
 *     ~ git clone https://github.com/seekrs/MacroLibX.git
 *     ~ cd MacroLibX
 *     ~ make IMAGES_OPTIMIZED=false
 * ```
 */
MLX_API void mlx_set_image_region(mlx_context mlx, mlx_image image, int x, int y, int w, int h, mlx_color* pixels);

/**
 * @brief            Transform and put image to the given window
 *
 * @param mlx        Internal MLX application
 * @param win        Internal window
 * @param img        Internal image
 * @param x          X coordinate
 * @param y          Y coordinate
 * @param scale_x    Scale x of the image
 * @param scale_y    Scale y of the image
 * @param angle      Rotation angle of the image (clockwise)
 */
MLX_API void mlx_put_transformed_image_to_window(mlx_context mlx, mlx_window win, mlx_image image, int x, int y, float scale_x, float scale_y, float angle);

#ifdef __cplusplus
}
#endif

#endif
