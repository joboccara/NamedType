#ifndef NAMED_TYPE_HPP
#define NAMED_TYPE_HPP

#include <functional>
#include <ratio>
#include <type_traits>
#include "underlying_functionalities.hpp"

template <typename T, typename Parameter, typename Ratio, template<typename> class... Skills>
class NamedTypeImpl : public Skills<NamedTypeImpl<T, Parameter, Ratio, Skills...>>...
{
public:
    // constructor
    explicit NamedTypeImpl(T const& value) : value_(value) {}
    template<typename T_ = T>
    explicit NamedTypeImpl(T&& value, typename std::enable_if<!std::is_reference<T_>{}, std::nullptr_t>::type = nullptr) : value_(std::move(value)) {}

    // get
    T& get() { return value_; }
    T const& get() const {return value_; }

    // conversions with ratios
    template <typename Ratio2>
    operator NamedTypeImpl<T, Parameter, Ratio2, Skills...>() const
    {
        return NamedTypeImpl<T, Parameter, Ratio2, Skills...>(get() * Ratio::num / Ratio::den * Ratio2::den / Ratio2::num);
    }
    template<typename Ratio1, typename Ratio2>
    using MultiplyRatio = std::ratio<Ratio1::num * Ratio2::num, Ratio1::den * Ratio2::den>;
    template <typename Ratio2>
    using GetMultiple = NamedTypeImpl<T, Parameter, MultiplyRatio<Ratio, Ratio2>, Skills...>;

private:
    T value_;
};

template <typename T, typename Parameter, template<typename> class... Skills>
using NamedType = NamedTypeImpl<T, Parameter, std::ratio<1>, Skills...>;

template <typename StrongType, typename Ratio>
using MultipleOf = typename StrongType::template GetMultiple<Ratio>;

template<template<typename T> class StrongType, typename T>
StrongType<T> make_named(T const& value)
{
    return StrongType<T>(value);
}

#endif
