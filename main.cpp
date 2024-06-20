
#include <iostream>
#include <utility>
#include <functional>

#include <type_traits>
#include <utility>
#include <tuple>
#include <utility>
#include <type_traits>
#include <concepts>
#include <optional>
#include <functional>

template<typename T>
struct input { T& t; };

template<typename T>
struct output { T& t; };

template<template<typename> class U, typename T>
struct is_instance_of : std::false_type {};

template<template<typename> class U, typename T>
struct is_instance_of<U, U<T>> : std::true_type {};

template<typename T>
concept wrapper = is_instance_of<input, T>::value || is_instance_of<output, T>::value;

template<typename Tuple>
struct tuples;

template<typename... Args> //TODO 
struct tuples<std::tuple<Args...>> { };

template<typename Tuple>
concept tuple_wrapper = tuples<Tuple>::value;

template <class... _Types>
auto inputs(_Types&&... _Args) { return std::make_tuple(input(_Args)...); }

template<typename T>
void print_element(const T& t)
{
	//std::cout << typeid(T).name() << ": " << t;
}

template<typename T>
void print_element(const input<T>& in)
{
	std::cout << in.t;
}

template<typename T>
void print_element(const output<T>& out)
{
	std::cout << out.t;
}


class yas
{
public:
	template<typename ...Args>
	static size_t count(Args&&...args) {

	}
	template<typename ...Args>
	static void save(char*, Args&&...args) {

	}
	template<typename ...Args>
	static void load(char*, size_t size, Args&&...arg) {

	}
};


class stream_object
{
public:
	explicit stream_object(size_t) {
		// malloc the memory 
	};
};



class malloc_chain
{
public:
	virtual malloc_chain& set_next(malloc_chain& _next) {
		next.emplace(_next);
		return *this;
	};

	virtual std::optional<char*> malloc_inner(size_t size) = 0;

	std::optional<char*> malloc(size_t size)
	{
		if (const auto opt_ptr = malloc_inner(size); opt_ptr.has_value())
			return opt_ptr;
		if (next.has_value())
			return next.value().get().malloc(size);
		return {};
	};

protected:
	std::optional<std::reference_wrapper<malloc_chain>> next;
};


class semaphore
{


};

template<typename T>
class mq
{


};

class event
{
public:
	void allocte() {}

private:
	std::vector<semaphore> semaphore_array;
	mq<size_t> index;
};


class tls_memory : public malloc_chain
{
public:
	std::optional<char*> malloc_inner(size_t size) override {
		return {};
	}
};

class managered_memory : public malloc_chain
{
public:
	std::optional<char*> malloc_inner(size_t size)override {
		return {};
	}
};

class file_transefer : public malloc_chain
{
public:
	std::optional<char*> malloc_inner(size_t size) override {
		return {};
	}
};

class shared_memory
{
public:
	shared_memory()
		:entrance{ first }
	{		
		first.set_next(second).set_next(last);
	}

	char* malloc(size_t size) {
		synchronization.allocte();
		auto opt_prt = entrance.malloc(size);
		if (opt_prt.has_value())return opt_prt.value();
		throw std::runtime_error("shared memory malloc failed.");
	}

	std::tuple<char*, size_t> find() {
		return { nullptr,0 };
	}

	static shared_memory& get() {
		if (!instance) instance = new shared_memory();
		return *instance;
	}
private:
	inline static shared_memory* instance = nullptr;
	event synchronization;
	tls_memory first;
	managered_memory second;
	file_transefer last;
	malloc_chain& entrance;
};

class seriliasize
{
	template<typename... Ts>
	static void to_parameters(std::tuple<Ts...>& theTuple)
	{
		auto [ptr, size] = shared_memory::get().find();
		yas::load(ptr, size, theTuple);

		std::cout << "deserialize:";
		std::apply([](const Ts&... args) {
			std::size_t n = 0;
			((std::cout << (n++ ? ", " : "") << typeid(args).name() << ": ", print_element(args)), ...);
			}, theTuple);
		std::cout << std::endl;
	}


	template<typename... Ts>
	static size_t count(std::tuple<Ts...>& theTuple)
	{
		return 0;
	}

	template<typename... Ts>
	static void to_string(std::tuple<Ts...>& theTuple)
	{
		// try malloc the shared memory	
		auto ptr = shared_memory::get().malloc(count(theTuple));
		yas::save(ptr, theTuple);
		std::cout << "serialize:";
		std::apply([](const Ts&... args) {
			std::size_t n = 0;
			((std::cout << (n++ ? ", " : "") << typeid(args).name() << ": ", print_element(args)), ...);
			}, theTuple);
		std::cout << std::endl;
	}
};



// 参数执行器
class tls
{
	using execute = std::function<void()>;
#define VOID_FN []{}
public:
	template<typename... Args>
	static void parameters_bind(Args&&... args)
	{
		auto [inputs, outputs] = parameters_sort(std::forward<Args>(args)...);
		writter = [=]() mutable { seriliasize::to_string(inputs); };
		reader = [=]() mutable { seriliasize::to_parameters(outputs); };
	}

	static void wait_response()
	{

	}

	static void write()
	{
		writter();
	}

	static void read()
	{
		reader();
	}

private:
	inline static thread_local execute writter = VOID_FN;
	inline static thread_local execute reader = VOID_FN;
};


auto parameters_sort()
{
	return std::tuple<std::tuple<>, std::tuple<>>();
}

// 参数收集器
template<typename First, typename... Rest>
auto parameters_sort(First&& first, Rest&&... rest)
{
	auto rest_result = parameters_sort(std::forward<Rest>(rest)...);

	if constexpr (std::is_same_v<std::decay_t<First>, input<std::decay_t<decltype(first.t)>>>)
	{
		return std::tuple_cat(
			std::make_tuple(std::tuple_cat(std::tuple<std::decay_t<First>>(std::forward<First>(first)), std::get<0>(rest_result))),
			std::make_tuple(std::get<1>(rest_result))
		);
	}
	else {
		return std::tuple_cat(
			std::make_tuple(std::get<0>(rest_result)),
			std::make_tuple(std::tuple_cat(std::tuple<std::decay_t<First>>(std::forward<First>(first)), std::get<1>(rest_result)))
		);
	}
}

template<wrapper... Args>
void rpc(Args&&... args)
{
	tls::parameters_bind(std::forward<Args>(args)...);
	tls::write();
	tls::wait_response();
	tls::read();
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
