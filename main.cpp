#include <iostream>
#include <utility>
#include <functional>
#include <type_traits>
#include <tuple>


#include <queue>


#if defined(_MSC_VER)
// 使用 MSVC 编译器
#if _MSVC_LANG >= 202002L
#define CPP20_SUPPORTED
#elif _MSVC_LANG >= 201703L
#define CPP17_SUPPORTED
#elif _MSVC_LANG >= 201402L
#define CPP14_SUPPORTED
#elif _MSVC_LANG >= 201103L
#define CPP11_SUPPORTED
#endif
#elif defined(__GNUC__)
// 使用 GNU 编译器
#if __cplusplus >= 202002L
#define CPP20_SUPPORTED
#elif __cplusplus >= 201703L
#define CPP17_SUPPORTED
#elif __cplusplus >= 201402L
#define CPP14_SUPPORTED
#elif __cplusplus >= 201103L
#define CPP11_SUPPORTED
#endif
#endif

// 不同C++版本的实现

#if defined(CPP20_SUPPORTED)
#include <concepts>

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

template<typename T>
concept input_concept = is_input_t<std::decay_t<T>>::value;

template<typename T>
concept output_concept = is_output_t<std::decay_t<T>>::value;

template<typename... T>
concept multi_input_concept = is_multi_input_t<multi_input_t<T...>>::value;

template<typename... T>
concept multi_output_concept = is_multi_output_t<multi_output_t<T...>>::value;

template<typename... Ty>
concept wrapper_concept = is_instance_of_v<input_t, Ty...> || is_instance_of_v<output_t, Ty...>
|| is_instance_of_v<multi_input_t, Ty...> || is_instance_of_v<multi_output_t, Ty...>;


#elif defined(CPP17_SUPPORTED) || defined(CPP14_SUPPORTED) || defined(CPP11_SUPPORTED)

template<typename T>
struct input_t { T& t; };

template<typename T>
struct is_input_t : std::false_type {};

template<typename Ty>
struct is_input_t<input_t<Ty>> : std::true_type {};

template<typename T>
constexpr bool is_input_v = is_input_t<T>::value;

template<typename T>
struct output_t { T& t; };

template<typename T>
struct is_output_t : std::false_type {};

template<typename Ty>
struct is_output_t<output_t<Ty>> : std::true_type {};

template<typename T>
constexpr bool is_output_v = is_output_t<T>::value;

template<typename... Ty>
struct multi_input_t { std::tuple<Ty&...> t; };

template<typename T>
struct is_multi_input_t : std::false_type {};

template<typename... Ty>
struct is_multi_input_t<multi_input_t<Ty...>> : std::true_type {};

template<typename T>
constexpr bool is_multi_input_v = is_multi_input_t<T>::value;

template<typename... Ty>
struct multi_output_t { std::tuple<Ty&...> t; };

template<typename T>
struct is_multi_output_t : std::false_type {};

template<typename... Ty>
struct is_multi_output_t<multi_output_t<Ty...>> : std::true_type {};

template<typename T>
constexpr bool is_multi_output_v = is_multi_output_t<T>::value;

template<template<typename> class U, typename T>
struct is_instance_of_t : std::false_type {};

template<template<typename> class U, typename... Ty>
struct is_instance_of_t<U, U<Ty...>> : std::true_type {};

template<template<typename> class U, typename T>
constexpr bool is_instance_of_v = is_instance_of_t<U, T>::value;

#endif

#if defined(CPP17_SUPPORTED)

template<typename... T>
struct is_wrapper_t : std::disjunction<
	std::integral_constant<bool, is_input_v<T> || is_output_v<T> || is_multi_input_v<T> || is_multi_output_v<T>>...> {};

template<typename... T>
constexpr bool is_wrapper_v = is_wrapper_t<T...>::value;

template<typename... T>
using wrapper = std::enable_if_t<is_wrapper_v<T...>, bool>;

#elif defined(CPP14_SUPPORTED)

