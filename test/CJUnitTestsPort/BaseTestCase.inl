/* Copyright (C) 2007 - 2008 MySQL AB, 2008 Sun Microsystems, Inc.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; version 2 of the License.

There are special exceptions to the terms and conditions of the GPL
as it is applied to this software. View the full text of the
exception in file EXCEPTIONS-CONNECTOR-C++ in the directory of this
software distribution.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <stdlib.h>

namespace testsuite
{
  /**
	 * Creates a new BaseTestFixture object.
	 *
	 * @param name
	 *            The name of the JUnit test case
	 */

  template <typename ConcreteTestFixture>
  BaseTestFixture<ConcreteTestFixture>::BaseTestFixture( String name )
                        : super             ( name          ),
                          TestFixtureCommon ( defaultHost
                                            , defaultLogin
                                            , defaultPasswd
                                            , defaultDb     ),

                          myInstanceNumber  ( 0             ),
                          conn              ( NULL          ),
                          pstmt             ( NULL          ),
                          stmt              ( NULL          ),
                          rs                ( NULL          ),
                        /*dbClass           ( "sql::mysql::Driver"),*/
                          hasSps            ( true          )

  {
    this->myInstanceNumber = instanceCount++;
  }


  template <typename ConcreteTestFixture>
  BaseTestFixture<ConcreteTestFixture>::BaseTestFixture( const String & name
                                                      ,  const String & dbUrl
                                                      ,  const String & dbUser
                                                      ,  const String & dbPasswd
                                                      ,  const String & dbSchema
                                                       )
                        : super             ( name          ),
                          TestFixtureCommon ( dbUrl
                                            , dbUser
                                            , dbPasswd
                                            , dbSchema     ),

                          myInstanceNumber  ( 0             ),
                          conn              ( NULL          ),
                          pstmt             ( NULL          ),
                          stmt              ( NULL          ),
                          rs                ( NULL          ),
                        /*dbClass           ( "sql::mysql::Driver"),*/
                          hasSps            ( true          )

  {
    this->myInstanceNumber = instanceCount++;
  }


  /* throws SQLException & */
  template <typename ConcreteTestFixture>
  void BaseTestFixture<ConcreteTestFixture>::createSchemaObject(String objectType, String objectName,
			String columnsAndOtherStuff)
  {
    this->createdObjects.push_back(objectType);
    this->createdObjects.push_back(objectName);

    dropSchemaObject(objectType, objectName);
    String createSql( _T("CREATE  ") );

    createSql.resize(objectName.length()
        + objectType.length() + columnsAndOtherStuff.length() + 10);

    createSql.append(objectType);
    createSql.append(" ");
    createSql.append(objectName);
    createSql.append(" ");
    createSql.append(columnsAndOtherStuff);
    this->stmt->executeUpdate(createSql);
  }


/* throws SQLException & */
  template<typename ConcreteTestFixture>
  void BaseTestFixture<ConcreteTestFixture>::createFunction(String functionName, String functionDefn)
  {
    createSchemaObject("FUNCTION", functionName, functionDefn);
  }


/* throws SQLException & */
  template<typename ConcreteTestFixture>
  void BaseTestFixture<ConcreteTestFixture>::dropFunction(String functionName)
  {
    dropSchemaObject("FUNCTION", functionName);
  }


/* throws SQLException & */
  template<typename ConcreteTestFixture>
  void BaseTestFixture<ConcreteTestFixture>::createProcedure(String procedureName, String procedureDefn)
  {
    createSchemaObject("PROCEDURE", procedureName, procedureDefn);
  }


/* throws SQLException & */
  template<typename ConcreteTestFixture>
  void BaseTestFixture<ConcreteTestFixture>::dropProcedure(String procedureName)
  {
    dropSchemaObject("PROCEDURE", procedureName);
  }


/* throws SQLException & */
  template<typename ConcreteTestFixture>
  void BaseTestFixture<ConcreteTestFixture>::createTable(String tableName, String columnsAndOtherStuff)
  {
    createSchemaObject("TABLE", tableName, columnsAndOtherStuff);
  }


/* throws SQLException & */
  template<typename ConcreteTestFixture>
  void BaseTestFixture<ConcreteTestFixture>::dropTable(String tableName)
  {
    dropSchemaObject("TABLE", tableName);
  }


/* throws SQLException & */
  template<typename ConcreteTestFixture>
  void BaseTestFixture<ConcreteTestFixture>::dropSchemaObject(String objectType, String objectName)
  {
    this->stmt->executeUpdate("DROP " + objectType + " IF EXISTS "
        + objectName);
  }


/* throws SQLException & */
  template<typename ConcreteTestFixture>
  Connection BaseTestFixture<ConcreteTestFixture>::getAdminConnection()
  {
    return getAdminConnectionWithProps(Properties());
  }


  /* throws SQLException & */
  template <typename ConcreteTestFixture>
  Connection BaseTestFixture<ConcreteTestFixture>::getAdminConnectionWithProps(Properties props)
  {
    //String adminUrl = System.getProperty(ADMIN_CONNECTION_PROPERTY_NAME);
    if (driver)//adminUrl != NULL)`
    {
      return driver->connect(host, /*port,*/ login, passwd); //adminUrl, props);
    }
    else
    {
      return NULL;
    }
  }


  /* getConnectionWith Props are legacy. Currently not used. Require changes in order to work */
  /* throws SQLException & */
  template <typename ConcreteTestFixture>
  sql::Connection * BaseTestFixture<ConcreteTestFixture>::getConnectionWithProps(const String & propsList)
  {
    return getConnectionWithProps( host, propsList);
  }


  /* throws SQLException & */
  template <typename ConcreteTestFixture>
  sql::Connection * BaseTestFixture<ConcreteTestFixture>::getConnectionWithProps(const String & url
                                                                 , const String & propsList)
  {
    Properties props;

    List keyValuePairs;
    StringUtils::split(keyValuePairs, propsList, _T(","), false);

    for ( Iterator iter = keyValuePairs.begin();iter != keyValuePairs.end(); ++iter )
    {
      String kvp = *iter;
      List splitUp;
      StringUtils::split( splitUp, kvp, _T("="), false);
      String value;

      for (int i = 1; i < splitUp.size(); i++)
      {
        if (i != 1)
        {
          value.append( _T( "=" ) );
        }
        value.append( splitUp[ i ] );
      }
      props[ StringUtils::trim( splitUp[ 0 ] ) ] =  value;
    }

    return getConnectionWithProps(url, props);
  }

  /**
	 * Returns a new connection with the given properties
	 *
	 * @param props
	 *            the properties to use (the URL will come from the standard for
	 *            this testcase).
	 *
	 * @return a new connection using the given properties.
	 *
	 * @throws SQLException &
	 *             DOCUMENT ME!
	 */

  /* throws SQLException & */
  template <typename ConcreteTestFixture>
  sql::Connection * BaseTestFixture<ConcreteTestFixture>::getConnectionWithProps(const Properties & props)
  {
    return driver ? driver->connect(host, /*port,*/ login, passwd) : NULL;//dbUrl, props);
  }


  /* throws SQLException & */
  template <typename ConcreteTestFixture>
  sql::Connection * BaseTestFixture<ConcreteTestFixture>::getConnectionWithProps(const String & url, const Properties & props)
  {
    return driver ? driver->connect(host, /*port,*/ login, passwd) : NULL;//dbUrl, props);
  }

  /**
	 * Returns the per-instance counter (for messages when multi-threading
	 * stress tests)
	 *
	 * @return int the instance number
	 */
  template <typename ConcreteTestFixture>
  int BaseTestFixture<ConcreteTestFixture>::getInstanceNumber()
  {
    return this->myInstanceNumber;
  }


  /* throws SQLException & */
  template <typename ConcreteTestFixture>
  String BaseTestFixture<ConcreteTestFixture>::getMysqlVariable(Connection & c
                                                  , const String & variableName)
  {
    Object value = getSingleIndexedValueWithQuery(c, 2,
        "SHOW VARIABLES LIKE '" + variableName + "'");
    if (value.get() != NULL)
    {
      if (value->instanceof( value_object::vtByte ) )
      {
        // workaround for bad 4.1.x bugfix  - c/j legacy comment
        return value->toString();//new String((byte[]) value);
      }
    }

    return value->toString();
  }

  /**
	 * Returns the named MySQL variable from the currently connected server.
	 *
	 * @param variableName
	 *            the name of the variable to return
	 *
	 * @return the value of the given variable, or NULL if it doesn't exist
	 *
	 * @throws SQLException &
	 *             if an error occurs
	 */

  /* throws SQLException & */
  template <typename ConcreteTestFixture>
  String BaseTestFixture<ConcreteTestFixture>::getMysqlVariable(const String & variableName)
  {
    return getMysqlVariable(this->conn, variableName);
  }

  /**
	 * Returns the properties that represent the default URL used for
	 * connections for all testcases.
	 *
	 * @return properties parsed from sql::mysql::testsuite.url
	 *
	 * @throws SQLException &
	 *             if parsing fails
	 */

  /* throws SQLException & */
  /* we currently don't use neither urls, nor properties - commented out*/
  /*template <typename ConcreteTestFixture>
  Properties BaseTestFixture<ConcreteTestFixture>::getPropertiesFromTestsuiteUrl()
  {
    Properties props = new NonRegisteringDriver().parseURL(dbUrl, NULL);
    String hostname = props[ NonRegisteringDriver.HOST_PROPERTY_KEY ];

    if ( hostname.size() == 0 )
    {
      props.insert(Properties::value_type(NonRegisteringDriver.HOST_PROPERTY_KEY,
          _T("localhost") ));
    }
    else if ( hostname.substr(0,1) == _T(":") )
    {
      props.insert(Properties::value_type(NonRegisteringDriver.HOST_PROPERTY_KEY,
          _T( "localhost" ) ) );
      props.insert( Properties::value_type( NonRegisteringDriver.PORT_PROPERTY_KEY, hostname
          .substr(1) ) );
    }

    String portNumber = props[ NonRegisteringDriver.PORT_PROPERTY_KEY ];

    if (portNumber.size() == 0 )
    {
      props.insert( Properties::value_type( NonRegisteringDriver.PORT_PROPERTY_KEY, _T("3306") ) );
    }

    return props;
  }*/


