#ifndef UNDERLYING_FUNCTIONALITIES_HPP
#define UNDERLYING_FUNCTIONALITIES_HPP

#include "crtp.hpp"
#include "named_type_impl.hpp"

#include <functional>
#include <iostream>
#include <memory>

namespace fluent
{

template <typename T>
struct PreIncrementable : crtp<T, PreIncrementable>
{
    IGNORE_SHOULD_RETURN_REFERENCE_TO_THIS_BEGIN

    T& operator++()
    {
        ++this->underlying().get();
        return this->underlying();
    }

    IGNORE_SHOULD_RETURN_REFERENCE_TO_THIS_END
};

template <typename T>
struct PostIncrementable : crtp<T, PostIncrementable>
{
    IGNORE_SHOULD_RETURN_REFERENCE_TO_THIS_BEGIN

    T operator++(int)
    {
        return T(this->underlying().get()++);
    }

    IGNORE_SHOULD_RETURN_REFERENCE_TO_THIS_END
};

template <typename T>
struct PreDecrementable : crtp<T, PreDecrementable>
{
    IGNORE_SHOULD_RETURN_REFERENCE_TO_THIS_BEGIN

    T& operator--()
    {
        --this->underlying().get();
        return this->underlying();
    }

    IGNORE_SHOULD_RETURN_REFERENCE_TO_THIS_END
};

template <typename T>
struct PostDecrementable : crtp<T, PostDecrementable>
{
    IGNORE_SHOULD_RETURN_REFERENCE_TO_THIS_BEGIN

    T operator--(int)
    {
        return T( this->underlying().get()-- );
    }

    IGNORE_SHOULD_RETURN_REFERENCE_TO_THIS_END
};

template <typename T>
struct BinaryAddable : crtp<T, BinaryAddable>
{
    constexpr T operator+(T const& other) const
    {
        return T(this->underlying().get() + other.get());
    }
    T& operator+=(T const& other)
    {
        this->underlying().get() += other.get();
        return this->underlying();
    }
};

template <typename T>
struct UnaryAddable : crtp<T, UnaryAddable>
{
    constexpr T operator+() const
    {
        return T(+this->underlying().get());
    }
};

template <typename T>
struct Addable : BinaryAddable<T>, UnaryAddable<T>
{
    using BinaryAddable<T>::operator+;
    using UnaryAddable<T>::operator+;
};

template <typename T>
struct BinarySubtractable : crtp<T, BinarySubtractable>
{
    constexpr T operator-(T const& other) const
    {
        return T(this->underlying().get() - other.get());
    }
    T& operator-=(T const& other)
    {
        this->underlying().get() -= other.get();
        return this->underlying();
    }
};
   
template <typename T>
struct UnarySubtractable : crtp<T, UnarySubtractable>
{
    constexpr T operator-() const
    {
        return T(-this->underlying().get());
    }
};
   
template <typename T>
struct Subtractable : BinarySubtractable<T>, UnarySubtractable<T>
{
    using UnarySubtractable<T>::operator-;
    using BinarySubtractable<T>::operator-;
};
   
template <typename T>
struct Multiplicable : crtp<T, Multiplicable>
{
    constexpr T operator*(T const& other) const
    {
        return T(this->underlying().get() * other.get());
    }
    T& operator*=(T const& other)
    {
        this->underlying().get() *= other.get();
        return this->underlying();
    }
};

template <typename T>
struct Divisible : crtp<T, Divisible>
{
    constexpr T operator/(T const& other) const
    {
        return T(this->underlying().get() / other.get());
    }
    T& operator/=(T const& other)
    {
        this->underlying().get() /= other.get();
        return this->underlying();
    }
};

template <typename T>
struct Modulable : crtp<T, Modulable>
{
    constexpr T operator%(T const& other) const
    {
        return T(this->underlying().get() % other.get());
    }
    T& operator%=(T const& other)
    {
        this->underlying().get() %= other.get();
        return this->underlying();
    }
};

template <typename T>
struct BitWiseInvertable : crtp<T, BitWiseInvertable>
{
    constexpr T operator~() const
    {
        return T(~this->underlying().get());
    }
};

template <typename T>
struct BitWiseAndable : crtp<T, BitWiseAndable>
{
    constexpr T operator&(T const& other) const
    {
        return T(this->underlying().get() & other.get());
    }
    T& operator&=(T const& other)
    {
        this->underlying().get() &= other.get();
        return this->underlying();
    }
};

template <typename T>
struct BitWiseOrable : crtp<T, BitWiseOrable>
{
    constexpr T operator|(T const& other) const
    {
        return T(this->underlying().get() | other.get());
    }
    T& operator|=(T const& other)
    {
        this->underlying().get() |= other.get();
        return this->underlying();
    }
};

template <typename T>
struct BitWiseXorable : crtp<T, BitWiseXorable>
{
    constexpr T operator^(T const& other) const
    {
        return T(this->underlying().get() ^ other.get());
    }
    T& operator^=(T const& other)
    {
        this->underlying().get() ^= other.get();
        return this->underlying();
    }
};

template <typename T>
struct BitWiseLeftShiftable : crtp<T, BitWiseLeftShiftable>
{
    constexpr T operator<<(T const& other) const
    {
        return T(this->underlying().get() << other.get());
    }
    T& operator<<=(T const& other)
    {
        this->underlying().get() <<= other.get();
        return this->underlying();
    }
};

template <typename T>
struct BitWiseRightShiftable : crtp<T, BitWiseRightShiftable>
{
    constexpr T operator>>(T const& other) const
    {
        return T(this->underlying().get() >> other.get());
    }
    T& operator>>=(T const& other)
    {
        this->underlying().get() >>= other.get();
        return this->underlying();
    }
};

template <typename T>
struct Comparable : crtp<T, Comparable>
{
    constexpr bool operator<(T const& other) const
    {
        return this->underlying().get() < other.get();
    }
    constexpr bool operator>(T const& other) const
    {
        return other.get() < this->underlying().get();
    }
    constexpr bool operator<=(T const& other) const
    {
        return !(other.get() < this->underlying().get());
    }
    constexpr bool operator>=(T const& other) const
    {
        return !(*this < other);
    }
// On Visual Studio before 19.22, you cannot define constexpr with friend function
// See: https://stackoverflow.com/a/60400110
#if defined(_MSC_VER) && _MSC_VER < 1922
    constexpr bool operator==(T const& other) const
    {
        return !(*this < other) && !(other.get() < this->underlying().get());
    }
#else
    friend constexpr bool operator==(Comparable<T> const& self, T const& other)
    {
        return !(self < other) && !(other.get() < self.underlying().get());
    }
#endif
    constexpr bool operator!=(T const& other) const
    {
        return !(*this == other);
    }
};

template< typename T >
struct Dereferencable;

template< typename T, typename Parameter, template< typename > class ... Skills >
struct Dereferencable<NamedType<T, Parameter, Skills...>> : crtp<NamedType<T, Parameter, Skills...>, Dereferencable>
{
    constexpr T& operator*() &
    {
        return this->underlying().get();
    }
    constexpr std::remove_reference_t<T> const& operator*() const &
    {
        return this->underlying().get();
    }
};

template <typename Destination>
struct ImplicitlyConvertibleTo
{
    template <typename T>
    struct templ : crtp<T, templ>
    {
        operator Destination() const
        {
            return this->underlying().get();
        }
    };
};

template <typename T>
struct Printable : crtp<T, Printable>
{
    static constexpr bool is_printable = true;

