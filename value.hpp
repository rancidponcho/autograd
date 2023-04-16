#pragma once

#include <functional>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Value {
   public:
    Value(const double value, const std::string label = "", const std::vector<Value*>& children = {}, const std::string& op = "")
        : _data{value}, _prev{children}, _op{op}, _label{label}, _backward{[]() {}} {}

    // << overload for printing
    friend std::ostream& operator<<(std::ostream& os, const Value& obj) {
        os << "Value(data=" << obj._data << ")";
        return os;
    }

    Value operator+(Value& other) {
        Value out(this->_data + other._data, {}, std::vector<Value*>{this, &other}, "+");
        out._backward = [&]() {
            this->_grad += out._grad;
            other._grad += out._grad;
        };
        return out;
    }

    Value operator*(Value& other) {
        Value out(this->_data * other._data, {}, std::vector<Value*>{this, &other}, "*");
        out._backward = [&]() {
            this->_grad += other._data * out._grad;
            other._grad += this->_data * out._grad;
        };
        return out;
    }

    Value tanh() {
        double t = std::tanh(this->_data);
        Value out(t, {}, std::vector<Value*>{this}, "tanh");
        out._backward = [&]() {
            double dtanh = 1.0 - t * t;
            this->_grad += out._grad * dtanh;
        };
        return out;
    }

    void backward() { _backward(); }

    double get() const { return _data; }
    double get_grad() const { return _grad; }
    const std::vector<Value*>& get_prev() const { return _prev; }
    const std::string& get_op() const { return _op; }
    const std::string& get_label() const { return _label; }

    void label(const std::string label) { _label = label; }
    void grad(const double grad) { _grad = grad; }

   private:
    const double _data;
    mutable double _grad{};
    const std::vector<Value*> _prev;  // might need to converted to set for efficiency
    const std::string _op;
    std::string _label;
    std::function<void()> _backward;
};