/* throws SQLException & */
  template<typename ConcreteTestFixture>
  int BaseTestFixture<ConcreteTestFixture>::getRowCount(const String & tableName)
  {
    ResultSet countRs;
    try
    {
      countRs.reset( this->stmt->executeQuery("SELECT COUNT(*) FROM "
          + tableName) );
      countRs->next();
      return countRs->getInt(1);
    }
    catch(...)
    {
    }

    if (countRs.get() != NULL)
    {
      countRs->close();
    }
  }


  /* throws SQLException & */
  template <typename ConcreteTestFixture>
  Object BaseTestFixture<ConcreteTestFixture>::getSingleIndexedValueWithQuery(Connection & c,
			int columnIndex, const String & query)
  {
    ResultSet valueRs;
    Statement svStmt;

    try
    {
      svStmt.reset( c->createStatement() );
      valueRs.reset( svStmt->executeQuery(query) );

      if (! valueRs->next())
      {
        return NULL;
      }

      return valueRs->getString(columnIndex);

    }
    catch (...)
    {
    }

    if (valueRs.get() != NULL)
    {
      valueRs->close();
    }

    if (svStmt.get() != NULL)
    {
       svStmt->close();
    }
  }


  /* throws SQLException & */
  template <typename ConcreteTestFixture>
  Object BaseTestFixture<ConcreteTestFixture>::getSingleIndexedValueWithQuery(int columnIndex,
			const String & query)
  {
    return getSingleIndexedValueWithQuery(this->conn, columnIndex, query);
  }


  /* throws SQLException & */
  template <typename ConcreteTestFixture>
  Object BaseTestFixture<ConcreteTestFixture>::getSingleValue(const String & tableName, const String & columnName,
			const String & whereClause)
  {
    return getSingleValueWithQuery(String("SELECT ") + columnName + " FROM "
        + tableName + ((whereClause.size() > 0) ? "" : " " + whereClause));
  }


  /* throws SQLException & */
  template <typename ConcreteTestFixture>
  Object BaseTestFixture<ConcreteTestFixture>::getSingleValueWithQuery(const String &  query)
  {
    return getSingleIndexedValueWithQuery(1, query);
  }


  template <typename ConcreteTestFixture>
  bool BaseTestFixture<ConcreteTestFixture>::isAdminConnectionConfigured()
  {
    return false/*System.getProperty(ADMIN_CONNECTION_PROPERTY_NAME) != NULL*/;
  }


  /* throws SQLException & */
  template <typename ConcreteTestFixture>
  bool BaseTestFixture<ConcreteTestFixture>::isServerRunningOnWindows()
  {
    return (getMysqlVariable("datadir").find_first_of('\\') != -1);
  }


  template <typename ConcreteTestFixture>
  void BaseTestFixture<ConcreteTestFixture>::logDebug(String message)
  {
    // TODO: add possibility to turn on debug messages in command line
    if (false)
      logMsg( message );
  }


