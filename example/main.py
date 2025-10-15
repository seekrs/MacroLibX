import macrolibpy as mlpy

if __name__ == '__main__':
    mlx = mlpy.Context.create()
    win = mlx.new_window(800, 600, "MacroLibX window")

    def onevent(ev):
        if ev == 0:
            mlx.loop_end()

    def onupdate():
        for x in range(0, 50):
            for y in range(0, 50):
                win.pixel_put(100 + x, 100 + y, 0xff0000ff)

    win.on_event(mlpy.EventType.WINDOW_EVENT, onevent)
    mlx.add_loop_hook(onupdate)
    mlx.loop()
