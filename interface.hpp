#pragma once
#include <cstdint>

// �Թ���ӿڴ����������ǿ��Լ��
typedef class c_uint32
{
public:
	explicit c_uint32(const uint32_t ui_data):m_ui_data_(ui_data){}
	uint32_t operator()()const { return m_ui_data_; }
private:
	uint32_t m_ui_data_;
}month, year, day;

// �Թ������ǿ��Լ��
class data
{
public:
	data(month,year,day)
	{
		
	}
};

// ���ڹ������ʹ��������ʽ������ⷸ��
// data(month(month),day(day),year(year))

// һ��ֵ�����۵����⣿�Ƿ����㿪������
