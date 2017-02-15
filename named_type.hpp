#ifndef NAMED_TYPE_HPP
#define NAMED_TYPE_HPP

#include <functional>
#include <type_traits>
#include "underlying_functionalities.hpp"

template <typename T, typename Parameter, template<typename> class... Skills>
class NamedType : public Skills<NamedType<T, Parameter, Skills...>>...
{
public:
    explicit NamedType(T const& value) : value_(value) {}
    template<typename T_ = T>
    explicit NamedType(T&& value, typename std::enable_if<!std::is_reference<T_>::value, std::nullptr_t>::type = nullptr) : value_(std::move(value)) {}

    T& get() { return value_; }
    T const& get() const {return value_; }
private:
    T value_;
};

template<template<typename T> class GenericTypeName, typename T>
GenericTypeName<T> make_named(T const& value)
{
    return GenericTypeName<T>(value);
}

#endif
