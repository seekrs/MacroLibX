
#include <math.h>
#include <stdio.h>
#include <stddef.h>

#include "../../includes/mlx.h"
#include "../../includes/mlx_extended.h"
#include "../../includes/mlx_keycodes.h"

#define MAX_PLAYER_COUNT 6

typedef struct {
	float x;
	float y;
	float draw;
	float erase;
	bool connected;
	bool overdraw;
	bool overerase;
} Cursor;

mlx_context mlx;
mlx_window win;
mlx_image target, overlay;
long frame = 0;

mlx_color bg = {.rgba = 0xFFFFFFFF};
mlx_color tr = {.rgba = 0x0};

mlx_color colors[MAX_PLAYER_COUNT] = {
	{.rgba = 0x0000FFFF},
	{.rgba = 0xFF0000FF},
	{.rgba = 0x00FF00FF},
	{.rgba = 0xFFFF00FF},
	{.rgba = 0x00FFFFFF},
	{.rgba = 0xFF00FFFF},
};

Cursor cursors[MAX_PLAYER_COUNT] = {
	{.x = 160, .y = 90},
	{.x = 160, .y = 90},
	{.x = 160, .y = 90},
	{.x = 160, .y = 90},
	{.x = 160, .y = 90},
	{.x = 160, .y = 90}
};

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
}

void controller_down(mlx_controller_event_code event, void *data)
{
	(void)data;

	if (event.controller_id >= MAX_PLAYER_COUNT)
		return;

	Cursor* cursor = &cursors[event.controller_id];

	switch (event.button)
	{
		case MLX_CONTROLLER_CONNECT: cursor->connected = true; break;
		case MLX_CONTROLLER_DISCONNECT: cursor->connected = false; break;

		case MLX_CONTROLLER_A: cursor->overdraw = true; break;
		case MLX_CONTROLLER_B: cursor->overerase = true; break;
		case MLX_CONTROLLER_DPAD_UP: cursor->y -= 1; break;
		case MLX_CONTROLLER_DPAD_DOWN: cursor->y += 1; break;
		case MLX_CONTROLLER_DPAD_LEFT: cursor->x -= 1; break;
		case MLX_CONTROLLER_DPAD_RIGHT: cursor->x += 1; break;
		case MLX_CONTROLLER_START:
			mlx_save_image_to_file(mlx, target, "masterpiece.png");
			break;
	}
	//printf("Controller Down: %i:%i\n", event.controller_id, event.button);
}

void controller_up(mlx_controller_event_code event, void *data)
{
	(void)data;

	if (event.controller_id >= MAX_PLAYER_COUNT)
		return;

	Cursor* cursor = &cursors[event.controller_id];

	switch (event.button)
	{
		case MLX_CONTROLLER_A: cursor->overdraw = false; break;
		case MLX_CONTROLLER_B: cursor->overerase = false; break;
	}
	//printf("Controller Up: %i:%i\n", event.controller_id, event.button);
}

void text(int cp, void* data)
{
	(void)data;

	char *ccp = (char *)&cp;
	//printf("Text '%s' = %x\n", ccp, cp);
}

float deadzone(float f)
{
	if (f > -0.1 && f < 0.1)
		return 0.0;
	return f;
}

void update_cursor(int i)
{
	Cursor* cursor = &cursors[i];

	float dx = deadzone(mlx_controller_get_axis(mlx, i, MLX_CONTROLLER_AXIS_RIGHTX));
	float dy = deadzone(mlx_controller_get_axis(mlx, i, MLX_CONTROLLER_AXIS_RIGHTY));
	dx += 0.25 * deadzone(mlx_controller_get_axis(mlx, i, MLX_CONTROLLER_AXIS_LEFTX));
	dy += 0.25 * deadzone(mlx_controller_get_axis(mlx, i, MLX_CONTROLLER_AXIS_LEFTY));

	cursor->x += dx;
	cursor->y += dy;
	if (cursor->x < 0) cursor->x = 0;
	if (cursor->y < 0) cursor->y = 0;
	if (cursor->x >= 320) cursor->x = 320;
	if (cursor->y >= 180) cursor->y = 180;

	cursor->draw = 4 * deadzone(mlx_controller_get_axis(mlx, i, MLX_CONTROLLER_AXIS_TRIGGERRIGHT));
	cursor->erase = 8 * deadzone(mlx_controller_get_axis(mlx, i, MLX_CONTROLLER_AXIS_TRIGGERLEFT));

	if (cursor->overdraw && cursor->draw < 1) cursor->draw = 1;
	if (cursor->overerase && cursor->erase < 1) cursor->erase = 2;

	float speed = fmin(sqrt(dx * dx + dy * dy), 1.0);
	float force = fmax(cursor->draw, cursor->erase / 2);

	if ((frame % 60) == 0 && force > 1)
		mlx_controller_rumble(mlx, i,
			fmin(force * (speed + 0.5) / 20, 1),
			fmin(force * (1 - speed) / 200, 1), 0.7);
}

void update(void *data)
{
	(void)data;

	mlx_clear_window(mlx, win, bg);
	mlx_clear_image(mlx, overlay, tr);

	for (int i = 0; i < MAX_PLAYER_COUNT; i++)
	{
		Cursor* cursor = &cursors[i];
		if (!cursor->connected)
			continue;

		update_cursor(i);

		if (cursor->draw)
			mlx_set_image_rectangle(mlx, target,
				cursor->x - cursor->draw / 2, cursor->y - cursor->draw / 2,
				cursor->draw,  cursor->draw, colors[i]);
		else if (cursor->erase)
			mlx_set_image_rectangle(mlx, target,
				cursor->x - cursor->erase / 2, cursor->y - cursor->erase / 2,
				cursor->erase,  cursor->erase, tr);

		mlx_set_image_pixel(mlx, overlay, cursor->x - 0.5, cursor->y - 0.5,
			(mlx_color){.rgba = (colors[i].rgba & ~0xFF) | 0x80});
	}

	mlx_put_transformed_image_to_window(mlx, win, target, 0, 0, 4, 4, 0);
	mlx_put_transformed_image_to_window(mlx, win, overlay, 0, 0, 4, 4, 0);
	frame++;
}

int main(void)
{
	mlx = mlx_init();

	mlx_set_fps_goal(mlx, 120);

	mlx_window_create_info info = {.title = "Controller Test", .width = 1280, .height = 720};
	win = mlx_new_window(mlx, &info);

	target = mlx_new_image(mlx, 320, 180);
	overlay = mlx_new_image(mlx, 320, 180);

	mlx_on_event(mlx, win, MLX_WINDOW_EVENT, window_event, NULL);
	mlx_on_event(mlx, win, MLX_KEYDOWN, key_down, NULL);
	mlx_on_event(mlx, win, MLX_CONTROLLERDOWN, (void(*)(int, void*))controller_down, NULL);
	mlx_on_event(mlx, win, MLX_CONTROLLERUP, (void(*)(int, void*))controller_up, NULL);
	mlx_on_event(mlx, win, MLX_TEXTINPUT, (void(*)(int, void*))text, NULL);

	mlx_add_loop_hook(mlx, update, NULL);
	mlx_loop(mlx);
	mlx_destroy_image(mlx, target);
	mlx_destroy_image(mlx, overlay);
	mlx_destroy_window(mlx, win);
	mlx_destroy_context(mlx);
}
