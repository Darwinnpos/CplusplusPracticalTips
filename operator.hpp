#pragma once

#include <boost/range.hpp>
#include <boost/range/algorithm.hpp>
#include <algorithm>
#include <iostream>
#include <vector>
#include <type_traits> // for std::enable_if

// 为 vector 提供 << 流插入
template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{

	std::ranges::for_each(v, [&](const T& it) { os << it; }); //C++20
	boost::range::for_each(v, [&](const T& it) { os << it; }); // boost range
	std::for_each(v.begin(), v.end(), [&](const T& it) { os << it; }); //C++11
	return os;
}

// | 管道操作符重载
class a{};

template <typename Fn> //C++20
	requires std::invocable<Fn, a&>
a& operator|(a& obj, const Fn& fn)
{
	fn(obj);
	return obj;
}

// C++11
template <typename Fn>
std::enable_if_t<std::is_invocable_v<Fn, a&>, a&>
operator|(a& obj, const Fn& fn)
{
	fn(obj);
	return obj;
}

// 将类a也进行泛化， 所有可调用对象均可以使用 | 操作进行
template <typename T,typename Fn>
std::enable_if_t<std::is_invocable_v<Fn, T&>, T&>
operator|(T& obj, const Fn& fn)
{
	fn(obj);
	return obj;
}

// 借助boost使枚举可以被打印且符合 SFINAE
#include <boost/describe.hpp>

enum class e
{
	a,
	b,
	c
};

BOOST_DESCRIBE_ENUM(e, a, b, c)

// 可能会对其他代码造成冲突
inline std::ostream& operator<<(std::ostream& os, e e)
{
	os << boost::describe::enum_to_string(e,{});
	return os;
}

// SFINAE
template<typename E> //C++20
	requires std::is_enum_v<E>
std::ostream& operator<<(std::ostream& os, E e)
{
	os << boost::describe::enum_to_string(e, {});
	return os;
}

// C++11 
template<typename E>
	std::enable_if_t<std::is_enum_v<E>>
operator<<(std::ostream& os, E e)
{
	os << boost::describe::enum_to_string(e, {});
	return os;
}

