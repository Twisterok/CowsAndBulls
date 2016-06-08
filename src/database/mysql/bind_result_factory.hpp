#ifndef __BIND_FACTORY__
#define __BIND_FACTORY__
#include <database\mysql\bind_result.hpp>
#include <database\mysql\bind_result_arr.hpp>
#include <database\mysql\bind_result_value.hpp>
#include <utils/ErrorCodes.h>

/*	Фарика по выделению нужных типов данные bind_result	*/
inline bind_result* bind_result_factory(MYSQL_FIELD* field, MYSQL_BIND& bind)
{
  switch (field->type)
  {  
  case MYSQL_TYPE_TINY:
  case MYSQL_TYPE_SHORT:		return new bind_result_value<int16_t, MYSQL_TYPE_SHORT>(bind);
  case MYSQL_TYPE_INT24:
  case MYSQL_TYPE_LONG:			return new bind_result_value<int32_t, MYSQL_TYPE_LONG>(bind);
  case MYSQL_TYPE_LONGLONG:		return new bind_result_value<int64_t, MYSQL_TYPE_LONGLONG>(bind);
  case MYSQL_TYPE_FLOAT:		return new bind_result_value<float, MYSQL_TYPE_FLOAT>(bind);
  case MYSQL_TYPE_DECIMAL:
  case MYSQL_TYPE_NEWDECIMAL:
  case MYSQL_TYPE_DOUBLE:		return new bind_result_value<double, MYSQL_TYPE_DOUBLE>(bind);

  case MYSQL_TYPE_DATE:
  case MYSQL_TYPE_DATETIME:
  case MYSQL_TYPE_NEWDATE:
  case MYSQL_TYPE_TIME:
  case MYSQL_TYPE_TIMESTAMP:
  case MYSQL_TYPE_YEAR:

  case MYSQL_TYPE_STRING:
  case MYSQL_TYPE_VAR_STRING:
  case MYSQL_TYPE_VARCHAR:		return new bind_result_arr<std::string, MYSQL_TYPE_STRING>(bind);

  case MYSQL_TYPE_BLOB:
  case MYSQL_TYPE_LONG_BLOB:
  case MYSQL_TYPE_MEDIUM_BLOB:
  case MYSQL_TYPE_TINY_BLOB:	return new bind_result_arr<blob_t, MYSQL_TYPE_BLOB>(bind);

  default: throw Exception(errors::ER_MYSQL_ERROR, "MySQL type error");
  }
}
#endif