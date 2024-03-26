#pragma once


#include <concepts>

//template<typename T>
//concept sortable = requires(T a, T b) {
//    { a < b } -> std::convertible_to<bool>;
//};


//// ʹ�ø���Լ��
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

// ���� sortable ����
template<typename T>
constexpr bool sortable = std::is_arithmetic_v<T>; // Ϊ��ʾ����࣬������� sortable �Ķ����ǻ��� T �Ƿ�Ϊ��������

// sort ����ģ�壬���ܿ�����Ĳ�������
template<typename T>
std::enable_if_t<sortable<T>>
sort(T& param) {
    // ����ʵ�������߼��������Ϊʾ�������� param ��һ���������������������������
    param.sort();
}

// Ҳ���԰������� sort ��������֧�ֲ�ͬ���͵Ĳ���
