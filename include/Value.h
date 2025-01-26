#pragma once
#include <bits/stdc++.h>

class Value
{

public:
    double data;
    std::string label;
    std::vector<std::shared_ptr<Value>> children;
    double grad;
    std::function<void(Value&)> _backward;
    std::string op;

    // copy constructor
    Value(const Value& other) noexcept: data(other.data),label(other.label),children(other.children),grad(other.grad),_backward(other._backward),op(other.op) {}

    /* explicitely defining the default constuctor */
    Value() = default;

    /* main constructor */
    Value(double _data,std::string _label = "",std::vector<std::shared_ptr<Value>> _children = {nullptr,nullptr},std::string _op = "",std::function<void(Value&)> __backward = nullptr):data(_data),label(std::move(_label)),children(std::move(_children)),op(std::move(_op)),_backward(__backward),grad(double(0.0)) {}

    /* move constructor */
    Value(Value&& other) noexcept : data(std::move(other.data)),label(std::move(other.label)),children(std::move(other.children)),op(std::move(other.op)),_backward(std::move(other._backward)),grad(std::move(other.grad)){}


    /* assignment move constructor */
    Value& operator=(Value&& other) noexcept {
        this->data = other.data;
        this->label = std::move(other.label);
        this->children = std::move(other.children);
        this->_backward = std::move(other._backward);
        this->op = std::move(other.op);
        this->grad = other.grad;
        return *this;
    }

    /* destructor */
    ~Value(){}

    /* for printing the Value object (right now implemented by overloading the ostream << operator)*/
    friend std::ostream& operator<<(std::ostream&, Value&) noexcept;

    /* operation definitions for Value objects */

    // addition
    Value& operator+(Value&);
    Value& operator+(double);
    friend Value& operator+(double, Value&);


    // multiplication
    Value& operator*(Value&);
    Value& operator*(double);
    friend Value& operator*(double, Value&);

    // substraction
    Value& operator-(Value&);
    Value& operator-(double);
    friend Value& operator-(double, Value&);

    //power
    Value& operator^(double);
};
