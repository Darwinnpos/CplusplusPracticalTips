#pragma once


#include <concepts>

//template<typename T>
//concept sortable = requires(T a, T b) {
//    { a < b } -> std::convertible_to<bool>;
//};


//// 使用概念约束
//template<typename T>       // Correct but verbose: "The parameter is
//    requires sortable<T>   // of type T which is the name of a type
//void sort(T&)           // that is sortable"
//{
//	
//}

//template<sortable T>       // Better: "The parameter is of type T
//void sort(T&)           // which is Sortable"
//{
//	
//}
//
//void sort(sortable auto&) // Best: "The parameter is Sortable"
//{
//	
//}

#include <type_traits>

// 定义 sortable 概念
template<typename T>
constexpr bool sortable = std::is_arithmetic_v<T>; // 为了示例简洁，这里假设 sortable 的定义是基于 T 是否为算术类型

// sort 函数模板，接受可排序的参数引用
template<typename T>
std::enable_if_t<sortable<T>>
sort(T& param) {
    // 这里实现排序逻辑，这里仅为示例，假设 param 是一个可排序的容器，调用其排序函数
    param.sort();
}

// 也可以按需重载 sort 函数，以支持不同类型的参数
