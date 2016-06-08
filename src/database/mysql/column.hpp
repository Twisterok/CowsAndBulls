#ifndef __COLUMN_HPP__
#define __COLUMN_HPP__
#include <string>
#include <vector>
#include <mysql.h>
#include <boost/variant.hpp>
#include <boost/none_t.hpp>
#include <database/mysql/db_variant.h>

/*	Функтор, который сам разбирается какое ему значение пришло в db_variable */
struct bind_param_visitor : boost::static_visitor<void> {
  MYSQL_BIND& b;
  explicit bind_param_visitor(MYSQL_BIND& b_) : b(b_)  {}
  void operator()(const null_t&)		const  { b.buffer_type = MYSQL_TYPE_NULL;}
  void operator()(const int16_t& r)		const  { b.buffer_type = MYSQL_TYPE_SHORT;		b.buffer = (char*)&r; }
  void operator()(const int32_t& r)		const  { b.buffer_type = MYSQL_TYPE_LONG;		b.buffer = (char*)&r; }
  void operator()(const int64_t& r)		const  { b.buffer_type = MYSQL_TYPE_LONGLONG;	b.buffer = (char*)&r; }
  void operator()(const float& r)		const  { b.buffer_type = MYSQL_TYPE_FLOAT;		b.buffer = (char*)&r; }
  void operator()(const double& r)		const  { b.buffer_type = MYSQL_TYPE_DOUBLE;		b.buffer = (char*)&r; }
  void operator()(const std::string& r) const  { b.buffer_type = MYSQL_TYPE_STRING;		b.buffer = (char*)r.c_str(); b.buffer_length = (unsigned long)r.size(); }
  void operator()(const blob_t& r)		const  { b.buffer_type = MYSQL_TYPE_BLOB;		b.buffer = (char*)r.data(); b.buffer_length = (unsigned long)r.size(); }
}; // bind_param_visitor
//-------------------------------------------------------------------

/*	Проставляет в MYSQL_BIND нужные тип и значение в зависимости	/
/	от значения, которое лежит в db_variable						*/
inline void bind_param(const db_variant& param, MYSQL_BIND& bind)
{
  bind_param_visitor visitor(bind);
  return boost::apply_visitor(visitor, param);	
}
//-------------------------------------------------------------------
#endif