#pragma once

#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <memory>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

class Value {
   public:
    Value(const float value = 0.0, const std::string label = "", const std::vector<Value*>& children = {}, const std::string& op = "")
        : _data{value}, _prev{children}, _op{op}, _label{label}, _backward{[]() {}} {}

    ~Value() = default;

    // << overload for printing
    friend std::ostream& operator<<(std::ostream& os, const Value& obj) {
        os << obj._label << " = " << obj._data;
        return os;
    }

    // for changing label
    Value& operator=(const std::string& label) {
        _label = label;
        return *this;
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
        float t = std::tanh(this->_data);
        Value out(t, {}, std::vector<Value*>{this}, "tanh");
        out._backward = [&, t]() {
            this->_grad += (1 - t * t) * out._grad;
        };
        return out;
    }

    void backward() {
        std::vector<Value*> topo;
        std::set<Value*> visited;
        build_topo(this, topo, visited);
        _grad = 1;

        for (auto it = topo.rbegin(); it != topo.rend(); ++it) {
            Value* v = *it;
            v->_backward();
        }
    }

    void graph(int precision = 4) {
        std::unordered_set<const Value*> visited;
        visited.insert(this);
        graph(*this, visited, 0, precision);
    }

    float get() const { return _data; }
    float get_grad() const { return _grad; }
    const std::vector<Value*>& get_prev() const { return _prev; }
    const std::string& get_op() const { return _op; }
    const std::string& get_label() const { return _label; }

    void set_data(const float data) { _data = data; }
    void label(const std::string label) { _label = label; }
    void grad(const float grad) { _grad = grad; }

   private:
    float _data;
    mutable float _grad{};
    const std::vector<Value*> _prev;  // might need to converted to set for efficiency
    const std::string _op;
    std::string _label;
    std::function<void()> _backward;

    // print the graph of a value node
    void graph(const Value& v, std::unordered_set<const Value*>& visited, int indent, int precision) const {
        std::cout << std::string(indent, ' ') << "+---" << v.get_label() << "[ " << std::fixed << std::setprecision(precision) << v.get() << " ][ " << v.get_grad() << " ]" << (v.get_op().empty() ? "" : "(" + v.get_op() + ")") << std::endl;
        for (const Value* prev : v.get_prev()) {
            if (!visited.count(prev)) {
                visited.insert(prev);
                graph(*prev, visited, indent + 4, precision);
            } else {
                std::cout << std::string(indent + 4, ' ') << "+---" << prev->get_label() << std::endl;
            }
        }
    }

    // topological sort
    void build_topo(Value* v, std::vector<Value*>& topo, std::set<Value*>& visited) {
        if (visited.find(v) == visited.end()) {
            visited.insert(v);

            for (Value* child : v->_prev) {
                build_topo(child, topo, visited);
            }

            topo.push_back(v);
        }
    }
};