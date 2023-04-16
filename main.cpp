#include "utils.hpp"
#include "value.hpp"

// std
#include <cmath>
#include <iostream>

double lol(double h = {}) {
    Value a{2, "a"};
    Value b{-3, "b"};
    Value c{10, "c"};
    Value e{a * b};
    e.label("e");
    Value d{e + c};
    d.label("d");
    Value f(-2, "f");
    Value L{d * f};
    L.label("L");

    return L.get();
}

double tan_h(double x) {
    return std::tanh(x);
}

int main() {
    double h = 0.001f;

    Value a{2, "a"};
    a.grad(-2 * -3);
    Value b{-3, "b"};
    b.grad(-2 * 2);
    Value c{10, "c"};
    c.grad(-2);
    Value e{a * b};
    e.label("e");
    e.grad(-2);
    Value d{e + c};
    d.label("d");
    d.grad(-2 * 1);
    Value f(-2, "f");
    f.grad(4 * 1);
    Value L{d * f};
    L.label("L");
    L.grad(1);

    graph(L);
    std::vector<double> x = arange(-5, 5, .2);
    std::vector<double> y = apply_function(x, tanh);
    plot(x, y);

    return 0;
}