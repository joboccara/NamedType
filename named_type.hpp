#ifndef NAMED_TYPE_HPP
#define NAMED_TYPE_HPP

#include <functional>
#include "underlying_functionalities.hpp"

template <typename T, typename Parameter, template<typename> class... Skills>
class NamedType : public Skills<NamedType<T, Parameter, Skills...>>...
{
public:
    explicit NamedType(T const& value) : value_(value) {}
    explicit NamedType(T&& value) : value_(std::move(value)) {}

    T& get() { return value_; }
    T const& get() const {return value_; }
private:
    T value_;
};

template <typename T, typename Parameter, template<typename> class... Skills>
class NamedTypeRef : public Skills<NamedType<T, Parameter, Skills...>>...
{
public:
    explicit NamedTypeRef(T& value) : value_(std::ref(value)) {}

    T& get() { return value_.get(); }
    T const& get() const {return value_.get(); }
private:
    std::reference_wrapper<T> value_;
};


template<template<typename T> class GenericTypeName, typename T>
GenericTypeName<T> make_named(T const& value)
{
    return GenericTypeName<T>(value);
}

#endif
