
#include <iostream>
#include <utility>
#include <functional>

#include <type_traits>
#include <utility>
#include <tuple>
#include <utility>
#include <type_traits>
#include <concepts>


template<typename T>
struct input{ T& t; };

template<typename T>
struct output{ T& t; };

template<template<typename> class U, typename T>
struct is_instance_of : std::false_type {};

template<template<typename> class U, typename T>
struct is_instance_of<U, U<T>> : std::true_type {};

template<typename T>
concept wrapper = is_instance_of<input, T>::value || is_instance_of<output, T>::value;

template<typename Tuple>
struct tuples;

template<typename... Args> //TODO
struct tuples<std::tuple<Args...>>  { };

template<typename Tuple>
concept tuple_wrapper = tuples<Tuple>::value;

template <class... _Types>
auto inputs(_Types&&... _Args) { return std::make_tuple(input(_Args)...); }

template<typename T>
void print_element(const T& t)
{
	std::cout << typeid(T).name() << ": " << t;
}

template<typename T>
void print_element(const input<T>& in)
{
	std::cout <<  in.t;
}

template<typename T>
void print_element(const output<T>& out)
{
	std::cout <<  out.t;
}



class stream_object
{
public:
	stream_object(size_t) {
		// malloc the memory 

	};
};


class frame
{
public:
	
};

template<typename... Ts>
void deserialize(std::tuple<Ts...>& theTuple)
{
	std::cout << "deserialize:";
	std::apply([](const Ts&... args) {
		std::size_t n = 0;
		((std::cout << (n++ ? ", " : "") << typeid(args).name() << ": ", print_element(args)), ...);
		}, theTuple);
	std::cout << std::endl;
}


template<typename... Ts>
void serialize(std::tuple<Ts...>& theTuple)
{
	// try malloc the shared memory

	std::cout << "serialize:";
	std::apply([](const Ts&... args) {
		std::size_t n = 0;
		((std::cout << (n++ ? ", " : "") << typeid(args).name() << ": ", print_element(args)), ...);
		}, theTuple);
	std::cout << std::endl;
}

using execute = std::function<void()>;

// 参数执行器
class tls_storer
{
public :
	inline static thread_local execute serializer;
	inline static thread_local execute deserializer;
};


// 参数收集器
template<typename... Outputs, typename... Inputs>
void collect(std::tuple<Inputs...>& inputs, std::tuple<Outputs...>& outputs)
{	
	tls_storer::serializer = [=] () mutable { serialize(inputs); };
	tls_storer::deserializer = [=]() mutable { deserialize(outputs); };
}	

// 参数收集器
template<typename... Outputs, typename... Inputs,typename First, typename... Rest>
void collect(std::tuple<Inputs...>& inputs, std::tuple<Outputs...>& outputs, First&& first, Rest&&... rest)
{	
	if constexpr  (std::is_same_v<std::decay_t<First>, input<std::decay_t<decltype(first.t)>>>)
	{	
		auto inputs2 = std::tuple_cat(inputs, std::make_tuple(std::forward<First>(first)));
		collect( inputs2, outputs, std::forward<Rest>(rest)...);
	}	
	else if constexpr (std::is_same_v<std::decay_t<First>, output<std::decay_t<decltype(first.t)>>>)
	{		
		auto outputs2 = std::tuple_cat(outputs, std::make_tuple(std::forward<First>(first)));
		collect( inputs, outputs2, std::forward<Rest>(rest)...);
	}
}	

void wait()
{

}

// 主体逻辑执行器
template<wrapper... Args>
void rpc(Args&&... args)
{		
	std::tuple<> input;
	std::tuple<> output;	
	collect(input, output, std::forward<Args>(args)...);
	tls_storer::serializer();
	wait();
	tls_storer::deserializer();
}

template<tuple_wrapper... Args>
void rpc(Args&&... args)
{
	//std::tuple<> input;
	//std::tuple<> output;
	//collect(input, output, std::forward<Args>(args)...);
	//serialize();
	//deserialize();
}

int main() 
{	
	int a{ 1 };
	double b{ 2 };
	float c{ 3 };
	bool d{ false };
	
	rpc(input(a), input(b), output(c), output(d));

	//rpc(inputs(a, b), output(c), output(d));

	return 0;
}
