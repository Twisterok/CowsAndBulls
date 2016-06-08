#ifndef __BIND_RESULT__
#define __BIND_RESULT__

#include <database/mysql/mysql_lib.hpp>
#include <database/mysql/db_variant.h>

//---------------------------
//		������� �����
//----------------------------
class bind_result
{
protected:
	//	� ���� bind MySQL ����� ������ ������
	MYSQL_BIND&	m_bind;

	//	��� ��������, ����� �� �������� (*m_bind.is_null).
	//	��� my_bool, ������ ��� � MYSQL_BIND ����� ������ ���� ���.
	my_bool		m_is_null;

public:
	//--------------------------------
	//		CONSTRUCTOR/DESTRUCTOR
	//	�������������� bind �, ����� ����� �� �������� � �����������,
	//	������� ���������� ���� m_is_null � �����.
	//--------------------------------
	bind_result(MYSQL_BIND& bind) :	m_bind(bind),	m_is_null(0)  { m_bind.is_null = &m_is_null; }
	virtual ~bind_result()  {}

	//-----------------------------------------------
	//	�������� ����������� ������ ��� �������,
	//	���� MySQL ����� ������ ������.
	//-----------------------------------------------
	virtual int set_result(MYSQL_STMT* stmt, unsigned int col, db_variant& var) = 0;
}; // bind_result

#endif