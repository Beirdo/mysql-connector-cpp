/*
   Copyright (C) 2007 - 2008 MySQL AB, 2008 - 2009 Sun Microsystems, Inc.

   The MySQL Connector/C++ is licensed under the terms of the GPL
   <http://www.gnu.org/licenses/old-licenses/gpl-2.0.html>, like most
   MySQL Connectors. There are special exceptions to the terms and
   conditions of the GPL as it is applied to this software, see the
   FLOSS License Exception
   <http://www.mysql.com/about/legal/licensing/foss-exception.html>.
 */

#include "PreparedStatementTest.h"

namespace testsuite
{
namespace regression
{

  void PreparedStatementTest::testStmtClose()
  {
    pstmt.reset( conn->prepareStatement( "select ?" ) );

    pstmt->setString( 1, "dummy" );

    pstmt->executeQuery();

    pstmt->close();

    // Application crashed on the destructor if close was closed before.
    // prepared statement had to contain any parameter.
    pstmt.reset();
  }


  void PreparedStatementTest::setUp()
  {
    super::setUp();

    dbmd.reset(conn->getMetaData());
  }
}
}
