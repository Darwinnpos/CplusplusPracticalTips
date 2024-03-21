#pragma once

#include <algorithm>
#include <iostream>
#include <vector>
#include <type_traits> // for std::enable_if

// Ϊ vector �ṩ << ������
template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
	std::for_each(v.begin(), v.end(), [&](const T& it) { os << it; });
	return os;
}

// | �ܵ�����������
class a
{
public:

};

#ifdef _HAS_CXX20

template <typename Fn>
	requires std::invocable<Fn, a&>
a& operator|(a& obj, const Fn& fn)
{
	fn(obj);
	return obj;
}

#else
template <typename Fn>
std::enable_if_t<std::is_invocable_v<Fn, A&>, A&>
operator|(A& obj, const Fn& fn)
{
	fn(obj);
	return obj;
}
#endif