template<typename T>
struct is_wrapper_element : std::integral_constant<bool,
	is_input_v<T> || is_output_v<T> || is_multi_input_v<T> || is_multi_output_v<T>> {};

template<typename... T>
struct is_wrapper_t : std::false_type {};

template<typename... T>
struct is_wrapper_t<std::tuple<T...>> : std::integral_constant<bool,
	is_wrapper_element<T>::value && ...> {};

template<typename... T>
constexpr bool is_wrapper_v = is_wrapper_t<T...>::value;

template<typename... T>
using wrapper = std::enable_if_t<is_wrapper_v<T...>, bool>;

#elif defined(CPP11_SUPPORTED)

template<typename T>
struct is_wrapper_element : std::integral_constant<bool,
	is_input_v<T> || is_output_v<T> || is_multi_input_v<T> || is_multi_output_v<T>> {};

template<typename... T>
struct is_wrapper_t : std::false_type {};

template<typename... T>
struct is_wrapper_t<std::tuple<T...>> : std::integral_constant<bool,
	is_wrapper_element<T>::value && ...> {};

template<typename... T>
constexpr bool is_wrapper_v = is_wrapper_t<T...>::value;

template<typename... T>
using wrapper = typename std::enable_if<is_wrapper_v<T...>, bool>::type;

#endif

class yas
{
public:
	template<typename ...Args>
	static size_t count(Args&&...args) {

	}
	template<typename ...Ts>
	static void save(char* addr, const std::tuple<Ts...>& theTuple) {
		//TODO 等待添加yas后完善
		//std::apply([](const Ts&... args) {}, theTuple);
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


#if defined(CPP20_SUPPORTED) || defined(CPP17_SUPPORTED)

#include <optional>
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
		if (next)
			return next.value().get().malloc(size);
		return { nullptr,{ {},{} } };
	};

protected:

	std::optional<std::reference_wrapper<malloc_manager>> next;
	inline static thread_local memory_info mem_info;	
};
#else

class malloc_manager
{
public:
	virtual std::tuple<char*, memory_info> malloc_inner(size_t size) = 0;

	virtual malloc_manager& set_next(malloc_manager& _next) {
		next.reset(&_next);
		return *this;
	}

	memory_info get_tls_info() { return mem_info; }

	std::tuple<char*, memory_info> malloc(size_t size)
	{
		auto ptr_and_mem_info = malloc_inner(size);
		if (std::get<0>(ptr_and_mem_info)) {
			return ptr_and_mem_info;
		}
		if (next) {
			return next->malloc(size);
		}
		return std::make_tuple(nullptr, memory_info{});
	}

protected:
	std::unique_ptr<malloc_manager> next;
	static thread_local memory_info mem_info;
};

thread_local memory_info malloc_manager::mem_info;
#endif

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

#if defined(CPP20_SUPPORTED) || defined(CPP17_SUPPORTED)
		auto [ptr, mem_info] =  entrance.malloc(size);
#else
		auto ret = entrance.malloc(size);
		auto ptr = std::get<0>(ret);
		auto mem_info = std::get<1>(ret);
#endif
		// 将信息写入共享内存
		//first.
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
#if defined(CPP20_SUPPORTED) || defined(CPP17_SUPPORTED) 
	template<typename... Ty1,typename... Ty2>
	constexpr execute& bind(const std::tuple<Ty1&...>& in, const std::tuple<Ty2&...>& out)
	{	
		this->counter = [&,in]() {
			seriliasize_buffer seriliasizer;
			std::apply([&](auto&&... args) { ((seriliasizer & std::forward<decltype(args)>(args)), ...); }, in);
			return seriliasizer.size();
		};
		
		this->writer = [&, in](malloc_fn&& malloc) {
			seriliasize_buffer seriliasizer( malloc() );
			std::apply([&](auto&&... args) { ((seriliasizer << std::forward<decltype(args)>(args)), ...); }, in);
		};

		this->reader = [&, out](const char* addr, size_t size)  mutable {
			seriliasize_buffer seriliasizer(addr, size);
			std::apply([&](auto&&... args) { ((seriliasizer >> std::forward<decltype(args)>(args)), ...); }, in);
		};

		return *this;
	}

