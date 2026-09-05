/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_extended.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <contact@kbz8.me>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 16:17:10 by maldavid          #+#    #+#             */
/*   Updated: 2025/10/22 16:59:38 by maldavid         ###   ########.fr       */
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


        /* Events related functions */

/**
* @brief           Struct representing the actual data stored in the code passed to controller event handlers
*/
typedef struct mlx_controller_event_code
{
	#if MLX_BYTEORDER == MLX_LITTLE_ENDIAN
		short button;
		short controller_id;
	#else
		short controller_id;
		short button;
	#endif
} mlx_controller_event_code;

/**
* @brief           Codes for polling analog controller inputs
*/
typedef enum mlx_controller_axis
{
   MLX_CONTROLLER_AXIS_LEFTX         = 0,
    MLX_CONTROLLER_AXIS_LEFTY        = 1,
    MLX_CONTROLLER_AXIS_RIGHTX       = 2,
    MLX_CONTROLLER_AXIS_RIGHTY       = 3,
    MLX_CONTROLLER_AXIS_TRIGGERLEFT  = 4,
    MLX_CONTROLLER_AXIS_TRIGGERRIGHT = 5,
} mlx_controller_axis;

/**
* @brief            Get a controller's analog input
*
* @param mlx        Internal MLX application
* @param id         Controller ID (-1 for default)
* @param axis       Axis type (see enum 'mlx_controller_axis')
*
* @return (float)   The axis value (ranges [-1, 1] for joysticks, [0, 1] for triggers)
*/
MLX_API float mlx_controller_get_axis(mlx_context mlx, int id, int axis);

/**
* @brief            Rumble a controller
*
* @param mlx        Internal MLX application
* @param id         Controller ID (-1 for default)
* @param high_freq  High frequency intensity (ranges [0, 1])
* @param low_freq   Low frequency intensity (ranges [0, 1])
* @param duration   Duration of the rumble in seconds
*
*/
MLX_API void mlx_controller_rumble(mlx_context mlx, int controller_id, float high_freq, float low_freq, float duration);

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
 *
 * Note: it is responsability of the user to make sure the size of `pixels` is
 * big enough for the given array.
 * WARNING: This function isn't performant, consider drawing to an image beforehand.
 */
MLX_DEPRECATED("Use an image rather than directly communicating with the window")
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
 * WARNING: This function isn't performant, consider drawing to an image beforehand.
 */
MLX_DEPRECATED("Use an image rather than directly communicating with the window")
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
MLX_API void mlx_put_transformed_image_to_window(mlx_context mlx, mlx_window win, mlx_image image, float x, float y, float scale_x, float scale_y, float angle);

/**
 * @brief            Get direct pointers to hidden functions
 *
 * @param mlx        Internal MLX application
 *
 * @return (mlx_function) A function pointer or NULL (0x0) in case of error
 */
MLX_API mlx_function mlx_get_proc_addr(mlx_context mlx, const char* name);

#ifdef __cplusplus
}
#endif

#endif
