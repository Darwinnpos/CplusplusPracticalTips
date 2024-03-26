#pragma once

#include <functional>
#include <iomanip>
#include <iostream>
#include <variant>


template <class... Ts>
struct overloaded : Ts...
{
	using Ts::operator()...;
};

template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;


class A
{
public:
	template <typename... Args>
	static void Call(Args&&... args)
	{
		((std::cout << args), ...) << std::endl;
		std::cout << "A is call" << std::endl;
	}
};

class B
{
public:
	template <typename... Args>
	static void Call(Args&&... args)
	{
		((std::cout << args), ...) << std::endl;
		std::cout << "B is call" << std::endl;
	}
};

class C
{
public:
	template <typename... Args>
	static void Call(Args&&... args)
	{
		((std::cout << args), ...) << std::endl;
		std::cout << "C is call" << std::endl;
	}
};


template<typename... Args>
void Call(Args&&... args)
{
	std::variant<A, B, C> var;
	var.emplace<C>();
	std::visit( [&](auto arg) { decltype(arg)::Call(std::forward<Args>(args)...); }, var);
}

class Variant
{
public:
	static void test()
	{
		{
			std::variant<double, bool, std::string> var;
			std::visit(overloaded{
				           [](auto arg) { std::cout << arg << ' '; },
				           [](double arg) { std::cout << std::fixed << arg << ' '; },
				           [](const std::string& arg) { std::cout << std::quoted(arg) << ' '; },
			           }, var);
		}
		{
			std::variant<double, bool, std::string> var;

			struct
			{
				void operator()(int) const { std::cout << "int!\n"; }
				void operator()(std::string const&) const { std::cout << "string!\n"; }
			} visitor;

			std::visit(visitor, var);
		}
		{
			Call(1, 2, 3, 4, 5);
		}
	}
};
