#ifndef NAMED_TYPE_FXD_HPP
#define NAMED_TYPE_FXD_HPP

template <typename T, typename Parameter, typename Ratio, template<typename> class... Skills>
class NamedTypeImpl;

template <typename T, typename Parameter, template<typename> class... Skills>
using NamedType = NamedTypeImpl<T, Parameter, std::ratio<1>, Skills...>;

#endif
