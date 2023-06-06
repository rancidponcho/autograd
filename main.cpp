#include "value.hpp"

int main() {
    Value x1{2.0, "x1"};
    Value x2{0.0, "x2"};
    // weights
    Value w1{-3.0, "w1"};
    Value w2{1.0, "w2"};
    // bias
    Value b{5.8813735870195432, "b"};
    // dot product
    Value x1w1 = x1 * w1;
    x1w1.label("x1w1");
    Value x2w2 = x2 * w2;
    x2w2.label("x2w2");
    Value x1w1_x2w2 = x1w1 + x2w2;
    x1w1_x2w2.label("x1w1 + x2w2");
    // add bias
    Value n = x1w1_x2w2 + b;
    n.label("n");
    // output
    Value o = n.tanh();
    o.label("o");
    
    o.backward();
    o.graph();

    return 0;
}
