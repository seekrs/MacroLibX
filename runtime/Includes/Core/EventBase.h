#ifndef __MLX_BASE_EVENT__
#define __MLX_BASE_EVENT__

namespace mlx
{
	struct EventBase
	{
		virtual Event What() const = 0;
	};
}

#endif
