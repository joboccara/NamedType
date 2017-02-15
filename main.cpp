#include <iostream>
#include <string>
#include "named_type.hpp"

// Usage examples

using Meter = NamedType<double, struct MeterParameter, Addable>;
Meter operator"" _meter(unsigned long long value)
{
    return Meter(value);
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

using NameRef = NamedType<std::string&, struct NameRefParameter, Printable>;

void printName(const NameRef name)
{
    std::cout << name << '\n';
}

template<typename Function>
using Comparator = NamedType<Function, struct ComparatorParameter>;

template <typename Function>
void performAction(Comparator<Function> comp)
{
    comp.get()();
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
}
