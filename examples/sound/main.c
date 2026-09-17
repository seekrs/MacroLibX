
#include <math.h>
#include <stdio.h>
#include <stddef.h>

#include "../../includes/mlx.h"
#include "../../includes/mlx_extended.h"
#include "../../includes/mlx_keycodes.h"

mlx_context mlx;
mlx_window win;

mlx_channel music_ch;
mlx_channel fx_ch;
mlx_sound music;
mlx_sound fx;

mlx_image slider_img;
mlx_image button_img;
mlx_image button_hover_img;

float music_dur;
float fx_dur;

bool mouse = false;
bool scroll = false;

int slider_hover = -1;
int slider_select = -1;
float sliders[7] = {};
char* labels[8] = {
	"Music Volume L", "Music Volume R", "Music Speed",
	"Fx Volume", "Fx Speed", "Fx Start", "Fx End",
	"[Space] Play FX | [N|M] Pause Music | [Z|X] Pause FX"
};

float slider_values[7] = {1.0, 1.0, 1.0, 0.5, 1.0, 0.0, 1.0};

void window_event(int event, void *data)
{
	(void)data;
	if (event == MLX_WINDOW_CLOSE)
		mlx_loop_end(mlx);
}

void key_down(int key, void *data)
{
	(void)data;
	if (key == MLX_KEY_ESCAPE)
		mlx_loop_end(mlx);
	else if (key == MLX_KEY_SPACE)
		mlx_play_sound_ex(mlx, fx_ch, fx, slider_values[5], slider_values[6], false);
	else if (key == MLX_KEY_Z)
		mlx_pause_channel(mlx, fx_ch);
	else if (key == MLX_KEY_X)
		mlx_resume_channel(mlx, fx_ch);
	else if (key == MLX_KEY_N)
		mlx_pause_channel(mlx, music_ch);
	else if (key == MLX_KEY_M)
		mlx_resume_channel(mlx, music_ch);
}

void mouse_down(int button, void *data)
{
	(void)data;
	if (button == MLX_MOUSE_LEFT) {
		mouse = true;
		slider_select = slider_hover;
	}
}

void mouse_up(int button, void *data)
{
	(void)data;
	if (button == MLX_MOUSE_LEFT) {
		mouse = false;
		slider_select = -1;
	}
}

void mouse_scroll(int dir, void *data)
{
	(void)data;
	if (slider_hover == -1)
		return;
	sliders[slider_hover] += (dir == MLX_MOUSE_SCROLL_UP) ? 0.02: -0.02;
	scroll = true;
}

void update_mouse()
{
	int x, y;
	mlx_mouse_get_pos(mlx, &x, &y);
	slider_hover = -1;

	if (x < 220 || x > 620)
		return;

	int slider = y * 8 / 360;
	slider_hover = slider;

	if (mouse && slider_hover == slider_select)
		sliders[slider] = (x - 220) / 400.0;
	else if (!scroll)
		return;
	scroll = false;

	switch (slider) {
		case 0:
		case 1:
			slider_values[slider] = sliders[slider] * 2;
			mlx_set_channel_volume(mlx, music_ch, slider_values[0], slider_values[1]);
			break;
		case 2:
			slider_values[slider] = sliders[slider] * 2;
			mlx_set_channel_speed(mlx, music_ch, slider_values[2]);
			break;
		case 3:
			slider_values[slider] = sliders[slider] * 2;
			mlx_set_channel_volume(mlx, fx_ch, slider_values[3], slider_values[3]);
			break;
		case 4:
			slider_values[slider] = sliders[slider] * 2;
			mlx_set_channel_speed(mlx, fx_ch, slider_values[4]);
			break;
		case 5:
		case 6:
			slider_values[slider] = sliders[slider] * fx_dur;
			break;
	}
}

