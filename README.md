A **strong type** is a type used in place of another type to carry specific **meaning** through its **name**.

This project experiments with strong types in C++.

It central piece is the templated class NamedType, which can be used to declare a strong type with a typedef-like syntax:

```
using Width = NamedType<double, struct WidthParameter>;
using Height = NamedType<double, struct WidthParameter>;
```

which can be used to make interfaces more expressive and more robust.
Note how the below constructor shows in which order it expects its parameters:

```
class Rectangle
{
public:
    Rectangle(Width width, Height height) : width_(width.get()), height_(height.get()) {}
    double getWidth() const { return width_; }
    double getHeight() const { return height_; }

private:
    double width_;
    double height_;
};
```

**Strong types are about better expressing your intentions, both to the compiler and to other human developers.**

This implementation of strong types can be used to add strong typing over generic or unknown types such as lambdas:

```
template<typename Function>
using Comparator = NamedType<Function, struct ComparatorParameter>;

template <typename Function>
void performAction(Comparator<Function> comp)
{
    comp.get()();
}

performAction(make_named<Comparator>([](){ std::cout << "compare\n"; }));
```

A special class is used for passing strong types by reference: NamedTypeRef.

You can have a look at main.cpp for usage examples.
