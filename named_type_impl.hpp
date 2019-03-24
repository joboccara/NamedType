#ifndef named_type_impl_h
#define named_type_impl_h

#include <type_traits>
#include <utility>

// Enable empty base class optimization with multiple inheritance on Visual Studio.
#if defined(_MSC_VER) && _MSC_VER >= 1910
#  define FLUENT_EBCO __declspec(empty_bases)
#else
#  define FLUENT_EBCO
#endif

namespace fluent
{

template<typename T>
using IsNotReference = typename std::enable_if<!std::is_reference<T>::value, void>::type;

template <typename T, typename Parameter, template<typename> class... Skills>
class FLUENT_EBCO NamedType : public Skills<NamedType<T, Parameter, Skills...>>...
{
public:
    using UnderlyingType = T;

    // constructor
    template <typename T_ = T, typename = std::enable_if<std::is_default_constructible_v<T>, void>>
    constexpr NamedType() noexcept(noexcept(T())) {}
    explicit constexpr NamedType(T const& value) : value_(value) noexcept(noexcept(T(value))) {}
    template<typename T_ = T, typename = IsNotReference<T_>>
    explicit constexpr NamedType(T&& value) : value_(std::move(value)) noexcept(noexcept(T(std::move(value)))) {}

    // get
    [[nodiscard]] constexpr T& get() noexcept { return value_; }
    [[nodiscard]] constexpr std::remove_reference_t<T> const& get() const noexcept {return value_; }

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
constexpr StrongType<T> make_named(T const& value)
{
    return StrongType<T>(value);
}

} // namespace fluent

#endif /* named_type_impl_h */
