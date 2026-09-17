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

        /* MLX extended types */


/**
 * @brief           Opaque handle that represents a MLX audio channel
 */
MLX_DEFINE_HANDLE(mlx_channel);

/**
 * @brief           Opaque handle that represents a MLX sound
 */
MLX_DEFINE_HANDLE(mlx_sound);


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
    MLX_CONTROLLER_AXIS_LEFTX        = 0,
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

        /* Sound related functions */

/**
* @brief            Creates a new audio channel
*
* @param mlx        Internal MLX application
*
* @return (mlx_channel) An opaque handler to the internal MLX audio channel or MLX_NULL_HANDLE (0x0) in case of error
*/
MLX_API mlx_channel mlx_new_audio_channel(mlx_context mlx);

/**
* @brief            Pauses the sound playing in the given audio channel
*
* @param mlx        Internal MLX application
* @param channel    Internal audio channel
*/
MLX_API void mlx_pause_channel(mlx_context mlx, mlx_channel channel);

/**
* @brief            Resumes the sound paused in the given audio channel
*
* @param mlx        Internal MLX application
* @param channel    Internal audio channel
*/
MLX_API void mlx_resume_channel(mlx_context mlx, mlx_channel channel);

/**
* @brief            Get the playback position of the sount in the given audio channel
*
* @param mlx        Internal MLX application
* @param channel    Internal audio channel
*
* @return (float) The time in seconds from the beginning of the sound in the channel
*/
MLX_API float mlx_get_channel_playback_position(mlx_context mlx, mlx_channel channel);

/**
* @brief            Sets the volume of an audio channel
*
* @param mlx        Internal MLX application
* @param channel    Internal audio channel
* @param left       Volume at the left ear (1 = default)
* @param right      Volume at the right ear (1 = default)
*/
MLX_API void mlx_set_channel_volume(mlx_context mlx, mlx_channel channel, float left, float right);

/**
* @brief            Sets the playback speed of an audio channel
*
* @param mlx        Internal MLX application
* @param channel    Internal audio channel
* @param speed      Speed ratio of the channel, this will also affect the sound's pitch
*/
MLX_API void mlx_set_channel_speed(mlx_context mlx, mlx_channel channel, float speed);

/**
* @brief            Destroys internal audio channel
*
* @param mlx        Internal MLX application
* @param channel    Internal audio channel
*/
MLX_API void mlx_destroy_audio_channel(mlx_context mlx, mlx_channel channel);

/**
* @brief            Creates a new sound from a wav file
*
* @param mlx        Internal MLX application
* @param path       Path to the wav file
* @param duration   Set to the duration of the sound in seconds
*
* @return (mlx_sound) An opaque handler to the internal MLX sound or MLX_NULL_HANDLE (0x0) in case of error
*/
MLX_API mlx_sound mlx_new_sound_from_wav(mlx_context mlx, char* path, float* duration);

/**
* @brief            Loads and plays a sound in an audio channel
*
* @param mlx        Internal MLX application
* @param channel    Internal audio channel
* @param sound      Internal sound handle
*/
MLX_API void mlx_play_sound(mlx_context mlx, mlx_channel channel, mlx_sound sound);

/**
* @brief            Loads and plays a sound in an audio channel
*
* @param mlx        Internal MLX application
* @param channel    Internal audio channel
* @param sound      Internal sound handle
* @param start      Seconds from which to start playing the sound
* @param end        Seconds from which to stop playing the sound
* @param loop       Should the sound start again after ending
*
* Negative time values (including -0.0) will be set relative to the end of the sound
*/
MLX_API void mlx_play_sound_ex(mlx_context mlx, mlx_channel channel, mlx_sound sound, float start, float end, bool loop);

/**
* @brief            Destroys internal sound handle
*
* @param mlx        Internal MLX application
* @param sound      Internal sound handle
*/
MLX_API void mlx_destroy_sound(mlx_context mlx, mlx_sound sound);

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