#elif defined(CPP14_SUPPORTED) 
	template<typename Tuple, typename Func, std::size_t... I>
	void tuple_for_each_impl(Tuple&& t, Func&& f, std::index_sequence<I...>)
	{
		using expander = int[];
		(void)expander {
			0, (void(f(std::get<I>(std::forward<Tuple>(t)))), 0)...
		};
	}

	template<typename Tuple, typename Func>
	void tuple_for_each(Tuple&& t, Func&& f)
	{
		constexpr auto size = std::tuple_size<typename std::decay<Tuple>::type>::value;
		tuple_for_each_impl(std::forward<Tuple>(t), std::forward<Func>(f), std::make_index_sequence<size>{});
	}

	template<typename... Ty1, typename... Ty2>
	execute& bind(const std::tuple<Ty1&...>& in, const std::tuple<Ty2&...>& out)
	{
		this->counter = [&, in]() {
			seriliasize_buffer seriliasizer;
			tuple_for_each(in, [&](auto&& arg) { seriliasizer& std::forward<decltype(arg)>(arg); });
			return seriliasizer.size();
			};

		this->writer = [&, in](malloc_fn&& malloc) {
			seriliasize_buffer seriliasizer(malloc());
			tuple_for_each(in, [&](auto&& arg) { seriliasizer << std::forward<decltype(arg)>(arg); });
			};

		this->reader = [&, out](const char* addr, size_t size) mutable {
			seriliasize_buffer seriliasizer(addr, size);
			tuple_for_each(out, [&](auto&& arg) { seriliasizer >> std::forward<decltype(arg)>(arg); });
			};

		return *this;
	}
#elif defined(CPP11_SUPPORTED) 
	template<std::size_t...>
	struct index_sequence {};

	template<std::size_t N, std::size_t... Is>
	struct make_index_sequence : make_index_sequence<N - 1, N - 1, Is...> {};

	template<std::size_t... Is>
	struct make_index_sequence<0, Is...> : index_sequence<Is...> {};

	template<typename Tuple, typename Func, std::size_t... I>
	void tuple_for_each_impl(Tuple&& t, Func&& f, index_sequence<I...>)
	{
		using expander = int[];
		(void)expander {
			0, (void(f(std::get<I>(std::forward<Tuple>(t)))), 0)...
		};
	}

	template<typename Tuple, typename Func>
	void tuple_for_each(Tuple&& t, Func&& f)
	{
		const std::size_t size = std::tuple_size<typename std::decay<Tuple>::type>::value;
		tuple_for_each_impl(std::forward<Tuple>(t), std::forward<Func>(f), make_index_sequence<size>{});
	}

	template<typename... Ty1, typename... Ty2>
	execute& bind(const std::tuple<Ty1&...>& in, const std::tuple<Ty2&...>& out)
	{
		this->counter = [&, in]() {
			seriliasize_buffer seriliasizer;
			tuple_for_each(in, [&](auto&& arg) { seriliasizer& std::forward<decltype(arg)>(arg); });
			return seriliasizer.size();
			};

		this->writer = [&, in](malloc_fn&& malloc) {
			seriliasize_buffer seriliasizer(malloc());
			tuple_for_each(in, [&](auto&& arg) { seriliasizer << std::forward<decltype(arg)>(arg); });
			};

		this->reader = [&, out](const char* addr, size_t size) mutable {
			seriliasize_buffer seriliasizer(addr, size);
			tuple_for_each(out, [&](auto&& arg) { seriliasizer >> std::forward<decltype(arg)>(arg); });
			};

		return *this;
	}
#endif

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
#if defined(CPP20_SUPPORTED)
	template<wrapper_concept First, wrapper_concept... Rest>
	static constexpr auto sort(First&& first, Rest&&... rest) -> decltype(auto)
	{
		auto rest_result = sort(std::forward<Rest>(rest)...);
		return sort_impl(std::forward<First>(first), std::forward<decltype(rest_result)>(rest_result));
	}
