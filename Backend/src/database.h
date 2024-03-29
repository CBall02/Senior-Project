#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "CppSQLite3.h"
#include "databaseReturn.h"



class Database {
public:

    ~Database();
    FWDErrorReturn<bool> openDatabase(const std::string& file = ":memory:");
    FWDErrorReturn<bool> closeDatabase();
    FWDErrorReturn<bool> sqlExec(const std::string& sqlCmd);
    FWDErrorReturn<CppSQLite3Query> queryDatabase(const std::string& sqlQuery);
    //CppSQLite3Query& getMostRecentQuery();
    bool tableExists(const std::string& tableName);
    FWDErrorReturn<CppSQLite3Table> getTable(const std::string& tableName);
    std::vector<std::string> getDatabaseTables();
    std::string getTableSchema(std::string tableName);

    friend std::ostream& operator<<(std::ostream& os, const Database& db){
        return os;
    }
    static Database* instance();

private:
    Database(const std::string& file = ":memory:");
    FWDErrorReturn<bool> executeNoReturnSQL(const std::string& sqlCmd);
    static inline void print(const std::string& statement);
    inline std::exception_ptr rethrow(CppSQLite3Exception& e);

private:
    CppSQLite3DB myDatabase;
    std::string fileName;
};
