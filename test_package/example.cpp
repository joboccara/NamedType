#include <iostream>

#include <named_type.hpp>

int main() {
    using Width = fluent::NamedType<double, struct WidthTag>;
    using Height = fluent::NamedType<double, struct HeightTag>;

    std::cout << "NamedType works!" << std::endl;

    return 0;
}