#pragma once
#include <Utils/CallOnExit.h>

namespace mlx
{
	template<typename F>
	CallOnExit<F>::CallOnExit(F&& functor) : m_functor(std::move(functor)) {}

	template<typename F>
	CallOnExit<F>::~CallOnExit()
	{
		if(m_functor.has_value())
			(*m_functor)();
	}

	template<typename F>
	void CallOnExit<F>::CallAndReset()
	{
		if(m_functor.has_value())
			(*m_functor)();
		m_functor.reset();
	}

	template<typename F>
	void CallOnExit<F>::Reset()
	{
		m_functor.reset();
	}
}
