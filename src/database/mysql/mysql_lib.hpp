#ifndef	__MYSQL_LIB_HPP__
#define __MYSQL_LIB_HPP__

#include <Windows.h>
#include <algorithm>
#include <string>
#include <mysql.h>
#include <utils/definitions.h>
#include <utils/Exception.h>
#include <utils/ErrorCodes.h>
/*----------------------/
/		C++11			/
/----------------------*/
using namespace std;
#define MYSQL_LIB_PATH			"libmysql.dll"			// Путь к libMysql.
#define	DL_LIBRARY(path)		LoadLibraryA(path);		
#define	DL_FUNCTION(lib, fun)	(decltype(fun)*)GetProcAddress(lib, STRINGIFY(fun));


class mysql_lib 
{
	/*	Конструкторы приватные, чтобы использовать только singleton	*/
	mysql_lib();
	~mysql_lib();

	string						m_dll_path;	
	HINSTANCE					m_lib;
public:
	decltype(mysql_close)					*p_mysql_close;
	decltype(mysql_error)					*p_mysql_error;
	decltype(mysql_fetch_field_direct)		*p_mysql_fetch_field_direct;
	decltype(mysql_free_result)				*p_mysql_free_result;
	decltype(mysql_get_client_info)			*p_mysql_get_client_info;
	decltype(mysql_init)					*p_mysql_init;
	decltype(mysql_next_result)				*p_mysql_next_result;
	decltype(mysql_num_fields)				*p_mysql_num_fields;
	decltype(mysql_query)					*p_mysql_query;
	decltype(mysql_real_connect)			*p_mysql_real_connect;
	decltype(mysql_set_character_set)		*p_mysql_set_character_set;
	decltype(mysql_stmt_bind_param)			*p_mysql_stmt_bind_param;
	decltype(mysql_stmt_bind_result)		*p_mysql_stmt_bind_result;
	decltype(mysql_stmt_close)				*p_mysql_stmt_close;
	decltype(mysql_stmt_execute)			*p_mysql_stmt_execute;
	decltype(mysql_stmt_fetch)				*p_mysql_stmt_fetch;
	decltype(mysql_stmt_fetch_column)		*p_mysql_stmt_fetch_column;
	decltype(mysql_stmt_init)				*p_mysql_stmt_init;
	decltype(mysql_stmt_prepare)			*p_mysql_stmt_prepare;
	decltype(mysql_stmt_result_metadata)	*p_mysql_stmt_result_metadata;
	decltype(mysql_store_result)			*p_mysql_store_result;
	
	/*	При вызове конструктора p_mysql_store_result устанавливается в 0.	/
	/	Поэтому, если она 0 - это означает что не все функции подтянулись	*/
	bool				empty() const  { return p_mysql_store_result == 0; }
	static mysql_lib*	singleton();
}; // lib

inline mysql_lib::mysql_lib() : p_mysql_store_result(0)
{	
	m_dll_path						=	MYSQL_LIB_PATH;
	m_lib							=	DL_LIBRARY(m_dll_path.c_str());
	if (!m_lib)						throw Exception(errors::ER_DLL_LOAD_ERROR,m_dll_path+": file not found");

	p_mysql_close					= DL_FUNCTION(m_lib,mysql_close);
	p_mysql_error					= DL_FUNCTION(m_lib,mysql_error);
	p_mysql_fetch_field_direct		= DL_FUNCTION(m_lib,mysql_fetch_field_direct);
	p_mysql_free_result				= DL_FUNCTION(m_lib,mysql_free_result);


	p_mysql_free_result				= DL_FUNCTION(m_lib,mysql_free_result)				
	p_mysql_get_client_info			= DL_FUNCTION(m_lib,mysql_get_client_info)			
	p_mysql_init					= DL_FUNCTION(m_lib,mysql_init)						
	p_mysql_next_result				= DL_FUNCTION(m_lib,mysql_next_result)				
	p_mysql_num_fields				= DL_FUNCTION(m_lib,mysql_num_fields)				
	p_mysql_query					= DL_FUNCTION(m_lib,mysql_query)					
	p_mysql_real_connect			= DL_FUNCTION(m_lib,mysql_real_connect)				
	p_mysql_set_character_set		= DL_FUNCTION(m_lib,mysql_set_character_set)		
	p_mysql_stmt_bind_param			= DL_FUNCTION(m_lib,mysql_stmt_bind_param)			
	p_mysql_stmt_bind_result		= DL_FUNCTION(m_lib,mysql_stmt_bind_result)			
	p_mysql_stmt_close				= DL_FUNCTION(m_lib,mysql_stmt_close)				
	p_mysql_stmt_execute			= DL_FUNCTION(m_lib,mysql_stmt_execute)				
	p_mysql_stmt_fetch				= DL_FUNCTION(m_lib,mysql_stmt_fetch)				
	p_mysql_stmt_fetch_column		= DL_FUNCTION(m_lib,mysql_stmt_fetch_column)		
	p_mysql_stmt_init				= DL_FUNCTION(m_lib,mysql_stmt_init)				
	p_mysql_stmt_prepare			= DL_FUNCTION(m_lib,mysql_stmt_prepare)				
	p_mysql_stmt_result_metadata	= DL_FUNCTION(m_lib,mysql_stmt_result_metadata)		
	p_mysql_store_result			= DL_FUNCTION(m_lib,mysql_store_result)				


	if (!p_mysql_close					||	
		!p_mysql_error					||	
		!p_mysql_fetch_field_direct		||
		!p_mysql_free_result			||	
		!p_mysql_get_client_info		||
		!p_mysql_init					||
		!p_mysql_next_result			||
		!p_mysql_num_fields				||
		!p_mysql_query					||
		!p_mysql_real_connect			||
		!p_mysql_set_character_set		||
		!p_mysql_stmt_bind_param		||
		!p_mysql_stmt_bind_result		||
		!p_mysql_stmt_close				||
		!p_mysql_stmt_execute			||
		!p_mysql_stmt_fetch				||
		!p_mysql_stmt_fetch_column		||
		!p_mysql_stmt_init				||
		!p_mysql_stmt_prepare			||
		!p_mysql_stmt_result_metadata	||
		!p_mysql_store_result)
	{
		throw Exception(errors::ER_DLL_LOAD_ERROR ,m_dll_path+": Loading functions failure");
	}

}

inline mysql_lib::~mysql_lib()
{
	if (m_lib)
	{
		FreeLibrary(m_lib);
		m_lib		= NULL;
	}
}

inline mysql_lib*	mysql_lib::singleton()
{
	static mysql_lib s_instance;
	return &s_instance;
}
#endif