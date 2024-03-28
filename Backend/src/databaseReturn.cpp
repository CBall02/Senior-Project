#include "databaseReturn.h"
#include <exception>
#include <string>



DatabaseReturn::DatabaseReturn(std::exception_ptr e) {
	err = e;
}

std::string DatabaseReturn::what() {
	try {
		if (err)
			std::rethrow_exception(err);
	}
	catch (const std::exception& e) {
		return e.what();
	}
	return "";
}

bool DatabaseReturn::boolean_test() const {
	return !err.operator bool();
}

QueryReturn::QueryReturn(CppSQLite3Query& result, std::exception_ptr e)
	: DatabaseReturn(e){
	this->result = std::move(result);
}

TableReturn::TableReturn(CppSQLite3Table& result, std::exception_ptr e)
	: DatabaseReturn(e) {
	this->result = std::move(result);
}


BoolReturn::BoolReturn(const bool& result, std::exception_ptr e) : DatabaseReturn(e) {
	this->result = result;
}

bool BoolReturn::boolean_test() const {
	return result && !err.operator bool();
}