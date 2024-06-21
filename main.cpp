
#include <iostream>
#include <utility>
#include <functional>
#include <type_traits>
#include <tuple>
#include <concepts>
#include <optional>
#include <queue>

#include <boost\lockfree\queue.hpp>

template<typename T>
using mq = boost::lockfree::queue<T>;

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
	template<typename ...Ts>
	static void save(char* addr, const std::tuple<Ts...>& theTuple) {
		//TODO 等待添加yas后完善
		std::apply([](const Ts&... args) { 	}, theTuple);
	}
	template<typename ...Ts>
	static void load(char*, size_t size, const std::tuple<Ts...>& theTuple) {


	}
};


class stream_object
{
public:
	explicit stream_object(size_t) {
		// malloc the memory 
	};
};


// 链式调用
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

enum class terminal
{
	client,
	server
};

template<terminal Terminal>
class event_list
{
public:
	event_list(size_t size)
		:semaphore_array(size),
		index_mq(size)
	{ }

	size_t allocte() {
		auto init = [this] { 
			size_t index;
			index_mq.pop(index);
			return index;
		};
		thread_local size_t tls_index{ init()};
		return tls_index;
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

	char* malloc(size_t size) const {
		
		auto opt_prt = entrance.malloc(size);
		if (opt_prt.has_value())return opt_prt.value();
		throw std::runtime_error("shared memory malloc failed.");
	}

	std::tuple<char*, size_t> find() const {
		return { nullptr,0 };
	}

private:	
	tls_memory<Terminal> first;
	managered_memory second;
	file_transefer last;
	malloc_chain& entrance;
};

template<terminal Terminal>
class frame
{
public:
	void trigger()
	{

	}


	static frame& get()
	{
		if (!instance)instance = new frame();
		return *instance;
	}
	shared_memory<Terminal>& memory_mng() 
	{
		return shd_memory;
	}

private:
	inline static frame* instance;
	shared_memory<Terminal> shd_memory;
};

class seriliasize
{
public:
	template<typename T, typename... Ts>
	static void to_parameters(T& memory_mng, const std::tuple<Ts...>& theTuple)
	{
		auto [ptr, size] = memory_mng.find();
		yas::load(ptr, size, theTuple);

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

	template<typename T ,typename... Ts>
	static void to_string(const T& memory_mng, const std::tuple<Ts...>& theTuple)
	{
		// try malloc the shared memory	
		auto ptr = memory_mng.malloc(count(theTuple));
		yas::save(ptr, theTuple);
		std::cout << "serialize:";
		std::apply([](const Ts&... args) {
			std::size_t n = 0;
			((std::cout << (n++ ? ", " : "") << typeid(args.t).name() << ": ", print_element(args)), ...);
			}, theTuple);
		std::cout << std::endl;
	}
};


class params;

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
		const auto& memory_mng = frame<terminal::client>::get().memory_mng();
		write = [&,inputs]() { seriliasize::to_string(memory_mng, inputs); };
		read = [&,outputs]() mutable { seriliasize::to_parameters(memory_mng, outputs); };
	}

	void wait_response()
	{
		 // 

		//
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
	
	//rpc(inputs(a, b), output(c), output(d));

	return 0;
}
