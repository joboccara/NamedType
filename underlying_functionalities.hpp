#ifndef UNDERLYING_FUNCTIONALITIES_HPP
#define UNDERLYING_FUNCTIONALITIES_HPP

#include "crtp.hpp"
#include "named_type.fwd.hpp"

#include <iostream>

namespace fluent
{

template <typename T>
struct Incrementable : crtp<T, Incrementable>
{
    T& operator+=(T const& other) { this->underlying().get() += other.get(); return this->underlying(); }
};

template <typename T>
struct Addable : crtp<T, Addable>
{
    T operator+(T const& other) const { return T(this->underlying().get() + other.get()); }
};

template <typename T>
struct Comparable : crtp<T, Comparable>
{
    bool operator==(T const& other) const { return this->underlying().get() == other.get(); }
    bool operator!=(T const& other) const { return !(*this == other); }
};

template <typename T>
struct Printable : crtp<T, Printable>
{
    void print(std::ostream& os) const { os << this->underlying().get(); }
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

template <typename T, typename Parameter, template<typename> class... Skills>
std::ostream& operator<<(std::ostream& os, NamedType<T, Parameter, Skills...> const& object)
{
    object.print(os);
    return os;
}

template<typename T>
struct Hashable
{
    static constexpr bool is_hashable = true;
};

} // namespace fluent

namespace std
{
template <typename T, typename Parameter, typename Converter, template<typename> class... Skills>
struct hash<fluent::NamedTypeImpl<T, Parameter, Converter, Skills...>>
{
    using NamedType = fluent::NamedTypeImpl<T, Parameter, Converter, Skills...>;
    using checkIfHashable = typename std::enable_if<NamedType::is_hashable, void>::type;
    
    size_t operator()(fluent::NamedTypeImpl<T, Parameter, Converter, Skills...> const& x) const
    {
        return std::hash<T>()(x.get());
    }
};
}
    

#endif
