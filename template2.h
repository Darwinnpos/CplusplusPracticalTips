#pragma once

#include <iostream>
// 模板的匹配问题

template<typename ...Args>
void foo(Args... args) {
	std::cout << "foo(Args... args)" << std::endl;
}

// 发现特殊版本 如果特殊版本可以匹配，就不会再去匹配上面的模板
template<typename Fn, typename... Args>
	//requires std::invocable<Fn, int>
std::enable_if_t<std::is_invocable_v<Fn, int>>
foo(Fn first,int size, Args... args) {
	
	first(size);
	std::cout << "foo(int first, Args... args)" << std::endl;
}

// 要求为左值 不可为右值
template <typename... Args>
	//requires (std::is_lvalue_reference_v<Args> && ...) 仅提供C++17
void foo(Args&... args)
{


}
