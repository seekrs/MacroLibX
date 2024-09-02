#include <PreCompiled.h>

#include <Platform/Inputs.h>

namespace mlx
{
	void Input::update()
	{
		MLX_PROFILE_FUNCTION();
		_xRel = 0;
		_yRel = 0;

		glfwPollEvents();

		static int i = 0;
		i++;
		if(i >= 500)
		{
			auto& hooks = _events_hooks[0];
			auto& win_hook = hooks[MLX_WINDOW_EVENT];
			if(win_hook.hook)
				win_hook.hook(0, win_hook.param);
		}
	}
}