/* throws IOException */

/**
  In conn/j tests newTempBinaryFile used to be in prepared statement's
  setBinaryStream. It's not supported by conn/c++ (so far), so - commented out
*/

 /*
  File BaseTestFixture<ConcreteTestFixture>::newTempBinaryFile(String name, long size)
   {
     File tempFile = File.createTempFile(name, "tmp");
     tempFile.deleteOnExit();
     cleanupTempFiles(tempFile, name);
     FileOutputStream fos = new FileOutputStream(tempFile);
     BufferedOutputStream bos = new BufferedOutputStream(fos);
     for (long i = 0; i < size; i++) {
       bos.write((byte) i);
     }
     bos.close();
     ASSERT(tempFile.exists());
     ASSERT_EQUALS(size, tempFile.length());
     return tempFile;
   }*/


  template <typename ConcreteTestFixture>
  bool BaseTestFixture<ConcreteTestFixture>::runLongTests()
  {
    return runTestIfSysPropDefined("sql::mysql::testsuite.runLongTests");
  }

  /**
	 * Checks whether a certain system property is defined, in order to
	 * run/not-run certain tests
	 *
	 * @param propName
	 *            the property name to check for
	 *
	 * @return true if the property is defined.
	 */
  template <typename ConcreteTestFixture>
  bool BaseTestFixture<ConcreteTestFixture>::runTestIfSysPropDefined(String propName)
  {
    //String prop = System.getProperty(propName);
    return true;//(prop != NULL) && (prop.length() > 0);
  }


  template <typename ConcreteTestFixture>
  bool BaseTestFixture<ConcreteTestFixture>::runMultiHostTests()
  {
    return !runTestIfSysPropDefined(NO_MULTI_HOST_PROPERTY_NAME);
  }

  /**
	 * Creates resources used by all tests.
	 *
	 * @throws Exception
	 *             if an error occurs.
	 */

  /* throws Exception */
  template <typename ConcreteTestFixture>
  void BaseTestFixture<ConcreteTestFixture>::setUp()
  {
    this->createdObjects.clear();

    try
    {
      this->conn.reset( getConnection() );
    }
    catch (sql::SQLException & sqle)
    {
      logErr( String("Couldn't get connection") + sqle.what() );
      throw sqle;
    }

    this->stmt.reset( this->conn->createStatement() );

    try
    {
      if (host.find_first_of( "mysql" ) != String::npos )
      {
        this->rs.reset( this->stmt->executeQuery("SELECT VERSION()") );
        this->rs->next();
        logDebug("Connected to " + this->rs->getString(1));
        //this->rs->close();
        this->rs.reset();
      }
      else
      {
        logDebug("Connected to "
            + this->conn->getMetaData()->getDatabaseProductName()
            + " / "
            + this->conn->getMetaData()->getDatabaseProductVersion());
      }
    }
    catch (sql::SQLException & sqle)
    {
      logErr( sqle.what() );
    }

    if (this->rs.get() != NULL)
      this->rs->close();

    DatabaseMetaData dbmd(conn->getMetaData());

    hasSps = dbmd->supportsStoredProcedures();

	  stmt->execute( String( "USE " ) + defaultDb );
  }

  /**
	 * Destroys resources created during the test case.
	 *
	 * @throws Exception
	 *             DOCUMENT ME!
	 */

  /* throws Exception */
  template <typename ConcreteTestFixture>
  void BaseTestFixture<ConcreteTestFixture>::tearDown()
  {
    if (this->rs.get() != NULL) {
      try {
        this->rs->close();
      } catch (sql::SQLException & ) {
        ;
      }
    }
    for (int i = 0; i < static_cast<int>(this->createdObjects.size() - 1); i += 2 )
    {
      try
      {
        dropSchemaObject(this->createdObjects[ i ], this->createdObjects[ i+1 ]);
      }
      catch (sql::SQLException & )
      {
      }
    }
    if (this->stmt.get() != NULL) {
      try
      {
        this->stmt->close();
      }
      catch (sql::SQLException & )
      {
      }
    }
    if (this->pstmt.get() != NULL) {
      try {
        this->pstmt->close();
      } catch (sql::SQLException & ) {
        ;
      }
    }
    if (this->conn.get()!= NULL)
    {
      try
      {
        conn.reset();
      }
      catch (sql::SQLException & )
      {
      }
    }
  }

  /**
	 * Checks whether the database we're connected to meets the given version
	 * minimum
	 *
	 * @param major
	 *            the major version to meet
	 * @param minor
	 *            the minor version to meet
	 *
	 * @return bool if the major/minor is met
	 *
	 * @throws SQLException &
	 *             if an error occurs.
	 */

  /* throws SQLException & */
  template <typename ConcreteTestFixture>
  bool BaseTestFixture<ConcreteTestFixture>::versionMeetsMinimum(int major, int minor)
  {
    return versionMeetsMinimum(major, minor, 0);
  }

  /**
	 * Checks whether the database we're connected to meets the given version
	 * minimum
	 *
	 * @param major
	 *            the major version to meet
	 * @param minor
	 *            the minor version to meet
	 *
	 * @return bool if the major/minor is met
	 *
	 * @throws SQLException &
	 *             if an error occurs.
	 */

  /* throws SQLException & */
  template <typename ConcreteTestFixture>
  bool BaseTestFixture<ConcreteTestFixture>::versionMeetsMinimum(int major, int minor, int subminor)
  {
    return true;
    /*
        ((dynamic_cast<sql::mysql::MySQL_Connection*> (this->conn)->versionMeetsMinimum(
                major, minor, subminor));*/

  }


  /*template <typename ConcreteTestFixture>
  bool BaseTestFixture<ConcreteTestFixture>::isClassAvailable(String classname)
  {
    try
    {
      Class.forName(classname);
      return true;
    }
    catch (ClassNotFoundException e)
    {
      return false;
    }
  }*/


  template <typename ConcreteTestFixture>
  void BaseTestFixture<ConcreteTestFixture>::closeMemberJDBCResources()
  {
    if (this->rs.get() != NULL) {
      ResultSet toClose = this->rs;
      this->rs.reset();
      try {
        toClose->close();
      } catch (sql::SQLException & sqlEx) {
        // ignore
      }
    }
    if (this->pstmt.get() != NULL) {
      PreparedStatement toClose = this->pstmt;
      this->pstmt.reset();
      try {
        toClose->close();
      } catch (sql::SQLException & sqlEx) {
        // ignore
      }
    }
  }


  /**
  (almost) Same reasons as for createTempFile - for commenting out
  */

  /*
  void BaseTestFixture<ConcreteTestFixture>::cleanupTempFiles(const File exampleTempFile, const String tempfilePrefix)
    {
      File tempfilePath = exampleTempFile.getParentFile();
      File[] possibleFiles = tempfilePath.listFiles(new FilenameFilter() {
        public bool accept(File dir, String name) {
          return (name.find_first_of(tempfilePrefix) != -1
              && !exampleTempFile.getName().equals(name));
        }});
      for (int i = 0; i < possibleFiles.length; i++) {
        try {
          possibleFiles[i].delete();
        } catch (Throwable t) {
          // ignore, we're only making a best effort cleanup attempt here
        }
      }
    }*/


  /* throws Exception */
  template <typename ConcreteTestFixture>
  void BaseTestFixture<ConcreteTestFixture>::assertResultSetsEqual(ResultSet & control, ResultSet & test)
  {
    int controlNumCols = control->getMetaData()->getColumnCount();
    int testNumCols = test->getMetaData()->getColumnCount();

    ASSERT_EQUALS(controlNumCols, testNumCols);

    while (control->next())
    {
      test->next();
      for (int i = 0; i < controlNumCols; i++) {
        Object controlObj = getObject( control.get(), i + 1 );
        Object testObj    = getObject( test.get()   , i + 1 );

        if (controlObj.get() == NULL) {
          ASSERT( testObj.get() == NULL );
        }
        else
        {
          ASSERT( testObj.get() != NULL );
        }

        /** don't really have getFloat **
        if (controlObj->instanceof( value_object::vtFloat ))
                {
                  ASSERT_EQUALS_EPSILON(((float)controlObj).floatValue(),
                      ((float)testObj).floatValue(), 0.1);
                }
                else*/

        if ( controlObj->instanceof( value_object::vtDouble ) )
        {
          ASSERT_EQUALS_EPSILON( controlObj->doubleValue(),
              testObj->doubleValue(), 0.1);
        }
        else
        {
          ASSERT_EQUALS( controlObj->toString(), testObj->toString() );
        }
      }
    }

    ASSERT( ! test->next() );
  }


  /** TODO: move these(and other methods not depending on template parameter)
  * to cpp file
  */
  template <typename ConcreteTestFixture>
  void BaseTestFixture<ConcreteTestFixture>::initTable( const String  & sTableName
                                                      , Properties    & _sqlProps
                                                      , Connection    & _conn) 
  {
    String    execString;
    String    sKeyName;
    String    binarySize;
    String    varbinarySize;
    String    createString;
    String    createString1;
    String    createString2;
    Statement statement;

    try
    {
      clearTable(sTableName, _conn);
      logMsg("deleted rows from table " + sTableName);
    }
    catch (sql::SQLException & e )
    {
      logMsg("Exception encountered deleting rows from  table: "
        + sTableName + ": " + e.what());
    }

    try
    {
      statement.reset( _conn->createStatement() );

      if ( ( sTableName.find_first_of("Binary_Tab") == 0 ) )
      {
        binarySize= _sqlProps[ "binarySize" ];
        logMsg("Binary Table Size : " + binarySize);

        String insertString= _sqlProps[ "Binary_Tab_Insert" ];

        logMsg("Insert String " + insertString);
        logMsg("Adding rows to the table");
        statement->executeUpdate(insertString);

        logMsg("Successfully inserted the row");
      }
      else if ( ( sTableName.find_first_of( "Varbinary_Tab" ) == 0 ) )
      {
        varbinarySize= _sqlProps[ ("varbinarySize") ];

        TestsListener::theInstance().messagesLog()
          << "Varbinary Table Size : " << varbinarySize << std::endl;
        String insertString= _sqlProps[ "Varbinary_Tab_Insert" ];

        logMsg("Insert String " + insertString);
        logMsg("Adding rows to the table");
        statement->executeUpdate(insertString);

        logMsg("Successfully inserted the row");
      }
      else
      {
        logMsg("Adding rows to the table" + sTableName);
        sKeyName= sTableName;
        sKeyName.append("_Insert");

        logMsg("sKeyName :" + sKeyName);
        execString= _sqlProps[ sKeyName ];
        statement->executeUpdate(execString);

        logMsg("Rows added to the table " + sTableName);
      }
    }
    catch (sql::SQLException & e)
    {
      logErr("sql::DbcException creating the Table" + sTableName);
      logErr(e.what());
      throw e;
    }
    catch (std::exception & )
    {
      logErr("Setup Failed!");
      exit(1);
    }
  }


  template <typename ConcreteTestFixture>
  void BaseTestFixture<ConcreteTestFixture>::clearTable(const String & sTableName
                                                        , Connection & _conn) 
  {
    logMsg("dropTab");

    String sTag= sTableName;
    sTag.append("_Delete");

    String removeString= sqlProps[ sTag ];

    logMsg("Executable String " + removeString);

    Statement _stmt( _conn->createStatement() );

    _stmt->executeUpdate(removeString);
    _stmt->close();
  }
}
