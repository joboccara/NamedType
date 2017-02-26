#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include "named_type.hpp"

// Usage examples

namespace 
{
namespace test
{

using Meter = NamedType<double, struct MeterParameter, Addable, Comparable>;
Meter operator"" _meter(unsigned long long value) { return Meter(value); }
//Meter operator"" _meter(long double value) { return Meter(value); }

using Kilometer = MultipleOf<Meter, std::ratio<1000>>;
Kilometer operator"" _kilometer(unsigned long long value) { return Kilometer(value); }
Kilometer operator"" _kilometer(long double value) { return Kilometer(value); }

using Millimeter = MultipleOf<Meter, std::ratio<1, 1000>>;

using Width = NamedType<Meter, struct WidthParameter>;
using Height = NamedType<Meter, struct HeightParameter>;

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

bool testBasicUsage()
{
    Rectangle r(Width(10_meter), Height(12_meter));
    return r.getWidth().get() == 10 &&  r.getHeight().get() == 12;
}

using NameRef = NamedType<std::string&, struct NameRefParameter>;

void changeValue(const NameRef name)
{
    name.get() = "value2";
}

bool testReference()
{
    std::string value = "value1";
    changeValue(NameRef(value));
    return value == "value2";
}

template<typename Function>
using Comparator = NamedType<Function, struct ComparatorParameter>;

template <typename Function>
std::string performAction(Comparator<Function> comp)
{
    return comp.get()();
}

bool testGenericType()
{
    return performAction(make_named<Comparator>([](){ return std::string("compare"); })) == "compare";
}

std::string displayDistanceInKilometer(Kilometer d)
{
    std::ostringstream result;
    result << d.get() << "km";
    return result.str();
}

bool testMeterToKm()
{
    return displayDistanceInKilometer(31000_meter) == "31km";
}

std::string displayDistanceInMeter(Meter d)
{
    std::ostringstream result;
    result << d.get() << "m";
    return result.str();
}

bool testKmToMeter()
{
    return displayDistanceInMeter(31_kilometer) == "31000m";
}

std::string displayDistanceInMillimeter(Millimeter d)
{
    std::ostringstream result;
    result << std::fixed << std::setprecision(0) << d.get() << "mm";
    return result.str();
}

bool testKmToMillimeter()
{
    return displayDistanceInMillimeter(31_kilometer) == "31000000mm";
}

bool testMeterToKmWithDecimals()
{
    return displayDistanceInKilometer(31234_meter) == "31.234km";
}

bool testComparable()
{
    return 10_meter == 10_meter && !(10_meter == 11_meter)
        && !(10_meter == 11_meter) && 10_meter !=  11_meter;
}

bool testAddableComparableConvertible()
{
    return 1_kilometer + 200_meter == 1200_meter
        && 1_kilometer + 200_meter == 1.2_kilometer;
}

template <typename TestFunction>
bool launchTest(std::string const& testName, TestFunction testFunction)
{
    const bool success = testFunction();
    if (!success)
        std::cout << "Test - " << testName << ": FAILED\n";
    return success;
}

void launchTests()
{
    bool success = true;
    success &= launchTest("Basic usage", testBasicUsage);
    success &= launchTest("Passing by reference", testReference);
    success &= launchTest("Generic type", testGenericType);
    success &= launchTest("meter to km", testMeterToKm);
    success &= launchTest("km to meter", testKmToMeter);
    success &= launchTest("km to mm", testKmToMillimeter);
    success &= launchTest("meter to km with decimals", testMeterToKmWithDecimals);
    success &= launchTest("comparable", testComparable);
    success &= launchTest("addable comparable convertible", testAddableComparableConvertible);
    if (success)
        std::cout << "All tests PASSED\n";
}

}
}
int main()
{
    test::launchTests();
}
