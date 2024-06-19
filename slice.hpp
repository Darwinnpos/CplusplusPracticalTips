#pragma once
#include <iostream>


// 对象切片问题


class base
{
public:
	virtual ~base() = default;

	virtual void test()
	{
		std::cout << "base call\n";
	}
};


class derived final :public base
{
public:
	void test() override
	{
		std::cout << "derived call " << a << "\n";
	}
	int a{9};
};

