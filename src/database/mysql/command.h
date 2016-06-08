#ifndef __MYSQL_COMMAND__
#define __MYSQL_COMMAND__

#include <string>
#include <mysql.h>
#include <boost/ptr_container/ptr_vector.hpp>
#include <database/mysql/mysql_lib.hpp>
#include <database/mysql/bind_result_factory.hpp>

/*	Класс работы с БД MySQL	*/
class command
{
	MYSQL*							m_con;			
	MYSQL_STMT*						m_stmt;			
	std::vector<MYSQL_BIND>			m_binds;			
	bool							m_autocommit;
	boost::ptr_vector<bind_result>	m_cols;
public:
	/*	CONSTRUCTOR/DESTRUCTOR	*/
	command(std::string host, int port, std::string db, std::string usr, std::string pwd);
	~command();
	
	/*	METHODS	*/
	void	check(bool expr);
	void	close_all();
	void	close_stmt();
	void	bind();    
	void	exec(const std::string& sql, const std::vector<db_variant>& params = std::vector<db_variant>());
	bool	fetch(std::vector<db_variant>& row);
	void	set_autocommit(bool autocommit)	{ m_autocommit = autocommit; }
	void	begin();
	void	commit();
	void	rollback();
};
#endif
