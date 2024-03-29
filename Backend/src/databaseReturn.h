#pragma once
#include "boolReturnStructure.h"
#include "CppSQLite3.h"
#include <exception>
#include <string>
#include <type_traits>

class ExceptionReturnBase :
    public safe_bool <ExceptionReturnBase> {
public:
    ExceptionReturnBase(const std::exception_ptr& e = nullptr);
    
    bool boolean_test() const;
    std::string what();
protected:
    std::exception_ptr err;
};


template <typename T>
class FWDErrorReturn : public ExceptionReturnBase {
public:
    FWDErrorReturn(T& result, std::exception_ptr e = nullptr);
    FWDErrorReturn(const T& result, std::exception_ptr e = nullptr);

    T& operator*();
    T* operator->() requires (std::is_class_v<T>);
public:
    T result;
};

template<typename T>
inline FWDErrorReturn<T>::FWDErrorReturn(T& result, std::exception_ptr e) : ExceptionReturnBase(e) {
    this->result = result;
}

template<typename T>
inline FWDErrorReturn<T>::FWDErrorReturn(const T& result, std::exception_ptr e) : ExceptionReturnBase(e) {
    this->result = result;
}

template<typename T>
inline T* FWDErrorReturn<T>::operator->() requires (std::is_class_v<T>) {
    return &result;
}

template<typename T>
inline T& FWDErrorReturn<T>::operator*() {
    return result;
}