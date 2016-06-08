//#include <Windows.h>
#include <algorithm>
#include <sstream>
#include <database/mysql/command.h>
#include <database/mysql/column.hpp>
#include <utils/Exception.h>

/*	CONSTRUCTOR/DESTRUCTOR	*/
command::command(std::string host, int port, std::string db, std::string usr, std::string pwd)
	: m_con(0), m_stmt(0), m_autocommit(true)
{
	if (mysql_lib::singleton()->empty())	throw Exception(errors::ER_DLL_LOAD_ERROR, "MYSQL link error");
	m_con = mysql_lib::singleton()->p_mysql_init(0);

	if (!m_con)								throw Exception(errors::ER_MYSQL_ERROR, "Open MYSQL connection failed");
	if (!m_con)								throw Exception(errors::ER_MYSQL_ERROR, "Open connection failure");
	try
	{
		check(mysql_lib::singleton()->p_mysql_real_connect(m_con, host.c_str(), usr.c_str(), pwd.c_str(), db.c_str(), port, 0, CLIENT_MULTI_STATEMENTS) == m_con);
		check(mysql_lib::singleton()->p_mysql_set_character_set(m_con, "cp1251") == 0);
	}
	catch (const Exception& ex)
	{
		close_all();
		throw ex;
	}
}

command::~command()
{
	close_all();
}
//----------------------------------------------------

void	command::check(bool expr)
{
	if (expr)	return;

	std::string msg = mysql_lib::singleton()->p_mysql_error(m_con);
	if (msg.empty())	msg = "MySQL error";

	throw Exception(errors::ER_MYSQL_ERROR, msg);
}
//----------------------------------------------------


void command::close_stmt()
{
	if (!m_stmt)	return;

	m_binds.clear();
	m_cols.clear();

	MYSQL_STMT* stmt(0);	std::swap(stmt, m_stmt);

	mysql_lib::singleton()->p_mysql_stmt_close(stmt);
}
//----------------------------------------------------


void	command::close_all()
{
	m_autocommit = true;
	close_stmt();
	if (!m_con)		return;
	MYSQL* con(0);	std::swap(con, m_con);

	mysql_lib::singleton()->p_mysql_close(con);
}
//----------------------------------------------------


void command::exec(const std::string& sql, const std::vector<db_variant>& params)
{
	close_stmt();
	m_stmt = mysql_lib::singleton()->p_mysql_stmt_init(m_con);
	if (!m_stmt) throw Exception(errors::ER_MYSQL_ERROR, "Statement initialization failed");
	check(mysql_lib::singleton()->p_mysql_stmt_prepare(m_stmt, sql.c_str(), (unsigned long)sql.size()) == 0);

	std::vector<MYSQL_BIND> binds(params.size());
	if (!binds.empty())
	{
		memset(binds.data(), 0, binds.size() * sizeof(MYSQL_BIND));
		for (size_t i(0); i < params.size(); ++i)
			bind_param(params[i], binds[i]);
		check(mysql_lib::singleton()->p_mysql_stmt_bind_param(m_stmt, binds.data()) == 0);
	}

	check(mysql_lib::singleton()->p_mysql_stmt_execute(m_stmt) == 0);
}


//----------------------------------------------------
void	command::bind()
{
	unsigned int count;

	m_binds.clear();
	m_cols.clear();


	MYSQL_RES* res(mysql_lib::singleton()->p_mysql_stmt_result_metadata(m_stmt));
	check(res != 0);

	count = mysql_lib::singleton()->p_mysql_num_fields(res);
	m_binds.resize(count);
	memset(m_binds.data(), 0, m_binds.size() * sizeof(MYSQL_BIND));

	for (size_t i(0); i<count; ++i)
	{
		MYSQL_FIELD* field(mysql_lib::singleton()->p_mysql_fetch_field_direct(res, i));
		m_cols.push_back(bind_result_factory(field, m_binds[i]));
	}


	check(mysql_lib::singleton()->p_mysql_stmt_bind_result(m_stmt, m_binds.data()) == 0);



	mysql_lib::singleton()->p_mysql_free_result(res);
}


//----------------------------------------------------
bool command::fetch(std::vector<db_variant>& row)
{
	row.clear();
	if (!m_stmt)			return false;
	if (m_cols.empty())		bind();

	int rv = mysql_lib::singleton()->p_mysql_stmt_fetch(m_stmt);
	if (MYSQL_NO_DATA == rv) return false;

	row.resize(m_cols.size());
	for (size_t i(0); i < m_cols.size(); ++i)
	{
		check(m_cols[i].set_result(m_stmt, i, row[i]) == 0);
	}
	return true;
}
//----------------------------------------------------

void command::commit()
{
	exec("COMMIT");
}
//----------------------------------------------------

void command::rollback()
{
	exec("ROLLBACK");
}
//----------------------------------------------------

void command::begin()
{
	exec("START TRANSACTION");
}
//----------------------------------------------------
