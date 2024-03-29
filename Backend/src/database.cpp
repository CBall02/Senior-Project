#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include <stdexcept>

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
FWDErrorReturn<bool> Database::openDatabase(const string& file) {
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
        err = rethrow(e);
        success = false;
    }
    return FWDErrorReturn(success, err);
}


/**
 * @brief Close the current database
 * 
 */
FWDErrorReturn<bool> Database::closeDatabase() {
    bool res;
    std::exception_ptr err = nullptr;
    try {
        myDatabase.close();
        print("Closed database successfully\n");
        res = true;
    }
    catch (CppSQLite3Exception& e) {
        print("Can't close database: " + e.errorMessage() + "\n");
        err = rethrow(e);
        res = false;
    }
    return FWDErrorReturn(res, err);
}


/**
 * @brief execute an SQL command that is not going to expect a return from the database
 * 
 * @param sqlCmd Command to execute
 * @return true if execution was successful
 */
FWDErrorReturn<bool> Database::executeNoReturnSQL(const string& sqlCmd){
    bool res;
    std::exception_ptr err = nullptr;
    try {
        myDatabase.execDML(sqlCmd.c_str());
        print("Operation Successful\n");
        res = true;
    }
    catch (CppSQLite3Exception& e) {
        print("Operation Unsuccessful: " + e.errorMessage() + "\n");
        err = rethrow(e);
        res = false;
    }
    return FWDErrorReturn(res, err);
}


/**
 * @brief execute an SQL command that is not going to expect a return from the database
 * 
 * @param sqlCmd Command to execute
 * @return true if execution was successful
 */
FWDErrorReturn<bool> Database::sqlExec(const std::string& sqlCmd) {
    return executeNoReturnSQL(sqlCmd);
}


/**
 * @brief Send a query to the database and get back the result of the query
 * 
 * @param sqlQuery Query to perform
 * @return CppSQLite3Query& 
 */
FWDErrorReturn<CppSQLite3Query> Database::queryDatabase(const std::string& sqlQuery) {
    CppSQLite3Query res;
    std::exception_ptr err = nullptr;
    try {
        res = myDatabase.execQuery(sqlQuery.c_str());
        print("Operation Successful\n");
    }
    catch (CppSQLite3Exception& e) {
        print("Operation Unsuccessful: " + e.errorMessage() + "\n");
        err = rethrow(e);
    }
    return FWDErrorReturn(res, err);
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

inline std::exception_ptr Database::rethrow(CppSQLite3Exception& e) {
    try {
        throw std::runtime_error(e.errorMessage());
    }
    catch (const std::exception& e) {
        return std::current_exception();
    }
}


Database* Database::instance(){
    static Database db;
    return &db;
}



FWDErrorReturn<CppSQLite3Table> Database::getTable(const std::string& tableName) {
    CppSQLite3Table tableResult;
    std::exception_ptr err = nullptr;
    try {
        tableResult = myDatabase.getTable(("select * from " + tableName).c_str());
        print("Operation Successful\n");
    }
    catch (CppSQLite3Exception& e) {
        print("Operation Unsuccessful: " + e.errorMessage() + "\n");
        err = rethrow(e);
    }
    return FWDErrorReturn(tableResult, err);
}


vector<string> Database::getDatabaseTables() {
    auto query = queryDatabase("SELECT name FROM sqlite_schema WHERE type = 'table' AND name NOT LIKE 'sqlite_%'; ");
    vector<string> ret;
    if (!query) { return ret; }

    while (!query->eof()) {
        for (int i = 0; i < query->numFields(); i++) {
            ret.push_back(query->fieldValue(i));
        }
        query->nextRow();
    }
    return ret;
}


string Database::getTableSchema(string tableName) {
    if (!tableExists(tableName)) { return ""; }

    auto query = queryDatabase("SELECT sql FROM sqlite_schema WHERE name='" + tableName + "';");
    if (!query) { return ""; }

    if (!query->eof()) {
        return query->fieldValue(0);
    }

    return "";
}