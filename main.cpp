#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include "named_type.hpp"

// Usage examples

template<typename T>
decltype(auto) tee(T&& value)
{
    std::cout << value << '\n';
    return std::forward<T>(value);
}

using Meter = fluent::NamedType<double, struct MeterParameter, fluent::Addable, fluent::Comparable>;
Meter operator"" _meter(unsigned long long value) { return Meter(value); }
//Meter operator"" _meter(long double value) { return Meter(value); }

using Width = fluent::NamedType<Meter, struct WidthParameter>;
using Height = fluent::NamedType<Meter, struct HeightParameter>;

class Rectangle
{
public:
    Rectangle(Width width, Height height) : width_(width.get()), height_(height.get()) {}
    Meter getWidth() const { return width_; }
    Meter getHeight() const { return height_; }

private:
    Meter width_;
    Meter height_;
};

TEST_CASE("Basic usage")
{
    Rectangle r(Width(10_meter), Height(12_meter));
    REQUIRE(r.getWidth().get() == 10);
    REQUIRE(r.getHeight().get() == 12);
}

using NameRef = fluent::NamedType<std::string&, struct NameRefParameter>;

void changeValue(NameRef name)
{
    name.get() = "value2";
}

TEST_CASE("Passing a strong reference")
{
    std::string value = "value1";
    changeValue(NameRef(value));
    REQUIRE(value == "value2");
}

TEST_CASE("Construction of NamedType::ref from the underlying type")
{
    using StrongInt = fluent::NamedType<int, struct StrongIntTag>;
    auto addOne = [](StrongInt::ref si) { ++(si.get()); };
    
    int i = 42;
    addOne(StrongInt::ref(i));
    REQUIRE(i == 43);
}

TEST_CASE("Implicit conversion of NamedType to NamedType::ref")
{
    using StrongInt = fluent::NamedType<int, struct StrongIntTag>;
    auto addOne = [](StrongInt::ref si) { ++(si.get()); };
    
    StrongInt i(42);
    addOne(i);
    REQUIRE(i.get() == 43);

    StrongInt j(42);
    addOne(StrongInt::ref(j));
    REQUIRE(j.get() == 43);
}

template<typename Function>
using Comparator = fluent::NamedType<Function, struct ComparatorParameter>;

template <typename Function>
std::string performAction(Comparator<Function> comp)
{
    return comp.get()();
}

TEST_CASE("Strong generic type")
{
    REQUIRE(performAction(fluent::make_named<Comparator>([](){ return std::string("compare"); })) == "compare");
}

TEST_CASE("Addable")
{
    using AddableType = fluent::NamedType<int, struct SubtractableTag, fluent::Addable>;
    AddableType s1(12);
    AddableType s2(10);
    REQUIRE((s1 + s2).get() == 22);
}

TEST_CASE("Subtractable")
{
    using SubtractableType = fluent::NamedType<int, struct SubtractableTag, fluent::Subtractable>;
    SubtractableType s1(12);
    SubtractableType s2(10);
    REQUIRE((s1 - s2).get() == 2);
}

TEST_CASE("Multiplicable")
{
    using MultiplicableType = fluent::NamedType<int, struct MultiplicableTag, fluent::Multiplicable>;
    MultiplicableType s1(12);
    MultiplicableType s2(10);
    REQUIRE((s1 * s2).get() == 120);
}

TEST_CASE("Negatable")
{
  using NegatableType = fluent::NamedType<int, struct NegatableTag, fluent::Negatable>;
  NegatableType value(10);
  REQUIRE((-value).get() == -10);
}

TEST_CASE("Comparable")
{
    REQUIRE((10_meter == 10_meter));
    REQUIRE(!(10_meter == 11_meter));
    REQUIRE((10_meter != 11_meter));
    REQUIRE(!(10_meter != 10_meter));
    REQUIRE((10_meter <  11_meter));
    REQUIRE(!(10_meter <  10_meter));
    REQUIRE((10_meter <= 10_meter));
    REQUIRE((10_meter <= 11_meter));
    REQUIRE(!(10_meter <= 9_meter));
    REQUIRE((11_meter >  10_meter));
    REQUIRE(!(10_meter > 11_meter));
    REQUIRE((11_meter >= 10_meter));
    REQUIRE((10_meter >= 10_meter));
    REQUIRE(!(9_meter >= 10_meter));
}

TEST_CASE("ConvertibleWithOperator")
{
    struct B
    {
        B(int x) : x(x) {}
        int x;
    };
    
    struct A
    {
        A(int x) : x(x) {}
        operator B () const { return B(x);}
        int x;
    };
        
    using StrongA = fluent::NamedType<A, struct StrongATag, fluent::ImplicitlyConvertibleTo<B>::templ>;
    StrongA strongA(A(42));
    B b = strongA;
    REQUIRE(b.x == 42);
}

