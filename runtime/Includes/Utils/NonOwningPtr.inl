#pragma once
#include <Utils/NonOwningPtr.h>

namespace mlx
{
	template<typename T>
	NonOwningPtr<T>::NonOwningPtr(T* ptr) : p_ptr(ptr) {}

	template<typename T>
	NonOwningPtr<T>::NonOwningPtr(NonOwningPtr<T>&& ptr) noexcept : p_ptr(ptr.p_ptr)
	{
		ptr.p_ptr = nullptr;
	}

	template<typename T>
	NonOwningPtr<T>& NonOwningPtr<T>::operator=(T* ptr)
	{
		p_ptr = ptr;
		return *this;
	}

	template<typename T>
	NonOwningPtr<T>& NonOwningPtr<T>::operator=(NonOwningPtr&& ptr) noexcept
	{
		p_ptr = ptr.p_ptr;
		ptr.p_ptr = nullptr;
		return *this;
	}

	template<typename T>
	NonOwningPtr<T>::operator bool() const noexcept
	{
		return p_ptr != nullptr;
	}

	template<typename T>
	T* NonOwningPtr<T>::Get() const noexcept
	{
		return p_ptr;
	}

	template<typename T>
	T* NonOwningPtr<T>::operator->() const noexcept
	{
		return p_ptr;
	}

	template<typename T>
	T& NonOwningPtr<T>::operator*() const noexcept
	{
		return *p_ptr;
	}
}