    void print(std::ostream& os) const
    {
        os << this->underlying().get();
    }
};

template <typename T, typename Parameter, template <typename> class... Skills>
typename std::enable_if<NamedType<T, Parameter, Skills...>::is_printable, std::ostream&>::type
operator<<(std::ostream& os, NamedType<T, Parameter, Skills...> const& object)
{
    object.print(os);
    return os;
}

template <typename T>
struct Hashable
{
    static constexpr bool is_hashable = true;
};

template <typename NamedType_>
struct FunctionCallable;

template <typename T, typename Parameter, template <typename> class... Skills>
struct FunctionCallable<NamedType<T, Parameter, Skills...>> : crtp<NamedType<T, Parameter, Skills...>, FunctionCallable>
{
    operator T const&() const
    {
        return this->underlying().get();
    }
    operator T&()
    {
        return this->underlying().get();
    }
};

template <typename NamedType_>
struct MethodCallable;

template <typename T, typename Parameter, template <typename> class... Skills>
struct MethodCallable<NamedType<T, Parameter, Skills...>> : crtp<NamedType<T, Parameter, Skills...>, MethodCallable>
{
    std::remove_reference_t<T> const* operator->() const
    {
        return std::addressof(this->underlying().get());
    }
    std::remove_reference_t<T>* operator->()
    {
        return std::addressof(this->underlying().get());
    }
};

template <typename NamedType_>
struct Callable
    : FunctionCallable<NamedType_>
    , MethodCallable<NamedType_>
{
};

template <typename T>
struct Arithmetic
    : PreIncrementable<T>
    , PostIncrementable<T>
    , PreDecrementable<T>
    , PostDecrementable<T>
    , Addable<T>
    , Subtractable<T>
    , Multiplicable<T>
    , Divisible<T>
    , Modulable<T>
    , BitWiseInvertable<T>
    , BitWiseAndable<T>
    , BitWiseOrable<T>
    , BitWiseXorable<T>
    , BitWiseLeftShiftable<T>
    , BitWiseRightShiftable<T>
    , Comparable<T>
    , Printable<T>
    , Hashable<T>
{
};

} // namespace fluent

namespace std
{
template <typename T, typename Parameter, template <typename> class... Skills>
struct hash<fluent::NamedType<T, Parameter, Skills...>>
{
    using NamedType = fluent::NamedType<T, Parameter, Skills...>;
    using checkIfHashable = typename std::enable_if<NamedType::is_hashable, void>::type;

    size_t operator()(fluent::NamedType<T, Parameter, Skills...> const& x) const noexcept
    {
        static_assert(noexcept(std::hash<T>()(x.get())), "hash fuction should not throw");

        return std::hash<T>()(x.get());
    }
};

} // namespace std

#endif