#elif defined(CPP17_SUPPORTED) || defined(CPP14_SUPPORTED) || defined(CPP11_SUPPORTED)
	template<typename First, typename... Rest, wrapper<First> = 0>
	static auto sort(First&& first, Rest&&... rest) -> decltype(auto) {
		auto rest_result = sort(std::forward<Rest>(rest)...);
		return sort_impl(std::forward<First>(first), std::forward<decltype(rest_result)>(rest_result));
	}
#endif
private:
#if defined(CPP20_SUPPORTED)
	static constexpr auto sort() -> std::tuple<std::tuple<>, std::tuple<>>
	{
		return std::tuple<std::tuple<>, std::tuple<>>();
	}

	template<input_concept First, typename RestResult>
	static constexpr auto sort_impl(First&& first, RestResult&& rest_result)
		-> decltype(auto)
	{
		return std::tuple_cat(
			std::make_tuple(std::tuple_cat(std::tie(first.t), std::get<0>(rest_result))),
			std::make_tuple(std::get<1>(rest_result))
		);
	}

	template<output_concept First, typename RestResult>
	static constexpr auto sort_impl(First&& first, RestResult&& rest_result)
		-> decltype(auto)
	{
		return std::tuple_cat(
			std::make_tuple(std::get<0>(rest_result)),
			std::make_tuple(std::tuple_cat(std::tie(first.t), std::get<1>(rest_result)))
		);
	}

	template<multi_input_concept First, typename RestResult>
	static constexpr auto sort_impl(First&& first, RestResult&& rest_result)
		-> decltype(auto)
	{
		return std::tuple_cat(
			std::make_tuple(std::tuple_cat(first.t, std::get<0>(rest_result))),
			std::make_tuple(std::get<1>(rest_result))
		);
	}

	template<multi_output_concept First, typename RestResult>
	static constexpr auto sort_impl(First&& first, RestResult&& rest_result)
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
#elif defined(CPP17_SUPPORTED) || defined(CPP14_SUPPORTED) || defined(CPP11_SUPPORTED)
	static auto sort() -> std::tuple<std::tuple<>, std::tuple<>> {
		return std::tuple<std::tuple<>, std::tuple<>>();
	}
	template<typename First, typename RestResult, typename std::enable_if<is_input_t<First>::value, int>::type = 0>
	static auto sort_impl(First&& first, RestResult&& rest_result) -> decltype(auto) {
		return std::tuple_cat(
			std::make_tuple(std::tuple_cat(std::tie(first.t), std::get<0>(rest_result))),
			std::make_tuple(std::get<1>(rest_result))
		);
	}
	template<typename First, typename RestResult, typename std::enable_if<is_output_t<First>::value, int>::type = 0>
	static auto sort_impl(First&& first, RestResult&& rest_result) -> decltype(auto) {
		return std::tuple_cat(
			std::make_tuple(std::get<0>(rest_result)),
			std::make_tuple(std::tuple_cat(std::tie(first.t), std::get<1>(rest_result)))
		);
	}
	template<typename First, typename RestResult, typename std::enable_if<is_multi_input_t<First>::value, int>::type = 0>
	static auto sort_impl(First&& first, RestResult&& rest_result) -> decltype(auto) {
		return std::tuple_cat(
			std::make_tuple(std::tuple_cat(first.t, std::get<0>(rest_result))),
			std::make_tuple(std::get<1>(rest_result))
		);
	}

	template<typename First, typename RestResult, typename std::enable_if<is_multi_output_t<First>::value, int>::type = 0>
	static auto sort_impl(First&& first, RestResult&& rest_result) -> decltype(auto) {
		return std::tuple_cat(
			std::make_tuple(std::get<0>(rest_result)),
			std::make_tuple(std::tuple_cat(first.t, std::get<1>(rest_result)))
		);
	}

	template<typename First, typename RestResult, typename std::enable_if<!is_input_t<First>::value && !is_output_t<First>::value &&
		!is_multi_input_t<First>::value && !is_multi_output_t<First>::value, int>::type = 0>
	static auto sort_impl(First&& first, RestResult&& rest_result) -> decltype(auto) {
		return std::tuple_cat(
			std::make_tuple(std::get<0>(rest_result)),
			std::make_tuple(std::get<1>(rest_result))
		);
	}

