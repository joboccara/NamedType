#include <iostream>
#include <string>
#include "named_type.hpp"

// Usage examples

using Meter = NamedType<double, struct MeterParameter, Addable>;
using Kilometer = MultipleOf<Meter, std::ratio<1000>>;
Meter operator"" _meter(unsigned long long value)
{
    return Meter(value);
}

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

using NameRef = NamedType<std::string&, struct NameRefParameter>;

void printName(const NameRef name)
{
    std::cout << name.get() << '\n';
}

template<typename Function>
using Comparator = NamedType<Function, struct ComparatorParameter>;

template <typename Function>
void performAction(Comparator<Function> comp)
{
    comp.get()();
}

void displayDistance(Kilometer d)
{
    std::cout << d.get() << "km\n";
}

int main()
{
    std::cout << "Basic example: Rectangle\n";
    Rectangle r(Width(10_meter), Height(12_meter));
    std::cout << "Rectangle of width " << r.getWidth().get() << "m and height " << r.getHeight().get() << "m\n";

    std::cout << '\n';
    std::cout << "Passing by reference\n";
    std::string userInput = "Johnny";
    printName(NameRef(userInput));

    std::cout << '\n';
    std::cout << "Strong lambda\n";
    performAction(make_named<Comparator>([](){ std::cout << "compare\n"; }));

    std::cout << '\n';
    displayDistance(31000_meter);
}