void fix_sliders()
{
	sliders[0] = slider_values[0] / 2;
	sliders[1] = slider_values[1] / 2;
	sliders[2] = slider_values[2] / 2;
	sliders[3] = slider_values[3] / 2;
	sliders[4] = slider_values[4] / 2;
	sliders[5] = slider_values[5] / fx_dur;
	sliders[6] = slider_values[6] / fx_dur;
	mlx_set_channel_volume(mlx, music_ch, slider_values[0], slider_values[1]);
	mlx_set_channel_speed(mlx, music_ch, slider_values[2]);
	mlx_set_channel_volume(mlx, fx_ch, slider_values[3], slider_values[3]);
	mlx_set_channel_speed(mlx, fx_ch, slider_values[4]);
}

void update(void *data)
{
	(void)data;
	mlx_clear_window(mlx, win, (mlx_color){.rgba = 0x080811FF});

	update_mouse();

	for (int i = 0; i < 7; i++)
	{
		mlx_image btn = (i == slider_hover) ? button_hover_img : button_img;

		mlx_put_transformed_image_to_window(mlx, win, slider_img,
			220, 20 + i * 40, 400, 16, 0);
		mlx_put_transformed_image_to_window(mlx, win, btn,
			sliders[i] * 400 + 210, 18 + i * 40, 20, 20, 0);

		char val[32];

		snprintf(val, 32, "%s [%.2f]", labels[i], slider_values[i]);
		mlx_string_put(mlx, win, 20, 30 + i * 40, (mlx_color){.rgba = 0xEEEEEEFF}, val);
	}
	mlx_string_put(mlx, win, 20, 310, (mlx_color){.rgba = 0xEEEEEEFF}, labels[7]);
}

int main(void)
{
	mlx = mlx_init();

	mlx_set_fps_goal(mlx, 60);

	mlx_window_create_info info = {.title = "Sound Test", .width = 640, .height = 360};
	win = mlx_new_window(mlx, &info);
	music_ch = mlx_new_audio_channel(mlx);
	fx_ch = mlx_new_audio_channel(mlx);
	music = mlx_new_sound_from_wav(mlx, "sounds/music.wav",  &music_dur);
	fx = mlx_new_sound_from_wav(mlx, "sounds/fx.wav", &fx_dur);
	slider_values[6] = fx_dur;

	slider_img = mlx_new_image(mlx, 1, 1);
	button_img = mlx_new_image(mlx, 1, 1);
	button_hover_img = mlx_new_image(mlx, 1, 1);
	mlx_clear_image(mlx, slider_img, (mlx_color){.rgba = 0x444466FF});
	mlx_clear_image(mlx, button_img, (mlx_color){.rgba = 0x555566FF});
	mlx_clear_image(mlx, button_hover_img, (mlx_color){.rgba = 0x6666FFFF});

	mlx_on_event(mlx, win, MLX_WINDOW_EVENT, window_event, NULL);
	mlx_on_event(mlx, win, MLX_KEYDOWN, key_down, NULL);
	mlx_on_event(mlx, win, MLX_MOUSEDOWN, mouse_down, NULL);
	mlx_on_event(mlx, win, MLX_MOUSEUP, mouse_up, NULL);
	mlx_on_event(mlx, win, MLX_MOUSEWHEEL, mouse_scroll, NULL);

	mlx_play_sound_ex(mlx, music_ch, music, 0.0, -0.0, true);

	fix_sliders();
	mlx_add_loop_hook(mlx, update, NULL);
	mlx_loop(mlx);

	mlx_destroy_image(mlx, slider_img);
	mlx_destroy_image(mlx, button_img);
	mlx_destroy_image(mlx, button_hover_img);
	mlx_destroy_sound(mlx, music);
	mlx_destroy_sound(mlx, fx);
	mlx_destroy_audio_channel(mlx, music_ch);
	mlx_destroy_audio_channel(mlx, fx_ch);
	mlx_destroy_window(mlx, win);
	mlx_destroy_context(mlx);
}
