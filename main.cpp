#include <iostream>
#include <sstream>
#include <string>
#include "named_type.hpp"

// Usage examples

namespace 
{
namespace test
{

using Meter = NamedType<double, struct MeterParameter, Addable>;
Meter operator"" _meter(unsigned long long value)
{
    return Meter(value);
}
using Kilometer = MultipleOf<Meter, std::ratio<1000>>;
Kilometer operator"" _kilometer(unsigned long long value)
{
    return Kilometer(value);
}

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

bool testMeterToKmWithDecimals()
{
    return displayDistanceInKilometer(31234_meter) == "31.234km";
}

template <typename TestFunction>
void launchTest(std::string const& testName, TestFunction testFunction)
{
    std::cout << "Test - " << testName << ": " << (testFunction() ? "OK" : "FAILED") << std::endl;
}

void launchTests()
{
    launchTest("Basic usage", testBasicUsage);
    launchTest("Passing by reference", testReference);
    launchTest("Generic type", testGenericType);
    launchTest("meter to km", testMeterToKm);
    launchTest("km to meter", testKmToMeter);
    launchTest("meter to km with decimals", testMeterToKmWithDecimals);
}

}
}
int main()
{
    test::launchTests();
}
