#include <iostream>
#include <utility>
#include <functional>
#include <type_traits>
#include <tuple>
#include <concepts>
#include <optional>
#include <queue>

//#include <boost\lockfree\queue.hpp>



template<typename T>
struct input_t { T& t; };

template<typename T>
struct is_input_t : std::false_type {};

template<typename Ty>
struct is_input_t<input_t<Ty>> : std::true_type {};

template<typename T>
inline constexpr bool is_input_v = is_input_t<T>::value;

template<typename... Ty>
struct multi_input_t { std::tuple<Ty&...> t; };

template<typename T>
struct is_multi_input_t : std::false_type {};

template<typename... Ty>
struct is_multi_input_t<multi_input_t<Ty...>> : std::true_type {};

template<typename T>
inline constexpr bool is_multi_input_v = is_multi_input_t<T>::value;

template<typename T>
struct output_t { T& t; };

template<typename T>
struct is_output_t : std::false_type {};

template<typename Ty>
struct is_output_t<output_t<Ty>> : std::true_type {};

template<typename T>
inline constexpr bool is_output_v = is_output_t<T>::value;

template<typename... Ty>
struct multi_output_t { std::tuple<Ty&...> t; };

template<typename T>
struct is_multi_output_t : std::false_type {};

template<typename... Ty>
struct is_multi_output_t<multi_output_t<Ty...>> : std::true_type {};

template<typename T>
inline constexpr bool is_multi_output_v = is_multi_output_t<T>::value;

template<template<typename> class U, typename... Ty>
struct is_instance_of_t : std::false_type {};

template<template<typename> class U, typename... Ty>
struct is_instance_of_t<U, U<Ty...>> : std::true_type {};

template<template<typename> class U, typename... Ty>
inline constexpr bool is_instance_of_v = is_instance_of_t<U, Ty...>::value;

template<typename... Ty>
concept wrapper = is_instance_of_v<input_t, Ty...> || is_instance_of_v<output_t, Ty...>
|| is_instance_of_v<multi_input_t, Ty...> || is_instance_of_v<multi_output_t, Ty...>;


