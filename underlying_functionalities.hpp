#ifndef UNDERLYING_FUNCTIONALITIES_HPP
#define UNDERLYING_FUNCTIONALITIES_HPP

#include "crtp.hpp"
#include "named_type.fwd.hpp"

template <typename T>
struct Incrementable : crtp<T, Incrementable>
{
    T& operator+=(T const& other) { this->underlying().get() += other.get(); return this->underlying(); }
};

template <typename T>
struct Addable : crtp<T, Addable>
{
    T operator+(T const& other) { return T(this->underlying().get() + other.get()); }
};

template <typename T>
struct Printable : crtp<T, Printable>
{
    void print(std::ostream& os) const { os << this->underlying().get(); }
};

template <typename T, typename Parameter, template<typename> class... Skills>
std::ostream& operator<<(std::ostream& os, NamedType<T, Parameter, Skills...> const& object)
{
    object.print(os);
    return os;
}

#endif