#endif
};

#if defined(CPP20_SUPPORTED) 
template <typename First, typename... Rest>
constexpr auto input(First&& first, Rest&&... rests)
-> decltype(auto)
{
	if constexpr (sizeof...(rests) == 0) {
		return input_t(first);
	}
	else {
		return multi_input_t(std::tie(first, rests...));
	}
}
#elif defined(CPP17_SUPPORTED) 

template <typename... Args>
auto input(Args&&... args) {
	if constexpr (sizeof...(Args) == 1) {
		return input_t<std::decay_t<Args>...>{ std::forward<Args>(args)... };
	}
	else {
		return multi_input_t<std::decay_t<Args>...>{ std::tie(args...) };
	}
}

#elif defined(CPP14_SUPPORTED) || defined(CPP11_SUPPORTED)
template <typename First, typename... Rest>
struct input_helper {
	static auto create(First& first, Rest&... rests)
		-> multi_input_t<First, Rest...> {
		return { std::tie(first, rests...) };
	}
};

template <typename First>
struct input_helper<First> {
	static auto create(First&& first) -> input_t<First> {
		return { first };
	}
};

template <typename First, typename... Rest>
auto input(First&& first, Rest&&... rests)
-> decltype(input_helper<First, Rest...>::create(first, rests...)) {
	return input_helper<First, Rest...>::create(first, rests...);
}
#endif
#ifdef CPP20_SUPPORTED
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
#elif defined(CPP17_SUPPORTED) 

template <typename... Args>
auto output(Args&&... args) {
	if constexpr (sizeof...(Args) == 1) {
		return output_t<std::decay_t<Args>...>{ std::forward<Args>(args)... };
	}
	else {
		return multi_output_t<std::decay_t<Args>...>{ std::tie(args...) };
	}
}

#elif defined(CPP14_SUPPORTED) || defined(CPP11_SUPPORTED)
template <typename First, typename... Rest>
struct output_helper {
	static auto create(First& first, Rest&... rests)
		-> multi_output_t<First, Rest...> {
		return { std::tie(first, rests...) };
	}
};
template <typename First>
struct output_helper<First> {
	static auto create(First& first) -> output_t<First> {
		return { first };
	}
};
template <typename First, typename... Rest>
auto output(First& first, Rest&... rests)
-> decltype(output_helper<First, Rest...>::create(first, rests...)) {
	return output_helper<First, Rest...>::create(first, rests...);
}
#endif

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
#if defined(CPP20_SUPPORTED)
	template<size_t N, wrapper_concept... Args>
	static constexpr void call(const char(&name)[N], Args&&... args)
	{
		thread_local execute<N> executer(name);
		const auto [in, out] = params::sort(std::forward<Args>(args)...);
		executer.bind(in, out).write().wait_result().read();
	}
#elif defined(CPP17_SUPPORTED) || defined(CPP14_SUPPORTED) || defined(CPP11_SUPPORTED)
	template<size_t N, typename... Args>
	typename std::enable_if<is_wrapper_v<Args...>, void>::type
		static call(const char(&name)[N], Args&&... args) {
		thread_local execute<N> executer(name);
		const auto [in, out] = params::sort(std::forward<Args>(args)...);
		executer.bind(in, out).write().wait_result().read();
	}
#endif
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

	rpc::call("name", input(1, b), output(c), output(d, e), input(f)); // 发起调用

	rpc::call("name", input(a, b), output(c), output(d, e), input(f)); // 发起调用

	rpc::config().log_disable(); // 关闭日志

	rpc::config().performance(); // 开启性能统计

	rpc::call("name", input(a, b), output(c), output(d, e), input(f)); // 发起调用

	std::cout << rpc::config().info(); // 输出日志

	return 0;
}
