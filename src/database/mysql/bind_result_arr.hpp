#ifndef __BIND_RESULT_ARR__
#define __BIND_RESULT_ARR__
#include <database/mysql/bind_result.hpp>

//-------------------------
//	Класс наследник bind_result.
//	Он предполагает работу с такими типами данных как 
//	blob/string (std::vector по сути).
//-------------------------
template <typename T, enum_field_types TypeID>
class bind_result_arr : public bind_result
{
	T				m_arr;		//	Шаблон буфера
	unsigned long	m_lenght;	//	Длина выходного буфера
public:
	//-------------------------------
	//		CONSTRUCTOR
	//-------------------------------
	bind_result_arr(MYSQL_BIND& bind);

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
		/*	Если текущего размера буффера не хватает для записи в него,						/
		/	то выделяем необходимое кол-во памяти. (m_length уже заполена во время fetch)	*/
		if (m_bind.buffer_length < m_lenght)	m_arr.resize(m_lenght);

		
		/*	Перекидываем указатель MYSQL_BIND на наш буффер.	/
		/	Именно туда будут впоследствии записаны данные		*/
		m_bind.buffer			= (void*)m_arr.data();
		m_bind.buffer_length	= m_lenght;

		/*	Заполняем буффер данными из MY_SQL */
		rv	= mysql_lib::singleton()->p_mysql_stmt_fetch_column(stmt, &m_bind, col, 0);

		/*	Обрезаем указатель	*/
		m_bind.buffer			= 0;
		m_bind.buffer_length	= 0;

		if (rv == 0)
		{
			/*	Если данные хорошо записались, то кладем их в db_variable	/
			/	Создаем db_variable и цепляем к ней буффер					*/
			var		= T();
			T& arr	= boost::get<T>(var);


			/* Устанавливаем необходимый размер буффера и копируем в него данные из m_arr, в котором лежат данные	*/
			arr.resize(size_t(m_lenght));
			if (!arr.empty())		memcpy((void*)arr.data(), m_arr.data(), arr.size());
		}
	}
	return rv;
} // bind_result_arr::
//------------------------------------------------------------------------
#endif