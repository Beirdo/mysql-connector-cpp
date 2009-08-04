/*
   Copyright 2007 - 2008 MySQL AB, 2008 - 2009 Sun Microsystems, Inc.  All rights reserved.

   The MySQL Connector/C++ is licensed under the terms of the GPL
   <http://www.gnu.org/licenses/old-licenses/gpl-2.0.html>, like most
   MySQL Connectors. There are special exceptions to the terms and
   conditions of the GPL as it is applied to this software, see the
   FLOSS License Exception
   <http://www.mysql.com/about/legal/licensing/foss-exception.html>.
*/

#ifndef _MYSQL_UTIL_H_
#define _MYSQL_UTIL_H_

#include "mysql_private_iface.h"
#include <cppconn/config.h>
#include <cppconn/sqlstring.h>


#ifndef UL64
#ifdef _WIN32
#define UL64(x) x##ui64
#else
#define UL64(x) x##ULL
#endif
#endif

#ifndef L64
#ifdef _WIN32
#define L64(x) x##i64
#else
#define L64(x) x##LL
#endif
#endif


#ifndef _WIN32
#  ifndef HAVE_FUNCTION_STRTOLL
#    define strtoll(__a, __b, __c)  static_cast<long long>(sql::mysql::util::strtold((__a), NULL))
#    define HAVE_FUNCTION_STRTOLL 1
#  endif
#  ifndef HAVE_FUNCTION_STRTOULL
#    define strtoull(__a, __b, __c)  static_cast<unsigned long long>(sql::mysql::util::strtold((__a), NULL))
#    define HAVE_FUNCTION_STRTOULL 1
#  endif
#else
#  define strtoll(x, e, b) _strtoi64((x), (e), (b))
#  define strtoull(x, e, b) _strtoui64((x), (e), (b))
#endif	//	_WIN32


namespace sql {
namespace mysql {
namespace NativeAPI {
class IMySQLCAPI;
}
namespace util {

void throwSQLException(::sql::mysql::NativeAPI::IMySQLCAPI & capi, MYSQL * mysql);
void throwSQLException(::sql::mysql::NativeAPI::IMySQLCAPI & capi, MYSQL_STMT * stmt);

int mysql_string_type_to_datatype(const sql::SQLString & name);
int mysql_type_to_datatype(const MYSQL_FIELD * const field);
const char * mysql_type_to_string(const MYSQL_FIELD * const field);

char * utf8_strup(const char * const src, size_t srclen);

long double strtold(const char *nptr, char **endptr);

typedef struct st_our_charset
{
	unsigned int	nr;
	const char		*name;
	const char		*collation;
	unsigned int	char_minlen;
	unsigned int	char_maxlen;
	const char		*comment;
	unsigned int 	(*mb_charlen)(unsigned int c);
	unsigned int 	(*mb_valid)(const char *start, const char *end);
} OUR_CHARSET;

const OUR_CHARSET * find_charset(unsigned int charsetnr);


} /* namespace util */
} /* namespace mysql */
} /* namespace sql */

#endif /* _MYSQL_UTIL_H_ */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
