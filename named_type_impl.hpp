#ifndef named_type_impl_h
#define named_type_impl_h

#include <type_traits>

namespace fluent
{
    
template<typename T>
using IsNotReference = typename std::enable_if<!std::is_reference<T>::value, void>::type;

template <typename T, typename Parameter, template<typename> class... Skills>
class NamedType : public Skills<NamedType<T, Parameter, Skills...>>...
{
public:
    using UnderlyingType = T;
    
    // constructor
    explicit NamedType(T const& value) : value_(value) {}
    template<typename T_ = T, typename = IsNotReference<T_>>
    explicit NamedType(T&& value) : value_(std::move(value)) {}
    
    // get
    T& get() { return value_; }
    T const& get() const {return value_; }
    
    // conversions
    using ref = NamedType<T&, Parameter, Skills...>;
    operator ref ()
    {
        return ref(value_);
    }
    
    struct argument
    {
        template<typename U>
        NamedType operator=(U&& value) const
        {
            return NamedType(std::forward<U>(value));
        }
        argument() = default;
        argument(argument const&) = delete;
        argument(argument &&) = delete;
        argument& operator=(argument const&) = delete;
        argument& operator=(argument &&) = delete;
    };
    
private:
    T value_;
};

template<template<typename T> class StrongType, typename T>
StrongType<T> make_named(T const& value)
{
    return StrongType<T>(value);
}
    
} // namespace fluent

#endif /* named_type_impl_h */
