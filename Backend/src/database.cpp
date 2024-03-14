#include <iostream>
#include <string>
#include <vector>

#include "database.h"
#include "CppSQLite3.h"

#ifdef _DEBUG
#define printDebug true
#else
#define printDebug false
#endif




using std::string;
using std::vector;
using std::cerr;


/**
 * @brief Construct a new Database object with the filename of the database
 * 
 * @param file 
 */
Database::Database(const string& file){
    fileName = file;
    openDatabase();
}


/**
 * @brief Destroy the Database object and cleanup
 * 
 */
Database::~Database(){
    closeDatabase();
}


/**
 * @brief Close the current database and open the database specified by $file
 * 
 * @param file database to open
 */
bool Database::openDatabase(const string& file) {
    fileName = file;
    if(!closeDatabase()){
        return false;
    }

    string msg;
    bool success = false;
    try {
        myDatabase.open(fileName.c_str());
        msg = "Opened database successfully\n";
        success = true;
    }
    catch (CppSQLite3Exception& e) {
        msg = "Can't open database: "  + e.errorMessage() + "\n";
    }
    print(msg);
    return success;
}


/**
 * @brief Close the current database
 * 
 */
bool Database::closeDatabase(){
    try {
        myDatabase.close();
        print("Closed database successfully\n");
        return true;
    }
    catch (CppSQLite3Exception& e) {
        print("Can't close database: " + e.errorMessage() + "\n");
        return false;
    }
}


/**
 * @brief execute an SQL command that is not going to expect a return from the database
 * 
 * @param sqlCmd Command to execute
 * @return true if execution was successful
 */
bool Database::executeNoReturnSQL(const string& sqlCmd){
    string msg;
    try {
        myDatabase.execDML(sqlCmd.c_str());
        print("Operation Successful\n");
        return true;
    }
    catch (CppSQLite3Exception& e) {
        print("Operation Unsuccessful: " + e.errorMessage() + "\n");
        return false;
    }
}


/**
 * @brief execute an SQL command that is not going to expect a return from the database
 * 
 * @param sqlCmd Command to execute
 * @return true if execution was successful
 */
bool Database::sqlExec(const std::string& sqlCmd) {
    return executeNoReturnSQL(sqlCmd);
}


/**
 * @brief Send a query to the database and get back the result of the query
 * 
 * @param sqlQuery Query to perform
 * @return CppSQLite3Query& 
 */
CppSQLite3Query& Database::queryDatabase(const std::string& sqlQuery) {
    string msg;
    try {
        queryResult = myDatabase.execQuery(sqlQuery.c_str());
        msg = "Operation Successful\n";
    }
    catch (CppSQLite3Exception& e) {
        msg = "Operation Unsuccessful: " + e.errorMessage() + "\n";
    }
    print(msg);
    return queryResult;
}


/**
 * @brief Get the result of the most recent query
 * 
 * @return CppSQLite3Query& 
 */
CppSQLite3Query& Database::getMostRecentQuery() {
    return queryResult;
}


/**
 * @brief Checks if the table exists
 * 
 * @param tableName table to check for
 * @return true if table exists
 */
bool Database::tableExists(const std::string& tableName) {
    return myDatabase.tableExists(tableName.c_str());
}


/**
 * @brief print the outcome of any operation or error to the console
 * 
 * @param statement statement to print
 */
inline void Database::print(const std::string& statement) {
    if (printDebug) {
        cerr << statement;
    }
}

Database* Database::instance(){
    static Database db;
    return &db;
}




CppSQLite3Table& Database::getTable(const std::string& tableName) {
    try {
        tableResult = myDatabase.getTable(tableName.c_str());
        print("Operation Successful\n");
    }
    catch (CppSQLite3Exception& e) {
        print("Operation Unsuccessful: " + e.errorMessage() + "\n");
    }
    return tableResult;
}
