#pragma once
#include <iostream>


// ������Ƭ����


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

