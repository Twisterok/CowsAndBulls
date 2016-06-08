#ifndef __BIND_RESULT_ARR__
#define __BIND_RESULT_ARR__
#include <database/mysql/bind_result.hpp>

//-------------------------
//	����� ��������� bind_result.
//	�� ������������ ������ � ������ ������ ������ ��� 
//	blob/string (std::vector �� ����).
//-------------------------
template <typename T, enum_field_types TypeID>
class bind_result_arr : public bind_result
{
	T				m_arr;		//	������ ������
	unsigned long	m_lenght;	//	����� ��������� ������
public:
	//-------------------------------
	//		CONSTRUCTOR
	//-------------------------------
	bind_result_arr(MYSQL_BIND& bind);

	//-------------------------------
	//	Methods
	//	����� ������ �� MY_SQL_BIND � db_variable
	//-------------------------------
	virtual int set_result(MYSQL_STMT* stmt, unsigned int col, db_variant& var);
};
//------------------------------------------------------------------------



//-------------------------------
//		CONSTRUCTOR
//-------------------------------
template <typename T, enum_field_types TypeID>
bind_result_arr<T, TypeID>::bind_result_arr(MYSQL_BIND& bind) :	bind_result(bind),
																m_lenght(0)
{	
	m_bind.buffer_type	= TypeID;
	m_bind.length		= &m_lenght;
}
//------------------------------------------------------------------------




//--------------------------------------------------------
//	Function: set_result
//--------------------------------------------------------
template <typename T, enum_field_types TypeID>
int bind_result_arr<T, TypeID>::set_result(MYSQL_STMT* stmt, unsigned int col, db_variant& var)
{
	int rv = 0;

	if (m_is_null)	var = null_t();
	else
	{
		/*	���� �������� ������� ������� �� ������� ��� ������ � ����,						/
		/	�� �������� ����������� ���-�� ������. (m_length ��� �������� �� ����� fetch)	*/
		if (m_bind.buffer_length < m_lenght)	m_arr.resize(m_lenght);

		
		/*	������������ ��������� MYSQL_BIND �� ��� ������.	/
		/	������ ���� ����� ������������ �������� ������		*/
		m_bind.buffer			= (void*)m_arr.data();
		m_bind.buffer_length	= m_lenght;

		/*	��������� ������ ������� �� MY_SQL */
		rv	= mysql_lib::singleton()->p_mysql_stmt_fetch_column(stmt, &m_bind, col, 0);

		/*	�������� ���������	*/
		m_bind.buffer			= 0;
		m_bind.buffer_length	= 0;

		if (rv == 0)
		{
			/*	���� ������ ������ ����������, �� ������ �� � db_variable	/
			/	������� db_variable � ������� � ��� ������					*/
			var		= T();
			T& arr	= boost::get<T>(var);


			/* ������������� ����������� ������ ������� � �������� � ���� ������ �� m_arr, � ������� ����� ������	*/
			arr.resize(size_t(m_lenght));
			if (!arr.empty())		memcpy((void*)arr.data(), m_arr.data(), arr.size());
		}
	}
	return rv;
} // bind_result_arr::
//------------------------------------------------------------------------
#endif