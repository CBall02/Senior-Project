#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "CppSQLite3.h"



class Database {
public:

    ~Database();
    bool openDatabase(const std::string& file = ":memory:");
    bool closeDatabase();
    bool sqlExec(const std::string& sqlCmd);
    CppSQLite3Query& queryDatabase(const std::string& sqlQuery);
    //CppSQLite3Query& getMostRecentQuery();
    bool tableExists(const std::string& tableName);
    CppSQLite3Table& getTable(const std::string& tableName);
    std::vector<std::string> getDatabaseTables();
    std::string getTableSchema(std::string tableName);

    friend std::ostream& operator<<(std::ostream& os, const Database& db){
        return os;
    }
    static Database* instance();

private:
    Database(const std::string& file = ":memory:");
    bool executeNoReturnSQL(const std::string& sqlCmd);
    inline void print(const std::string& statement);

private:
    CppSQLite3DB myDatabase;
    CppSQLite3Query queryResult;
    CppSQLite3Statement statementResult;
    CppSQLite3Table tableResult;

    std::string fileName;
};
