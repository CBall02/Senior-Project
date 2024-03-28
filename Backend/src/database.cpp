#include <iostream>
#include <string>
#include <vector>
#include <exception>

#include "database.h"
#include "CppSQLite3.h"
#include "databaseReturn.h"

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
BoolReturn Database::openDatabase(const string& file) {
    if (file.empty()) { return false; }
    fileName = file;
    if(!closeDatabase()){
        return false;
    }


    bool success = false;
    std::exception_ptr err = nullptr;
    try {
        myDatabase.open(fileName.c_str());
        print("Opened database " + fileName +  " successfully\n");
        success = true;
    }
    catch (CppSQLite3Exception& e) {
        print("Can't open database: "  + e.errorMessage() + "\n");
        err = std::current_exception();
        success = false;
    }
    return BoolReturn(success, err);
}


/**
 * @brief Close the current database
 * 
 */
BoolReturn Database::closeDatabase() {
    bool res;
    std::exception_ptr err = nullptr;
    try {
        myDatabase.close();
        print("Closed database successfully\n");
        res = true;
    }
    catch (CppSQLite3Exception& e) {
        print("Can't close database: " + e.errorMessage() + "\n");
        err = std::current_exception();
        res = false;
    }
    return BoolReturn(res, err);
}


/**
 * @brief execute an SQL command that is not going to expect a return from the database
 * 
 * @param sqlCmd Command to execute
 * @return true if execution was successful
 */
BoolReturn Database::executeNoReturnSQL(const string& sqlCmd){
    bool res;
    std::exception_ptr err = nullptr;
    try {
        myDatabase.execDML(sqlCmd.c_str());
        print("Operation Successful\n");
        res = true;
    }
    catch (CppSQLite3Exception& e) {
        print("Operation Unsuccessful: " + e.errorMessage() + "\n");
        err = std::current_exception();
        res = false;
    }
    return BoolReturn(res, err);
}


/**
 * @brief execute an SQL command that is not going to expect a return from the database
 * 
 * @param sqlCmd Command to execute
 * @return true if execution was successful
 */
BoolReturn Database::sqlExec(const std::string& sqlCmd) {
    return executeNoReturnSQL(sqlCmd);
}


/**
 * @brief Send a query to the database and get back the result of the query
 * 
 * @param sqlQuery Query to perform
 * @return CppSQLite3Query& 
 */
QueryReturn Database::queryDatabase(const std::string& sqlQuery) {
    CppSQLite3Query res;
    std::exception_ptr err = nullptr;
    try {
        res = myDatabase.execQuery(sqlQuery.c_str());
        print("Operation Successful\n");
    }
    catch (CppSQLite3Exception& e) {
        print("Operation Unsuccessful: " + e.errorMessage() + "\n");
        err = std::current_exception();
    }
    return QueryReturn(res, err);
}


/**
 * @brief Get the result of the most recent query
 * 
 * @return CppSQLite3Query& 
 */
//CppSQLite3Query& Database::getMostRecentQuery() {
//    return queryResult;
//}


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



TableReturn Database::getTable(const std::string& tableName) {
    CppSQLite3Table tableResult;
    std::exception_ptr err = nullptr;
    try {
        tableResult = myDatabase.getTable(("select * from " + tableName).c_str());
        print("Operation Successful\n");
    }
    catch (CppSQLite3Exception& e) {
        print("Operation Unsuccessful: " + e.errorMessage() + "\n");
        err = std::current_exception();
    }
    return TableReturn(tableResult, err);
}


vector<string> Database::getDatabaseTables() {
    auto result = queryDatabase("SELECT name FROM sqlite_schema WHERE type = 'table' AND name NOT LIKE 'sqlite_%'; ");
    vector<string> ret;
    if (!result) { return ret; }

    while (!result.result.eof()) {
        for (int i = 0; i < result.result.numFields(); i++) {
            ret.push_back(result.result.fieldValue(i));
        }
        result.result.nextRow();
    }
    return ret;
}


string Database::getTableSchema(string tableName) {
    if (!tableExists(tableName)) { return ""; }

    auto result = queryDatabase("SELECT sql FROM sqlite_schema WHERE name='" + tableName + "';");
    if (!result) { return ""; }

    if (!result.result.eof()) {
        return result.result.fieldValue(0);
    }

    return "";
}