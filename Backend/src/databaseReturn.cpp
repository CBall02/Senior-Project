#include "databaseReturn.h"
#include <exception>
#include <stdexcept>
#include <string>



ExceptionReturnBase::ExceptionReturnBase(const std::exception_ptr& e) {
	err = e;
}

std::string ExceptionReturnBase::what() {
	try {
		if (err)
			std::rethrow_exception(err);
	}
	catch (const std::exception& e) {
		return e.what();
	}
	return "";
}

bool ExceptionReturnBase::boolean_test() const {
	return !err.operator bool();
}