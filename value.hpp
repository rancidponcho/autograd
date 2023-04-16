#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <iomanip>

class Value {
   public:
    Value(const double value, const std::string label = "", const std::vector<Value>& children = {}, const std::string& op = "")
        : _data{value}, _prev{children}, _op{op}, _label{label} {}

    // << overload for printing
    friend std::ostream& operator<<(std::ostream& os, const Value& obj) {
        os << "Value(data=" << obj._data << ")";
        return os;
    }

    // +
    Value operator+(const Value& other) const {
        return Value(this->_data + other._data, {}, std::vector<Value>{*this, other}, "+");  // idk if this fixes the label parameter or what
    }

    // *
    Value operator*(const Value& other) const {
        return Value(this->_data * other._data, {}, std::vector<Value>{*this, other}, "*");
    }

    Value tanh() {
        return Value(std::tanh(this->_data), {}, std::vector<Value>{*this}, "tanh");  // tanh = (exp(x) - exp(-x)) / (exp(x) + exp(-x))
    }

    double get() const { return _data; }
    double get_grad() const { return _grad; }
    const std::vector<Value>& get_prev() const { return _prev; }
    const std::string& get_op() const { return _op; }
    const std::string& get_label() const { return _label; }

    void label(const std::string label) { _label = label; }
    void grad(const double grad) { _grad = grad; }

   private:
    const double _data;
    double _grad{};
    const std::vector<Value> _prev;  // might need to converted to set for efficiency
    const std::string _op;
    std::string _label;
};