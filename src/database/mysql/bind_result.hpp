#ifndef __BIND_RESULT__
#define __BIND_RESULT__

#include <database/mysql/mysql_lib.hpp>
#include <database/mysql/db_variant.h>

//---------------------------
//		Базовый класс
//----------------------------
class bind_result
{
protected:
	//	В этот bind MySQL будет пушить данные
	MYSQL_BIND&	m_bind;

	//	Для удобства, чтобы не вызывать (*m_bind.is_null).
	//	Тип my_bool, потому что в MYSQL_BIND лежит именно этот тип.
	my_bool		m_is_null;

public:
	//--------------------------------
	//		CONSTRUCTOR/DESTRUCTOR
	//	Инициализируем bind и, чтобы потом не возиться с указателями,
	//	цепляем переменную флаг m_is_null к бинду.
	//--------------------------------
	bind_result(MYSQL_BIND& bind) :	m_bind(bind),	m_is_null(0)  { m_bind.is_null = &m_is_null; }
	virtual ~bind_result()  {}

	//-----------------------------------------------
	//	Выделяет необходимую память под колонку,
	//	куда MySQL будет пушить данные.
	//-----------------------------------------------
	virtual int set_result(MYSQL_STMT* stmt, unsigned int col, db_variant& var) = 0;
}; // bind_result

#endif