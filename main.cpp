#include "utils.hpp"
#include "value.hpp"

// std
#include <cmath>
#include <iostream>

double tan_h(double x) {
    return std::tanh(x);
}

int main() {
    // inputs
    Value x1{2.0, "x1"};
    x1.grad(-3 * 0.5);
    Value x2{0.0, "x2"};
    x2.grad(1 * 0.5);
    // weights
    Value w1{-3.0, "w1"};
    w1.grad(2 * 0.5);
    Value w2{1.0, "w2"};
    w2.grad(0 * 0.5);
    // bias
    Value b{6.8813735870195432, "b"};
    b.grad(0.5);
    // dot product
    Value x1w1 = x1 * w1;
    x1w1.label("x1w1");
    x1w1.grad(0.5);

    Value x2w2 = x2 * w2;
    x2w2.label("x2w2");
    x2w2.grad(0.5);

    Value x1w1_x2w2 = x1w1 + x2w2;
    x1w1_x2w2.label("x1w1 + x2w2");
    x1w1_x2w2.grad(0.5);
    // add bias
    Value n = x1w1_x2w2 + b;
    n.label("n");
    n.grad(0.5);  // derivative of tanh is 1 - tanh^2
    // output
    Value o = n.tanh();
    o.label("o");
    o.grad(1);

    graph(o);

    return 0;
}