class yas
{
public:
	template<typename ...Args>
	static size_t count(Args&&...args) {

	}
	template<typename ...Ts>
	static void save(char* addr, const std::tuple<Ts...>& theTuple) {
		//TODO 等待添加yas后完善
		std::apply([](const Ts&... args) {}, theTuple);
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

enum class mem_type : uint32_t
{
	tls,
};

struct memory_info
{
	mem_type type;
	size_t index;
};

template<typename T>
concept malloc_param = std::is_same_v<T, uint32_t>; /*|| requires(T t) {
	{ t() } -> std::convertible_to<uint32_t>;}*/

// 链式调用
class malloc_manager
{
public:
	virtual std::tuple<char*, memory_info> malloc_inner(size_t size) = 0;

	virtual malloc_manager& set_next(malloc_manager& _next) {
		next.emplace(_next);
		return *this;
	};	

	memory_info get_tls_info() { return mem_info; };

	std::tuple<char*, memory_info> malloc(size_t size)
	{		
		if (const auto ptr_and_mem_info = malloc_inner(size); std::get<0>(ptr_and_mem_info)){
			return ptr_and_mem_info;
		}
		if (next.has_value())
			return next.value().get().malloc(size);
		return {};
	};

protected:
	std::optional<std::reference_wrapper<malloc_manager>> next;
	inline static thread_local memory_info mem_info;
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


template<typename T>
using mq = std::queue<T>;


template<terminal Terminal>
class event_list
{
public:
	event_list(size_t size)
		:semaphore_array(size),
		index_mq()
	{ }

	size_t allocte() {
		auto init = [this] {
			size_t index{0};
			//index_mq.pop(index);
			return index;
		};
		thread_local size_t tls_index{ init() };
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
class tls_memory : public malloc_manager
{
public:
	tls_memory()
		:memory_list(32, 4096),
		synchronize(32)
	{}

	std::tuple<char*, memory_info> malloc_inner(size_t size) {
		auto index = synchronize.allocte();
		if (size > capacity) return { nullptr,{} };
		return { memory_list.at(size).get(), { mem_type::tls, index }};
	}

	char* get_synchronize_addr() { }

private:
	const size_t capacity{ 4096 };
	std::vector<basic_shared_memory> memory_list;
	event_list<Terminal> synchronize;	
};





class managered_memory : public malloc_manager
{
public:
	std::tuple<char*, memory_info> malloc_inner(size_t size) {


		return { nullptr,{} };
	}
};

class file_transefer : public malloc_manager
{
public:
	std::tuple<char*, memory_info> malloc_inner(size_t size) {
		// TODO 		
		return { nullptr,{} };
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
		auto [ptr, mem_info] =  entrance.malloc(size);
		// 将信息写入共享内存
		first.
		return ptr;
	}

	std::tuple<char*, size_t> find(memory_info info) const {
		return { nullptr, 0 };
	}

private:
	tls_memory<Terminal> first;
	managered_memory second;
	file_transefer last;
	malloc_manager& entrance;
};

template<terminal Terminal>
class frame
{
public:
	void trigger()
	{

	}

	constexpr static void wait_response() {}
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

class seriliasize_buffer
{
public:
	explicit seriliasize_buffer(const char* _addr = nullptr, std::size_t _size = 0) :addr(_addr), len(_size) {}

	template<typename T>
	seriliasize_buffer& operator& (T&& vv) 
	{
		return *this;
	}

	template<typename T>
	seriliasize_buffer& operator<<(T&& vv)
	{
		return *this;
	}

	template<typename T>
	seriliasize_buffer& operator>>(T&& vv)
	{
		return *this;
	}	

	size_t size() const {
		return len;
	}

private:
	const char* addr;
	const std::size_t len;
};


class params;


template<typename T>
concept malloc_fn_concept = requires (T t) {
	{ t() } -> std::same_as<char*>;
};

template<size_t N>
class execute
{		
	using malloc_fn = std::function<char*()>;
	using find_fn = std::function<std::tuple<char*, size_t>()>;
	using write_fn = std::function<void(malloc_fn&&)>;
	using count_fn = std::function<std::size_t()>;
	using read_fn = std::function<void(char*,size_t)>;
	
public:
	execute(const char(&_name)[N])
		: name{ _name }
		, name_len{ N-1 }
	{

	}

	template<typename... Ty1,typename... Ty2>
	constexpr execute& bind(const std::tuple<Ty1&...>& in, const std::tuple<Ty2&...>& out)
	{	
		this->counter = [&,in]() {
			seriliasize_buffer seriliasizer;
			std::apply([&](auto&&... args) { ((seriliasizer & std::forward<decltype(args)>(args)), ...); }, in);
			return seriliasizer.size();
		};
		
		this->writer = [&, in](malloc_fn_concept auto&& malloc) {
			seriliasize_buffer seriliasizer( malloc() );
			std::apply([&](auto&&... args) { ((seriliasizer << std::forward<decltype(args)>(args)), ...); }, in);
		};

		this->reader = [&, out](const char* addr, size_t size)  mutable {
			seriliasize_buffer seriliasizer(addr, size);
			std::apply([&](auto&&... args) { ((seriliasizer >> std::forward<decltype(args)>(args)), ...); }, in);
		};

		return *this;
	}

	constexpr execute& write() {
		auto size = std::invoke(this->counter);
		std::invoke(this->writer, [size]() { return frame<terminal::client>::get().memory_mng().malloc(size); });
		return *this;
	}

	constexpr execute& wait_result() {
		// 写入通信信息，写入触发信号，等待触发信号		
		frame<terminal::client>::wait_response();
		return *this;
	}

	constexpr execute& read() {
		
		std::invoke(this->reader, nullptr, 0);
		return *this;
	}

private:
	const char* name;
	const std::size_t name_len;
	count_fn counter;
	write_fn writer;
	read_fn reader;
};


class params {

public:

	template<wrapper First, wrapper... Rest>
	static constexpr auto sort(First&& first, Rest&&... rest) -> decltype(auto)
	{
		auto rest_result = sort(std::forward<Rest>(rest)...);

		if constexpr (is_input_t<First>::value) {
			return handle_input(std::forward<First>(first), std::forward<decltype(rest_result)>(rest_result));
		}
		else if constexpr (is_output_t<First>::value) {
			return handle_output(std::forward<First>(first), std::forward<decltype(rest_result)>(rest_result));
		}
		else if constexpr (is_multi_input_t<First>::value) {
			return handle_multi_input(std::forward<First>(first), std::forward<decltype(rest_result)>(rest_result));
		}
		else if constexpr (is_multi_output_t<First>::value) {
			return handle_multi_output(std::forward<First>(first), std::forward<decltype(rest_result)>(rest_result));
		}
		else {
			return handle_default(std::forward<decltype(rest_result)>(rest_result));
		}
	}

private:
	static constexpr auto sort() -> std::tuple<std::tuple<>, std::tuple<>>
	{
		return std::tuple<std::tuple<>, std::tuple<>>();
	}

	template<typename First, typename RestResult>
	static constexpr auto handle_input(First&& first, RestResult&& rest_result)
		-> decltype(auto)
	{
		return std::tuple_cat(
			std::make_tuple(std::tuple_cat(std::tie(first.t), std::get<0>(rest_result))),
			std::make_tuple(std::get<1>(rest_result))
		);
	}

	template<typename First, typename RestResult>
	static constexpr auto handle_output(First&& first, RestResult&& rest_result)
		-> decltype(auto)
	{
		return std::tuple_cat(
			std::make_tuple(std::get<0>(rest_result)),
			std::make_tuple(std::tuple_cat(std::tie(first.t), std::get<1>(rest_result)))
		);
	}

	template<typename First, typename RestResult>
	static constexpr auto handle_multi_input(First&& first, RestResult&& rest_result)
		-> decltype(auto)
	{
		return std::tuple_cat(
			std::make_tuple(std::tuple_cat(first.t, std::get<0>(rest_result))),
			std::make_tuple(std::get<1>(rest_result))
		);
	}

	template<typename First, typename RestResult>
	static constexpr auto handle_multi_output(First&& first, RestResult&& rest_result)
		-> decltype(auto)
	{
		return std::tuple_cat(
			std::make_tuple(std::get<0>(rest_result)),
			std::make_tuple(std::tuple_cat(first.t, std::get<1>(rest_result)))
		);
	}

	template<typename RestResult>
	static constexpr auto handle_default(RestResult&& rest_result)
		-> decltype(auto)
	{
		return std::tuple_cat(
			std::make_tuple(std::get<0>(rest_result)),
			std::make_tuple(std::get<1>(rest_result))
		);
	}
};

template <typename First, typename... Rest>
constexpr auto input(First& first, Rest&... rests)
-> decltype(auto)
{
	if constexpr (sizeof...(rests) == 0) {
		return input_t(first);
	}
	else {
		return multi_input_t(std::tie(first, rests...));
	}
}


template <typename First, typename... Rest>
constexpr auto output(First& first, Rest&... rests)
-> decltype(auto)
{
	if constexpr (sizeof...(rests) == 0) {
		return output_t(first);
	}
	else {
		return multi_output_t(std::tie(first, rests...));
	}
}

enum class level : uint32_t {
	disable,
	base,
	detaile,
};


class trace
{
public:
	void trace_detaile()
	{

	}
	void trace_disable()
	{

	}
	void trace_base()
	{
	}

	void log_detaile()
	{

	}

	void log_disable()
	{

	}

	trace& info()
	{
		return *this;
	}

	void log_base()
	{

	}

	void performance()
	{

	}

	friend std::ostream& operator<<(std::ostream& os, const trace& t);

	void callback_register(std::function<void()>& f)
	{

	}
private:


};

std::ostream& operator<<(std::ostream& os, const trace& t)
{
	return os;
}


trace global_config;

class rpc
{
public:
	template<size_t N, wrapper... Args>
	static constexpr void call(const char(&name)[N], Args&&... args)
	{
		thread_local execute<N> executer(name);
		const auto [in, out] = params::sort(std::forward<Args>(args)...);
		executer.bind(in, out).write().wait_result().read();
	}

	static trace& config()
	{
		return global_config;
	}
};



int main()
{

	int a{ 1 };
	double b{ 2 };
	float c{ 3 };
	bool d{ false };
	int e{ 4 };
	int f{ 6 };

	rpc::config().log_detaile(); // 详细日志
	rpc::config().trace_detaile(); // 详细追踪

	rpc::call("name", input(a, b), output(c), output(d, e), input(f)); // 发起调用

	rpc::call("name", input(a, b), output(c), output(d, e), input(f)); // 发起调用

	rpc::config().log_disable(); // 关闭日志

	rpc::config().performance(); // 开启性能统计

	rpc::call("name", input(a, b), output(c), output(d, e), input(f)); // 发起调用

	std::cout << rpc::config().info(); // 输出日志

	return 0;
}



