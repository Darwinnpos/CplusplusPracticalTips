#pragma once
#include <cstdint>

// 对构造接口传入参数进行强制约束
typedef class c_uint32
{
public:
	explicit c_uint32(const uint32_t ui_data):m_ui_data_(ui_data){}
	uint32_t operator()()const { return m_ui_data_; }
private:
	uint32_t m_ui_data_;
}month, year, day;

// 对构造进行强制约束
class data
{
public:
	data(month,year,day)
	{
		
	}
};

// 对于构造必须使用如下形式，则避免犯错
// data(month(month),day(day),year(year))

// 一个值得讨论的问题？是否是零开销抽象
