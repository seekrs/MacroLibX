#ifndef __MLX_SINGLETON__
#define __MLX_SINGLETON__

#include <Utils/NonCopyable.h>

namespace mlx
{
	template <typename T>
	class Singleton : public NonCopyable
	{
		public:
			inline static T& Get()
			{
				static T instance;
				return instance;
			}
	};
}

#endif // __MLX_SINGLETON__
