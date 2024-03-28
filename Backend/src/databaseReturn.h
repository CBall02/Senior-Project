#pragma once
#include "boolReturnStructure.h"
#include "CppSQLite3.h"
#include <exception>
#include <string>

class DatabaseReturn :
    public safe_bool <DatabaseReturn> {
public:
    DatabaseReturn(std::exception_ptr e = nullptr);
    
    bool boolean_test() const;
    std::string what();
protected:
    std::exception_ptr err;
};


class QueryReturn : public DatabaseReturn {
public:
    QueryReturn(CppSQLite3Query& result, std::exception_ptr e = nullptr);

public:
    CppSQLite3Query result;
};


class TableReturn : public DatabaseReturn {
public:
    TableReturn(CppSQLite3Table& result, std::exception_ptr e = nullptr);

public:
    CppSQLite3Table result;
};



class BoolReturn : public DatabaseReturn {
public:
    BoolReturn(const bool& result, std::exception_ptr e = nullptr);
    bool boolean_test() const;

public:
    bool result;
};