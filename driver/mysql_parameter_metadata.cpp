/*
   Copyright (C) 2007 - 2008 MySQL AB, 2008 - 2009 Sun Microsystems, Inc.

   The MySQL Connector/C++ is licensed under the terms of the GPL
   <http://www.gnu.org/licenses/old-licenses/gpl-2.0.html>, like most
   MySQL Connectors. There are special exceptions to the terms and
   conditions of the GPL as it is applied to this software, see the
   FLOSS License Exception
   <http://www.mysql.com/about/legal/licensing/foss-exception.html>.
*/

#ifndef _WIN32
#include <stdlib.h>
#endif	//	_WIN32

#include "mysql_parameter_metadata.h"
#include <cppconn/exception.h>

namespace sql
{
namespace mysql
{


/* {{{ MySQL_ParameterMetaData::MySQL_ParameterMetaData -I- */
MySQL_ParameterMetaData::MySQL_ParameterMetaData(const MYSQL_STMT * const stmt)
{
	param_count = mysql_stmt_param_count(const_cast<MYSQL_STMT *>(stmt));
}
/* }}} */


/* {{{ MySQL_ParameterMetaData::getParameterClassName -U- */
std::string
MySQL_ParameterMetaData::getParameterClassName(unsigned int /* paramNo */)
{
	throw sql::MethodNotImplementedException("MySQL_ParameterMetaData::getParameterClassName()");
	return ""; // fool compilers
}
/* }}} */


/* {{{ MySQL_ParameterMetaData::getParameterCount -I- */
int
MySQL_ParameterMetaData::getParameterCount()
{
	return param_count;
}
/* }}} */


/* {{{ MySQL_ParameterMetaData::getParameterMode -U- */
int
MySQL_ParameterMetaData::getParameterMode(unsigned int /* paramNo */)
{
	throw sql::MethodNotImplementedException("MySQL_ParameterMetaData::getParameterClassName()");
	return 0; // fool compilers
}
/* }}} */


/* {{{ MySQL_ParameterMetaData::getParameterType -U- */
int
MySQL_ParameterMetaData::getParameterType(unsigned int /* paramNo */)
{
	throw sql::MethodNotImplementedException("MySQL_ParameterMetaData::getParameterClassName()");
	return 0; // fool compilers
}
/* }}} */


/* {{{ MySQL_ParameterMetaData::getParameterTypeName -U- */
std::string
MySQL_ParameterMetaData::getParameterTypeName(unsigned int /* paramNo */)
{
	throw sql::MethodNotImplementedException("MySQL_ParameterMetaData::getParameterClassName()");
	return 0; // fool compilers
}
/* }}} */


/* {{{ MySQL_ParameterMetaData::getPrecision -U- */
int
MySQL_ParameterMetaData::getPrecision(unsigned int /* paramNo */)
{
	throw sql::MethodNotImplementedException("MySQL_ParameterMetaData::getParameterClassName()");
	return 0; // fool compilers
}
/* }}} */


/* {{{ MySQL_ParameterMetaData::getScale -U- */
int
MySQL_ParameterMetaData::getScale(unsigned int /* paramNo */)
{
	throw sql::MethodNotImplementedException("MySQL_ParameterMetaData::getParameterClassName()");
	return 0; // fool compilers
}
/* }}} */


/* {{{ MySQL_ParameterMetaData::isNullable -U- */
int
MySQL_ParameterMetaData::isNullable(unsigned int /* paramNo */)
{
	throw sql::MethodNotImplementedException("MySQL_ParameterMetaData::getParameterClassName()");
	return 0; // fool compilers
}
/* }}} */


/* {{{ MySQL_ParameterMetaData::isSigned -U- */
bool
MySQL_ParameterMetaData::isSigned(unsigned int /* paramNo */)
{
	throw sql::MethodNotImplementedException("MySQL_ParameterMetaData::getParameterClassName()");
	return 0; // fool compilers
}
/* }}} */

}; /* namespace mysql */
}; /* namespace sql */
/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */


