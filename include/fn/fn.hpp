#include <type_traits>
#include <functional>

namespace fn {

namespace cog {

/* CREDIT START */
/* snippet from @ecatmur (https://stackoverflow.com/a/12283159/11921314) */
template<typename T>
struct remove_class
{
};

template<typename C, typename R, typename... A>
struct remove_class<R(C::*)(A...)>
{
	static inline constexpr size_t count = sizeof...(A);
};
template<typename C, typename R, typename... A>
struct remove_class<R(C::*)(A...) const>
{
	static inline constexpr size_t count = sizeof...(A);
};
template<typename C, typename R, typename... A>
struct remove_class<R(C::*)(A...) volatile>
{
	static inline constexpr size_t count = sizeof...(A);
};
template<typename C, typename R, typename... A>
struct remove_class<R(C::*)(A...) const volatile>
{
	static inline constexpr size_t count = sizeof...(A);
};

template <typename T>
struct is_callable
{
	template <typename U, typename V = decltype(&U::operator())>
	static inline constexpr bool test(void)
	{
		return true;
	}

	template <typename U, typename ...Args>
	static inline constexpr bool test(Args &&...)
	{
		return false;
	}

	static inline constexpr bool value = test<T>();
};

template <typename T>
struct fun_arg
{
	static inline constexpr size_t get_count(void)
	{
		if constexpr (is_callable<T>::value)
			return remove_class<decltype(&std::remove_reference<T>::type::operator())>::count;
		else
			return 0;
	}

	static inline constexpr size_t count = get_count();
};
template <typename R, typename... A>
struct fun_arg<R(A...)>
{
	static inline constexpr size_t count = sizeof...(A);
};
template <typename R, typename... A>
struct fun_arg<R(&)(A...)>
{
	static inline constexpr size_t count = sizeof...(A);
};
template<typename R, typename... A>
struct fun_arg<R(*)(A...)>
{
	static inline constexpr size_t count = sizeof...(A);
};

/* CREDIT END @ecatmur */

template <size_t Count, typename TupleAcc, typename Fn>
auto decompose_impl(TupleAcc &&acc, Fn &&fn)
{
	//if constexpr (Count)
	return Count;
}

template <typename Fn>
auto decompose(Fn &&fn)
{
	static constexpr size_t count = fun_arg<Fn>::count;
	if constexpr (count == 0) {
		return fn;
	} else {
		return decompose_impl<count>(std::tuple<>(), std::forward<Fn>(fn));
	}
}

}

}