// Cleanup windows include - windows.h is included by "catch.hpp" -->

#define NOGDI

// <-- Cleanup windows include

#include "catch.hpp"

#include "NamedType/named_type.hpp"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

// Usage examples

template <typename T>
decltype(auto) tee(T&& value)
{
    std::cout << value << '\n';
    return std::forward<T>(value);
}

using Meter = fluent::NamedType<unsigned long long, struct MeterParameter, fluent::Addable, fluent::Comparable>;
Meter operator"" _meter(unsigned long long value)
{
    return Meter(value);
}

using Width = fluent::NamedType<Meter, struct WidthParameter>;
using Height = fluent::NamedType<Meter, struct HeightParameter>;

class Rectangle
{
public:
    Rectangle(Width width, Height height) : width_(width.get()), height_(height.get())
    {
    }
    Meter getWidth() const
    {
        return width_;
    }
    Meter getHeight() const
    {
        return height_;
    }

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

TEST_CASE("Default construction")
{
    using StrongInt = fluent::NamedType<int, struct StrongIntTag>;
    StrongInt strongInt;
    strongInt.get() = 42;
    REQUIRE(strongInt.get() == 42);
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
    REQUIRE(performAction(fluent::make_named<Comparator>([]() { return std::string("compare"); })) == "compare");
}

TEST_CASE("Addable")
{
    using AddableType = fluent::NamedType<int, struct AddableTag, fluent::Addable>;
    AddableType s1(12);
    AddableType s2(10);
    REQUIRE((s1 + s2).get() == 22);
    REQUIRE((+s1).get() == 12);
}

TEST_CASE("Addable constexpr")
{
    using AddableType = fluent::NamedType<int, struct AddableTag, fluent::Addable>;
    constexpr AddableType s1(12);
    constexpr AddableType s2(10);
    static_assert((s1 + s2).get() == 22, "Addable is not constexpr");
    static_assert((+s1).get() == 12, "Addable is not constexpr");
}

TEST_CASE("BinaryAddable")
{
    using BinaryAddableType = fluent::NamedType<int, struct BinaryAddableTag, fluent::BinaryAddable>;
    BinaryAddableType s1(12);
    BinaryAddableType s2(10);
    REQUIRE((s1 + s2).get() == 22);
}

TEST_CASE("BinaryAddable constexpr")
{
    using BinaryAddableType = fluent::NamedType<int, struct BinaryAddableTag, fluent::BinaryAddable>;
    constexpr BinaryAddableType s1(12);
    constexpr BinaryAddableType s2(10);
    static_assert((s1 + s2).get() == 22, "BinaryAddable is not constexpr");
}

TEST_CASE("UnaryAddable")
{
    using UnaryAddableType = fluent::NamedType<int, struct UnaryAddableTag, fluent::UnaryAddable>;
    UnaryAddableType s1(12);
    REQUIRE((+s1).get() == 12);
}

TEST_CASE("UnaryAddable constexpr")
{
    using UnaryAddableType = fluent::NamedType<int, struct UnaryAddableTag, fluent::UnaryAddable>;
    constexpr UnaryAddableType s1(12);
    static_assert((+s1).get() == 12, "UnaryAddable is not constexpr");
}

TEST_CASE("Subtractable")
{
    using SubtractableType = fluent::NamedType<int, struct SubtractableTag, fluent::Subtractable>;
    SubtractableType s1(12);
    SubtractableType s2(10);
    REQUIRE((s1 - s2).get() == 2);
    REQUIRE((-s1).get() == -12);
}

TEST_CASE("Subtractable constexpr")
{
    using SubtractableType = fluent::NamedType<int, struct SubtractableTag, fluent::Subtractable>;
    constexpr SubtractableType s1(12);
    constexpr SubtractableType s2(10);
    static_assert((s1 - s2).get() == 2, "Subtractable is not constexpr");
    static_assert((-s1).get() == -12, "Subtractable is not constexpr");
}

TEST_CASE("BinarySubtractable")
{
    using BinarySubtractableType = fluent::NamedType<int, struct BinarySubtractableTag, fluent::BinarySubtractable>;
    BinarySubtractableType s1(12);
    BinarySubtractableType s2(10);
    REQUIRE((s1 - s2).get() == 2);
}

TEST_CASE("BinarySubtractable constexpr")
{
    using BinarySubtractableType = fluent::NamedType<int, struct BinarySubtractableTag, fluent::BinarySubtractable>;
    constexpr BinarySubtractableType s1(12);
    constexpr BinarySubtractableType s2(10);
    static_assert((s1 - s2).get() == 2, "BinarySubtractable is not constexpr");
}

TEST_CASE("UnarySubtractable")
{
    using UnarySubtractableType = fluent::NamedType<int, struct UnarySubtractableTag, fluent::UnarySubtractable>;
    UnarySubtractableType s(12);
    REQUIRE((-s).get() == -12);
}

TEST_CASE("UnarySubtractable constexpr")
{
    using UnarySubtractableType = fluent::NamedType<int, struct UnarySubtractableTag, fluent::UnarySubtractable>;
    constexpr UnarySubtractableType s(12);
    static_assert((-s).get() == -12, "UnarySubtractable is not constexpr");
}


TEST_CASE("Multiplicable")
{
    using MultiplicableType = fluent::NamedType<int, struct MultiplicableTag, fluent::Multiplicable>;
    MultiplicableType s1(12);
    MultiplicableType s2(10);
    REQUIRE((s1 * s2).get() == 120);
    s1 *= s2;
    REQUIRE(s1.get() == 120);
}

TEST_CASE("Multiplicable constexpr")
{
    using MultiplicableType = fluent::NamedType<int, struct MultiplicableTag, fluent::Multiplicable>;
    constexpr MultiplicableType s1(12);
    constexpr MultiplicableType s2(10);
    static_assert((s1 * s2).get() == 120, "Multiplicable is not constexpr");
}

TEST_CASE("Divisible")
{
    using DivisibleType = fluent::NamedType<int, struct DivisibleTag, fluent::Divisible>;
    DivisibleType s1(120);
    DivisibleType s2(10);
    REQUIRE((s1 / s2).get() == 12);
    s1 /= s2;
    REQUIRE(s1.get() == 12);
}

TEST_CASE("Divisible constexpr")
{
    using DivisibleType = fluent::NamedType<int, struct DivisibleTag, fluent::Divisible>;
    constexpr DivisibleType s1(120);
    constexpr DivisibleType s2(10);
    static_assert((s1 / s2).get() == 12, "Divisible is not constexpr");
}


TEST_CASE("Modulable")
{
    using ModulableType = fluent::NamedType<int, struct ModulableTag, fluent::Modulable>;
    ModulableType s1(5);
    ModulableType s2(2);
    CHECK((s1 % s2).get() == 1);
    s1 %= s2;
    CHECK(s1.get() == 1);
}

TEST_CASE("Modulable constexpr")
{
    using ModulableType = fluent::NamedType<int, struct ModulableTag, fluent::Modulable>;
    constexpr ModulableType s1(5);
    constexpr ModulableType s2(2);
    static_assert((s1 % s2).get() == 1, "Modulable is not constexpr");
}

TEST_CASE("BitWiseInvertable")
{
    using BitWiseInvertableType = fluent::NamedType<int, struct BitWiseInvertableTag, fluent::BitWiseInvertable>;
    BitWiseInvertableType s1(13);
    CHECK((~s1).get() == (~13));
}

TEST_CASE("BitWiseInvertable constexpr")
{
    using BitWiseInvertableType = fluent::NamedType<int, struct BitWiseInvertableTag, fluent::BitWiseInvertable>;
    constexpr BitWiseInvertableType s1(13);
    static_assert((~s1).get() == (~13), "BitWiseInvertable is not constexpr");
}

TEST_CASE("BitWiseAndable")
{
    using BitWiseAndableType = fluent::NamedType<int, struct BitWiseAndableTag, fluent::BitWiseAndable>;
    BitWiseAndableType s1(2);
    BitWiseAndableType s2(64);
    CHECK((s1 & s2).get() == (2 & 64));
    s1 &= s2;
    CHECK(s1.get() == (2 & 64));
}

TEST_CASE("BitWiseAndable constexpr")
{
    using BitWiseAndableType = fluent::NamedType<int, struct BitWiseAndableTag, fluent::BitWiseAndable>;
    constexpr BitWiseAndableType s1(2);
    constexpr BitWiseAndableType s2(64);
    static_assert((s1 & s2).get() == (2 & 64), "BitWiseAndable is not constexpr");
}

TEST_CASE("BitWiseOrable")
{
    using BitWiseOrableType = fluent::NamedType<int, struct BitWiseOrableTag, fluent::BitWiseOrable>;
    BitWiseOrableType s1(2);
    BitWiseOrableType s2(64);
    CHECK((s1 | s2).get() == (2 | 64));
    s1 |= s2;
    CHECK(s1.get() == (2 | 64));
}

TEST_CASE("BitWiseOrable constexpr")
{
    using BitWiseOrableType = fluent::NamedType<int, struct BitWiseOrableTag, fluent::BitWiseOrable>;
    constexpr BitWiseOrableType s1(2);
    constexpr BitWiseOrableType s2(64);
    static_assert((s1 | s2).get() == (2 | 64), "BitWiseOrable is not constexpr");
}

TEST_CASE("BitWiseXorable")
{
    using BitWiseXorableType = fluent::NamedType<int, struct BitWiseXorableTag, fluent::BitWiseXorable>;
    BitWiseXorableType s1(2);
    BitWiseXorableType s2(64);
    CHECK((s1 ^ s2).get() == (2 ^ 64));
    s1 ^= s2;
    CHECK(s1.get() == (2 ^ 64));
}

TEST_CASE("BitWiseXorable constexpr")
{
    using BitWiseXorableType = fluent::NamedType<int, struct BitWiseXorableTag, fluent::BitWiseXorable>;
    constexpr BitWiseXorableType s1(2);
    constexpr BitWiseXorableType s2(64);
    static_assert((s1 ^ s2).get() == (2 ^ 64), "BitWiseXorable is not constexpr");
}

TEST_CASE("BitWiseLeftShiftable")
{
    using BitWiseLeftShiftableType =
        fluent::NamedType<int, struct BitWiseLeftShiftableTag, fluent::BitWiseLeftShiftable>;
    BitWiseLeftShiftableType s1(2);
    BitWiseLeftShiftableType s2(3);
    CHECK((s1 << s2).get() == (2 << 3));
    s1 <<= s2;
    CHECK(s1.get() == (2 << 3));
}

TEST_CASE("BitWiseLeftShiftable constexpr")
{
    using BitWiseLeftShiftableType =
        fluent::NamedType<int, struct BitWiseLeftShiftableTag, fluent::BitWiseLeftShiftable>;
    constexpr BitWiseLeftShiftableType s1(2);
    constexpr BitWiseLeftShiftableType s2(3);
    static_assert((s1 << s2).get() == (2 << 3), "BitWiseLeftShiftable is not constexpr");
}

TEST_CASE("BitWiseRightShiftable")
{
    using BitWiseRightShiftableType =
        fluent::NamedType<int, struct BitWiseRightShiftableTag, fluent::BitWiseRightShiftable>;
    BitWiseRightShiftableType s1(2);
    BitWiseRightShiftableType s2(3);
    CHECK((s1 >> s2).get() == (2 >> 3));
    s1 >>= s2;
    CHECK(s1.get() == (2 >> 3));
}

TEST_CASE("BitWiseRightShiftable constexpr")
{
    using BitWiseRightShiftableType =
        fluent::NamedType<int, struct BitWiseRightShiftableTag, fluent::BitWiseRightShiftable>;
    constexpr BitWiseRightShiftableType s1(2);
    constexpr BitWiseRightShiftableType s2(3);
    static_assert((s1 >> s2).get() == (2 >> 3), "BitWiseRightShiftable is not constexpr");
}

TEST_CASE("Comparable")
{
    REQUIRE((10_meter == 10_meter));
    REQUIRE(!(10_meter == 11_meter));
    REQUIRE((10_meter != 11_meter));
    REQUIRE(!(10_meter != 10_meter));
    REQUIRE((10_meter < 11_meter));
    REQUIRE(!(10_meter < 10_meter));
    REQUIRE((10_meter <= 10_meter));
    REQUIRE((10_meter <= 11_meter));
    REQUIRE(!(10_meter <= 9_meter));
    REQUIRE((11_meter > 10_meter));
    REQUIRE(!(10_meter > 11_meter));
    REQUIRE((11_meter >= 10_meter));
    REQUIRE((10_meter >= 10_meter));
    REQUIRE(!(9_meter >= 10_meter));
}

TEST_CASE("ConvertibleWithOperator")
{
    struct B
    {
        B(int x_) : x(x_)
        {
        }
        int x;
    };

    struct A
    {
        A(int x_) : x(x_)
        {
        }
        operator B() const
        {
            return B(x);
        }
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
        A(int x_) : x(x_)
        {
        }
        int x;
    };

    struct B
    {
        B(A a) : x(a.x)
        {
        }
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

    std::unordered_map<SerialNumber, int> hashMap = {{SerialNumber{"AA11"}, 10}, {SerialNumber{"BB22"}, 20}};
    SerialNumber cc33{"CC33"};
    hashMap[cc33] = 30;
    REQUIRE(hashMap[SerialNumber{"AA11"}] == 10);
    REQUIRE(hashMap[SerialNumber{"BB22"}] == 20);
    REQUIRE(hashMap[cc33] == 30);
}

struct testFunctionCallable_A
{
    testFunctionCallable_A(int x_) : x(x_)
    {
    }
    // ensures that passing the argument to a function doesn't make a copy
    testFunctionCallable_A(testFunctionCallable_A const&) = delete;
    testFunctionCallable_A(testFunctionCallable_A&&) = default;
    testFunctionCallable_A& operator+=(testFunctionCallable_A const& other)
    {
        x += other.x;
        return *this;
    }
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
    auto functionTakingA = [](A const& a) { return a.x; };

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
        A(int x_) : x(x_)
        {
        }
        A(A const&) = delete; // ensures that invoking a method doesn't make a copy
        A(A&&) = default;

        int method()
        {
            return x;
        }
        int constMethod() const
        {
            return x;
        }

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
        A(int x_) : x(x_)
        {
        }
        A(A const&) = delete; // ensures that invoking a method or function doesn't make a copy
        A(A&&) = default;

        int method()
        {
            return x;
        }
        int constMethod() const
        {
            return x;
        }

    private:
        int x;
    };

    auto functionTakingA = [](A const& a) { return a.constMethod(); };

    using StrongA = fluent::NamedType<A, struct StrongATag, fluent::Callable>;
    StrongA strongA(A(42));
    const StrongA constStrongA(A(42));
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
    auto getFullName = [](FirstName const& firstName_, LastName const& lastName_) //
    {
        return firstName_.get() + lastName_.get(); //
    };

    auto fullName = getFullName(firstName = "James", lastName = "Bond");
    REQUIRE(fullName == "JamesBond");
}

TEST_CASE("Named arguments in any order")
{
    using FirstName = fluent::NamedType<std::string, struct FirstNameTag>;
    using LastName = fluent::NamedType<std::string, struct LastNameTag>;
    static const FirstName::argument firstName;
    static const LastName::argument lastName;

    auto getFullName = fluent::make_named_arg_function<FirstName, LastName>([](FirstName const& firstName_, LastName const& lastName_)
    {
        return firstName_.get() + lastName_.get();
    });

    auto fullName = getFullName(lastName = "Bond", firstName = "James");
    REQUIRE(fullName == "JamesBond");

    auto otherFullName = getFullName(firstName = "James", lastName = "Bond");
    REQUIRE(otherFullName == "JamesBond");
}

TEST_CASE("Named arguments with bracket constructor")
{
    using Numbers = fluent::NamedType<std::vector<int>, struct NumbersTag>;
    static const Numbers::argument numbers;
    auto getNumbers = [](Numbers const& numbers_)
    {
        return numbers_.get();
    };

    auto vec = getNumbers(numbers = {1, 2, 3});
    REQUIRE(vec == std::vector<int>{1, 2, 3});
}

TEST_CASE("Empty base class optimization")
{
    REQUIRE(sizeof(Meter) == sizeof(double));
}

using strong_int = fluent::NamedType<int, struct IntTag>;

TEST_CASE("constexpr")
{
    using strong_bool = fluent::NamedType<bool, struct BoolTag>;

    static_assert(strong_bool{true}.get(), "NamedType is not constexpr");
}

struct throw_on_construction
{
    throw_on_construction()
    {
        throw 42;
    }

