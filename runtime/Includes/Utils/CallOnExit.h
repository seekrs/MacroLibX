#ifndef __MLX_CALL_ON_EXIT__
#define __MLX_CALL_ON_EXIT__

namespace mlx
{
	template <typename F>
	class CallOnExit
	{
		public:
			CallOnExit() = default;
			CallOnExit(F&& functor);
			CallOnExit(const CallOnExit&) = delete;
			CallOnExit(CallOnExit&&) = delete;

			void CallAndReset();
			void Reset();

			CallOnExit& operator=(const CallOnExit&) = delete;
			CallOnExit& operator=(CallOnExit&&) = default;

			~CallOnExit();

		private:
			std::optional<F> m_functor;
	};

	template<typename F>
	CallOnExit(F) -> CallOnExit<F>;
}

#include <Utils/CallOnExit.inl>

#endif
