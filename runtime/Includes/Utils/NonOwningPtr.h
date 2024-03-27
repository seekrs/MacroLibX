/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NonOwningPtr.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 21:03:37 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/27 21:05:05 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_NON_OWNING_PTR__
#define __MLX_NON_OWNING_PTR__

namespace mlx
{
	template<typename T>
	class NonOwningPtr
	{
		public:
			NonOwningPtr(T* ptr = nullptr);
			NonOwningPtr(const NonOwningPtr&) = default;
			NonOwningPtr(NonOwningPtr&& ptr) noexcept;

			NonOwningPtr& operator=(T* ptr);
			NonOwningPtr& operator=(const NonOwningPtr&) = default;
			NonOwningPtr& operator=(NonOwningPtr&& ptr) noexcept;

			inline operator bool() const noexcept;

			inline T* Get() const noexcept;
			inline T* operator->() const noexcept;
			inline T& operator*() const noexcept;

			~NonOwningPtr() = default;

		private:
			T* p_ptr = nullptr;
	};
}

#include <Utils/NonOwningPtr.inl>

#endif
