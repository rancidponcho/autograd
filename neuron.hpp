#pragma once

#include "value.hpp"

// std
#include <random>
#include <stdexcept>

class Neuron {
   public:
    Neuron(int num_in) : bias{random(), "b"} {
        weights.reserve(num_in);
        for (int i = 0; i < num_in; ++i) {
            weights.emplace_back(random(), "w" + std::to_string(i));
        }
    }

    // void operator()(std::vector<Value>& x) {
    //     if (!weights.size() == x.size()) {
    //         throw std::runtime_error("Neuron: input size does not match weight size");
    //     }
    //     std::vector<Value> dotted(weights.size());
    //     for (int i = 0; i < weights.size(); ++i) {
    //         // dot product
    //         Value v = weights[i] * x[i];
    //         v.label("w" + std::to_string(i) + "x" + std::to_string(i));
    //         multiplied[i] = v;
    //     }
    // }

   private:
    std::vector<Value> weights;
    std::vector<Value> multiplied;
    Value bias;

    static float random() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(-1, 1);
        return dis(gen);
    }
};