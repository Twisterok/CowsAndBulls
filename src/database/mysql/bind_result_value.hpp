#ifndef __BIND_RESULT_VALUE__
#define __BIND_RESULT_VALUE__
#include <database/mysql/bind_result.hpp>

//-------------------------
//	Класс наследник bind_result.
//	Он предполагает работу с примитивными типами данных
//-------------------------
template <typename T, enum_field_types TypeID>
class bind_result_value : public bind_result
{
	//	Шаблон значения
	T	m_val;
public:

	//-------------------------------
	//		CONSTRUCTOR
	//-------------------------------
	bind_result_value(MYSQL_BIND& bind);

	//-------------------------------
	//	Methods
	//	Пушит данные из MY_SQL_BIND в db_variable
	//-------------------------------
	virtual int set_result(MYSQL_STMT* stmt, unsigned int col, db_variant& var);
};
//------------------------------------------------------------------------

//-------------------------------
//		CONSTRUCTOR
//-------------------------------
template <typename T, enum_field_types TypeID>
bind_result_value<T, TypeID>::bind_result_value(MYSQL_BIND& bind) : bind_result(bind), m_val(0)
{
	m_bind.buffer_type		= TypeID;
	m_bind.buffer			= (void*)&m_val;
	m_bind.buffer_length	= sizeof(T);
}
//------------------------------------------------------------------------


//--------------------------------------------------------
//	Function: set_result
//--------------------------------------------------------
template <typename T, enum_field_types TypeID>
int bind_result_value<T, TypeID>::set_result(MYSQL_STMT*, unsigned int, db_variant& var)
{
	if (m_is_null)	var = null_t();
	else			var = m_val;
	return 0;
}	//bind_result_value
//------------------------------------------------------------------------
#endif