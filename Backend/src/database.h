#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "CppSQLite3.h"
#include "databaseReturn.h"



class Database {
public:

    ~Database();
    BoolReturn openDatabase(const std::string& file = ":memory:");
    BoolReturn closeDatabase();
    BoolReturn sqlExec(const std::string& sqlCmd);
    QueryReturn queryDatabase(const std::string& sqlQuery);
    //CppSQLite3Query& getMostRecentQuery();
    bool tableExists(const std::string& tableName);
    TableReturn getTable(const std::string& tableName);
    std::vector<std::string> getDatabaseTables();
    std::string getTableSchema(std::string tableName);

    friend std::ostream& operator<<(std::ostream& os, const Database& db){
        return os;
    }
    static Database* instance();

private:
    Database(const std::string& file = ":memory:");
    BoolReturn executeNoReturnSQL(const std::string& sqlCmd);
    inline void print(const std::string& statement);

private:
    CppSQLite3DB myDatabase;
    std::string fileName;
};
