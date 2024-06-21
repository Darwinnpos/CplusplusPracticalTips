
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
#include <queue>

// 参数包装器
template<typename T>
struct input { T& t; };

template<typename T>
struct output { T& t; };

template<typename T>
struct unused { T& t; };

// wrapper 概念
template<template<typename> class U, typename T>
struct is_instance_of : std::false_type {};

template<template<typename> class U, typename T>
struct is_instance_of<U, U<T>> : std::true_type {};

template<typename T>
concept wrapper = is_instance_of<input, T>::value || is_instance_of<output, T>::value || is_instance_of<unused, T>::value;

// 包参数处理 
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
	std::cout << typeid(T).name() << ": " << t;
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
public:
	void release() { }
	void wait() { }
};

template<typename T>
class mq
{
public:
	T pop()
	{
		return T();
	}
	void push(T&)
	{

	}
};

enum class terminal
{
	client,
	server
};

template<terminal Terminal>
class event_list
{
public:
	event_list(size_t size) { }

	size_t allocte() {
		thread_local size_t index{ index_mq.pop() };
		return index;
	}

private:
	std::vector<semaphore> semaphore_array;
	mq<size_t> index_mq;
};


class basic_shared_memory
{
public:
	basic_shared_memory(size_t bytes)
		:size(bytes),
		addr(new char[bytes])
	{
		
	}

	char* get() { return addr; }

private:
	size_t size;
	char* addr;
};

template<terminal Terminal>
class tls_memory : public malloc_chain 
{
public:
	tls_memory()
		:memory_list(32, 4096),
		synchronize(32)
	{}

	std::optional<char*> malloc_inner(size_t size) override {
		auto index = synchronize.allocte();
		if (size > capacity) return {};
		return memory_list.at(size).get();
	}

private:
	const size_t capacity{ 4096 };
	std::vector<basic_shared_memory> memory_list;
	event_list<Terminal> synchronize;
};

class managered_memory : public malloc_chain
{
public:
	std::optional<char*> malloc_inner(size_t size)override {
		// TODO 		
		return {};
	}
};

class file_transefer : public malloc_chain
{
public:
	std::optional<char*> malloc_inner(size_t size) override {
		// TODO 		
		return {};
	}
};

template<terminal Terminal>
class shared_memory
{
public:
	shared_memory()
		:entrance{ first }
	{
		first.set_next(second).set_next(last);
	}

	char* malloc(size_t size) {
		
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
	tls_memory<Terminal> first;
	managered_memory second;
	file_transefer last;
	malloc_chain& entrance;
};

class seriliasize
{
public:
	template<typename... Ts>
	static void to_parameters(const std::tuple<Ts...>& theTuple)
	{
		auto [ptr, size] = shared_memory<terminal::client>::get().find();
		yas::load(ptr, size, 0);

		std::cout << "deserialize:";
		
		std::apply([](const Ts&... args) {
			std::size_t n = 0;
			((std::cout << (n++ ? ", " : "") << typeid(args.t).name() << ": ", print_element(args)), ...);
			}, theTuple);

		std::cout << std::endl;
	}


	template<typename... Ts>
	static size_t count(const std::tuple<Ts...>& theTuple)
	{
		return 0;
	}

	template<typename... Ts>
	static void to_string(const std::tuple<Ts...>& theTuple)
	{
		// try malloc the shared memory	
		auto ptr = shared_memory<terminal::client>::get().malloc(count(theTuple));
		yas::save(ptr, 0);
		std::cout << "serialize:";
		std::apply([](const Ts&... args) {
			std::size_t n = 0;
			((std::cout << (n++ ? ", " : "") << typeid(args.t).name() << ": ", print_element(args)), ...);
			}, theTuple);
		std::cout << std::endl;
	}
};


struct params;
// 参数执行器
struct execute
{
	using fn = std::function<void()>;
	fn write;
	fn read;

	template<typename... Args>
	void params_bind(Args&&... args)
	{
		auto [inputs, outputs] = params::sort(std::forward<Args>(args)...);
		write = [=]() { seriliasize::to_string(inputs); };
		read = [=]() mutable { seriliasize::to_parameters(outputs); };
	}

	void wait_response()
	{
		 
	}
};


struct params {	
	
	static auto sort()
	{
		return std::tuple<std::tuple<>, std::tuple<>>();
	}

	// 参数收集器
	template<wrapper First, wrapper... Rest>
	static auto sort(First&& first, Rest&&... rest)
	{
		auto rest_result = sort(std::forward<Rest>(rest)...);

		if constexpr (std::is_same_v<std::decay_t<First>, input<std::decay_t<decltype(first.t)>>>)
		{
			return std::tuple_cat(
				std::make_tuple(
					std::tuple_cat(std::tuple<std::decay_t<First>>(std::forward<First>(first)),
						std::get<0>(rest_result))),
				std::make_tuple(std::get<1>(rest_result))
			);
		}
		else {
			return std::tuple_cat(
				std::make_tuple(std::get<0>(rest_result)),
				std::make_tuple(
					std::tuple_cat(std::tuple<std::decay_t<First>>(std::forward<First>(first)),
						std::get<1>(rest_result)))
			);
		}
	}
};


template<wrapper... Args>
void rpc(Args&&... args)
{
	thread_local execute exe;
	exe.params_bind(std::forward<Args>(args)...);
	exe.write(); // 参数的分类调用
	exe.wait_response();
	exe.read();  // 参数的分类延迟调用
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
	int e{ 4 };
	int f{ 6 };

	// 参数包装器应用实例
	rpc(input(a), input(b), output(c), output(d), output(e), input(f)); //9 * 400  3600个手动编写的代码，或者400个耦合复杂逻辑的代码
	
	rpc(input(a), input(b), output(c), output(d), output(e), input(f)); 
	//rpc(inputs(a, b), output(c), output(d));

	return 0;
}
