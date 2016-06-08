#include <windows.h>
#include <utils/DBLogger.h>
#include <utils/utils.h>
#include <utils/ErrorCodes.h>

DBLogger::DBLogger()
{
	try
	{
		m_active = false;
		ODBCParameters odbc = utils::getODBCParameters("datalog");
		m_cmd.reset(new command(odbc.m_host,
								odbc.m_port,
								odbc.m_database,
								odbc.m_uid,
								odbc.m_pwd));
	}
	catch (std::exception& ex)
	{
        throw Exception(errors::ER_MYSQL_ERROR, string("Error initializing connection to mysql. ")+ex.what());
	}
}

void	DBLogger::LD_LogEvent(int EventID, string EventInfo, int EventStatus)
{
	if (!m_active)		return;
	vector<db_variant>	params;
    string				username;

    username.resize(256,0);
    utils::getCurrentUserName(username);

	params.push_back(EventID);
	params.push_back(EventStatus);
	params.push_back(username);
    params.push_back(EventInfo);
    params.push_back(0);
    params.push_back(0);
	
	m_cmd->exec("INSERT INTO events (EventData, EventID, EventStatus, EventSysUserName, EventInfo, EvBatchKey, EvApplKey) VALUES (NOW(), ?, ?, ?, ?, ?, ?);",params);
}
