#pragma once

#include <iostream>
// ģ���ƥ������

template<typename ...Args>
void foo(Args... args) {
	std::cout << "foo(Args... args)" << std::endl;
}

// ��������汾 �������汾����ƥ�䣬�Ͳ�����ȥƥ�������ģ��
template<typename Fn, typename... Args>
	//requires std::invocable<Fn, int>
std::enable_if_t<std::is_invocable_v<Fn, int>>
foo(Fn first,int size, Args... args) {
	
	first(size);
	std::cout << "foo(int first, Args... args)" << std::endl;
}

// Ҫ��Ϊ��ֵ ����Ϊ��ֵ
template <typename... Args>
	//requires (std::is_lvalue_reference_v<Args> && ...) ���ṩC++17
void foo(Args&... args)
{


}
