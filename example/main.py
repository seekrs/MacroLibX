import macrolibpy as mlpy
import pathlib
import os
import math

current_path = pathlib.Path(__file__).parent.resolve()

mlx = mlpy.Context.create()
win = mlx.new_window(400, 400, "My window")
mlx.set_fps_goal(60)
logo_png = mlx.new_image_from_file(os.path.join(current_path, "42_logo.png"))[0]
logo_bmp = mlx.new_image_from_file(os.path.join(current_path, "42_logo.bmp"))[0]
logo_jpg = mlx.new_image_from_file(os.path.join(current_path, "42_logo.jpg"))[0]

win.pixel_put(200, 10, 0xFF0FFFF)
win.put_image(logo_png, 0, 0)

mlx.set_font_scale(os.path.join(current_path, "font.ttf"), 16.0)
win.string_put(20, 20, 0x0020FFFF, "that text will disappear")

custom_img = mlx.new_image(100, 100)
i, j, k = 0, 0, 0
while i < (100 * 100) * 4:
    if j >= 100:
        j = 0
        k += 1
    if i < 10000 or i > 20000:
        custom_img.set_pixel(j, k, (k << 24) | (j << 16) | (i << 8) | 0x99)
    i += 4
    j += 1

def onevent(ev):
    if ev == 0:
        mlx.loop_end()

THRESHOLD = 200
CIRCLE_RADIUS = 50
CIRCLE_DIAMETER = CIRCLE_RADIUS + CIRCLE_RADIUS
def onupdate():
    if onupdate.i > THRESHOLD:
        win.clear(0x334D4DFF)
        win.put_transformed_image(logo_bmp, 220, 40, 0.5, 0.5, onupdate.i)

    if onupdate.i >= THRESHOLD + THRESHOLD / 4:
        mlx.set_font_scale("default", 16.0)
    else:
        mlx.set_font_scale("default", 6.0)

    win.string_put(160, 120, 0xFF2066FF, "this text should be behind")
    win.put_image(logo_png, 100, 100)
    win.put_image(custom_img, 150, 60)

    mlx.set_font("default")
    win.string_put(20, 50, 0xFFFFFFFF, "that's a text")

    color = 0
    for j in range(0, 400):
        win.pixel_put(j, j, 0x0000FFFF + (color << 24))
        win.pixel_put(399 - j, j, 0x0000FFFF)
        color += (color < 255)

    if onupdate.i < THRESHOLD:
        win.put_transformed_image(logo_jpg, 210, 150, 0.5, 2.0, 0.0)
    else:
        win.put_transformed_image(logo_jpg, 210, 150, abs(math.sin(onupdate.i / 100.0)), abs(math.cos(onupdate.i / 100.0) * 2.0), 0.0)

    mlx.set_font_scale("default", 8.0)
    win.string_put(210, 175, 0xFFAF2BFF, "hidden")

    win.pixel_put_region(200, 170, CIRCLE_DIAMETER, CIRCLE_DIAMETER, onupdate.pixels_circle)

    onupdate.i += 1
onupdate.i = 0
onupdate.pixels_circle = [pixel for pixel in range(0, CIRCLE_DIAMETER * CIRCLE_DIAMETER)]

i = 0
for j in range(0, CIRCLE_DIAMETER):
    for k in range(0, CIRCLE_DIAMETER):
        if((CIRCLE_RADIUS - j) * (CIRCLE_RADIUS - j) + (CIRCLE_RADIUS - k) * (CIRCLE_RADIUS - k) < CIRCLE_RADIUS * CIRCLE_RADIUS):
            onupdate.pixels_circle[i] = 0xA10000FF + ((j * k * i) << 8)
        i += 1

win.on_event(mlpy.EventType.WINDOW_EVENT, onevent)
mlx.add_loop_hook(onupdate)
mlx.loop()