TEST_CASE("ConvertibleWithConstructor")
{
    struct A
    {
        A(int x) : x(x) {}
        int x;
    };
        
    struct B
    {
        B(A a) : x(a.x) {}
        int x;
    };
        
    using StrongA = fluent::NamedType<A, struct StrongATag, fluent::ImplicitlyConvertibleTo<B>::templ>;
    StrongA strongA(A(42));
    B b = strongA;
    REQUIRE(b.x == 42);
}
    
TEST_CASE("ConvertibleToItself")
{
    using MyInt = fluent::NamedType<int, struct MyIntTag, fluent::ImplicitlyConvertibleTo<int>::templ>;
    MyInt myInt(42);
    int i = myInt;
    REQUIRE(i == 42);
}
    
TEST_CASE("Hash")
{
    using SerialNumber = fluent::NamedType<std::string, struct SerialNumberTag, fluent::Comparable, fluent::Hashable>;

    std::unordered_map<SerialNumber, int> hashMap = { {SerialNumber{"AA11"}, 10}, {SerialNumber{"BB22"}, 20} };
    SerialNumber cc33{"CC33"};
    hashMap[cc33] = 30;
    REQUIRE(hashMap[SerialNumber{"AA11"}] == 10);
    REQUIRE(hashMap[SerialNumber{"BB22"}] == 20);
    REQUIRE(hashMap[cc33] == 30);
}

struct testFunctionCallable_A
{
    testFunctionCallable_A(int x) : x(x) {}
    testFunctionCallable_A(testFunctionCallable_A const&) = delete; // ensures that passing the argument to a function doesn't make a copy
    testFunctionCallable_A(testFunctionCallable_A&&) = default;
    testFunctionCallable_A& operator+=(testFunctionCallable_A const& other) { x += other.x; return *this; }
    int x;
};
    
testFunctionCallable_A operator+(testFunctionCallable_A const& a1, testFunctionCallable_A const& a2)
{
    return testFunctionCallable_A(a1.x + a2.x);
}
    
bool operator==(testFunctionCallable_A const& a1, testFunctionCallable_A const& a2)
{
    return a1.x == a2.x;
}

TEST_CASE("Function callable")
{
    using A = testFunctionCallable_A;
    auto functionTakingA = [](A const& a){ return a.x; };
    
    using StrongA = fluent::NamedType<A, struct StrongATag, fluent::FunctionCallable>;
    StrongA strongA(A(42));
    const StrongA constStrongA(A(42));
    REQUIRE(functionTakingA(strongA) == 42);
    REQUIRE(functionTakingA(constStrongA) == 42);
    REQUIRE(strongA + strongA == 84);
}

TEST_CASE("Method callable")
{
    class A
    {
    public:
        A(int x) : x(x) {}
        A(A const&) = delete; // ensures that invoking a method doesn't make a copy
        A(A&&) = default;
        
        int method(){ return x; }
        int constMethod() const{ return x; }
    private:
        int x;
    };
    
    using StrongA = fluent::NamedType<A, struct StrongATag, fluent::MethodCallable>;
    StrongA strongA(A(42));
    const StrongA constStrongA(A((42)));
    REQUIRE(strongA->method() == 42);
    REQUIRE(constStrongA->constMethod() == 42);
}

TEST_CASE("Callable")
{
    class A
    {
    public:
        A(int x) : x(x) {}
        A(A const&) = delete; // ensures that invoking a method or function doesn't make a copy
        A(A&&) = default;
        
        int method(){ return x; }
        int constMethod() const{ return x; }
    private:
        int x;
    };
    
    auto functionTakingA = [](A const& a){ return a.constMethod(); };
    
    using StrongA = fluent::NamedType<A, struct StrongATag, fluent::Callable>;
    StrongA strongA(A(42));
    const StrongA constStrongA(A((42)));
    REQUIRE(functionTakingA(strongA) == 42);
    REQUIRE(strongA->method() == 42);
    REQUIRE(constStrongA->constMethod() == 42);
}

TEST_CASE("Named arguments")
{
    using FirstName = fluent::NamedType<std::string, struct FirstNameTag>;
    using LastName = fluent::NamedType<std::string, struct LastNameTag>;
    static const FirstName::argument firstName;
    static const LastName::argument lastName;
    auto getFullName = [](FirstName const& firstName, LastName const& lastName)
    {
        return firstName.get() + lastName.get();
    };
    
    auto fullName = getFullName(firstName = "James", lastName = "Bond");
    REQUIRE(fullName == "JamesBond");
}

TEST_CASE("Empty base class optimization")
{
    REQUIRE(sizeof(Meter) == sizeof(double));
}