    throw_on_construction(int)
    {
        throw "exception";
    }
};

using C = fluent::NamedType<throw_on_construction, struct throwTag>;

TEST_CASE("noexcept")
{
    CHECK(noexcept(strong_int{}));
    CHECK(!noexcept(C{}));

    CHECK(noexcept(strong_int(3)));
    CHECK(!noexcept(C{5}));
}

TEST_CASE("Arithmetic")
{
    using strong_arithmetic = fluent::NamedType<int, struct ArithmeticTag, fluent::Arithmetic>;
    strong_arithmetic a{1};
    strong_arithmetic b{2};

    CHECK((a + b).get() == 3);

    a += b;
    CHECK(a.get() == 3);

    CHECK((a - b).get() == 1);

    a -= b;
    CHECK(a.get() == 1);

    a.get() = 5;
    CHECK((a * b).get() == 10);

    a *= b;
    CHECK(a.get() == 10);

    CHECK((a / b).get() == 5);

    a /= b;
    CHECK(a.get() == 5);
}

TEST_CASE("Version macros are defined")
{
    static_assert(NAMED_TYPE_VERSION_MAJOR >= 1, "");
    static_assert(NAMED_TYPE_VERSION_MINOR >= 0, "");
    static_assert(NAMED_TYPE_VERSION_PATCH >= 0, "");

    std::stringstream ss;
    ss << NAMED_TYPE_VERSION_MAJOR << '.' << NAMED_TYPE_VERSION_MINOR << '.' << NAMED_TYPE_VERSION_PATCH;
    CHECK(ss.str() == NAMED_TYPE_VERSION);
}

TEST_CASE("Printable")
{
    using StrongInt = fluent::NamedType<int, struct StrongIntTag, fluent::Printable>;

    std::ostringstream oss;
    oss << StrongInt( 42 );
    CHECK(oss.str() == "42");
}

TEST_CASE("Dereferencable")
{
    using StrongInt = fluent::NamedType<int, struct StrongIntTag, fluent::Dereferencable>;

    {
        StrongInt a{1};
        int& value = *a;
        CHECK( value == 1 );
    }

    {
        const StrongInt a{1};
        const int& value = *a;
        CHECK( value == 1 );
    }

    {
        StrongInt a{1};
        int& value = *a;
        value = 2;
        CHECK( a.get() == 2 );
    }

    {
        auto functionReturningStrongInt = []() { return StrongInt{28}; };
        auto functionTakingInt = []( int value ) { return value; };

        int value = functionTakingInt( *functionReturningStrongInt() );
        CHECK( value == 28 );
    }
}

TEST_CASE("Dereferencable constexpr")
{
    using StrongInt = fluent::NamedType<int, struct StrongIntTag, fluent::Dereferencable>;

    constexpr StrongInt a{28};
    static_assert( *a, "Dereferencable is not constexpr");
}

TEST_CASE("PreIncrementable")
{
    using StrongInt = fluent::NamedType<int, struct StrongIntTag, fluent::PreIncrementable>;
    StrongInt a{1};
    StrongInt b = ++a;
    CHECK( a.get() == 2 );
    CHECK( b.get() == 2 );
}

TEST_CASE("PostIncrementable")
{
    using StrongInt = fluent::NamedType<int, struct StrongIntTag, fluent::PostIncrementable>;
    StrongInt a{1};
    StrongInt b = a++;
    CHECK( a.get() == 2 );
    CHECK( b.get() == 1 );
}

TEST_CASE("PreDecrementable")
{
    using StrongInt = fluent::NamedType<int, struct StrongIntTag, fluent::PreDecrementable>;
    StrongInt a{1};
    StrongInt b = --a;
    CHECK( a.get() == 0 );
    CHECK( b.get() == 0 );
}

TEST_CASE("PostDecrementable")
{
    using StrongInt = fluent::NamedType<int, struct StrongIntTag, fluent::PostDecrementable>;
    StrongInt a{1};
    StrongInt b = a--;
    CHECK( a.get() == 0 );
    CHECK( b.get() == 1 );
}
