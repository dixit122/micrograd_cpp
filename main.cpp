#include <bits/stdc++.h>
#include <cassert>
#include <cmath>
#include <functional>
#include <memory>

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
    Value(const Value& other)
        : data(other.data),
        label(other.label),
        children(other.children),
        grad(other.grad),
        _backward(other._backward),
        op(other.op) {
    }

    /* explicitely defining the default constuctor */
    Value() = default;

    /* main constructor */
    Value(double _data,std::string _label = "",std::vector<std::shared_ptr<Value>> _children = {nullptr,nullptr},std::string _op = "",std::function<void(Value&)> __backward = nullptr):data(_data),label(std::move(_label)),children(std::move(_children)),op(std::move(_op)),_backward(__backward),grad(double(0.0)) {}

    /* move constructor */
    Value(Value &&other) noexcept : data(std::move(other.data)),label(std::move(other.label)),children(std::move(other.children)),op(std::move(other.op)),_backward(std::move(other._backward)),grad(std::move(other.grad)){std::cout << "move constructor called\n";}


    /* assignment move constructor */
    Value& operator=(Value &&other) noexcept  {

        std::cout << "move operator = called\n";

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
    friend std::ostream& operator<<(std::ostream &out_stream, Value &obj) noexcept {
        out_stream << obj.label << " = " << obj.data << "\n";
        return out_stream;
    }

    /* operation definitions for Value objects */

    // addition
    Value& operator+(Value &other){

        Value* out = new Value();
        out->data = this->data + other.data;
        std::vector<std::shared_ptr<Value>> __children = {std::make_shared<Value>(*this),std::make_shared<Value>(other)};
        out->op = "+";
        out->_backward = [&](Value& this_ref){
            //local_out = a + b
            //dlocal_out/da = 1
            //dlocal_out/db = 1
            //dloss/dlocal_out = out.grad
            //dloss/da = dloss/dlocal_out * dlocal_out/da = out.grad * 1.0
            //dloss/db = dloss/dlocal_out * dlocal_out/db = out.grad * 1.0
            this->grad += double(1.0) * this_ref.grad;
            other.grad += double(1.0) * this_ref.grad;
        };

        /* return Value(new_data,"",__children,__op,__backward); */
        return *out;
    }

    // for supporting the Value + double
    Value& operator+(double val){

        Value *val_obj = new Value(val);
        return (*this + *val_obj);
    }

    // for supporting the double + Value
    friend Value& operator+(double val,Value &other){

        return other + val;
    }


    // multiplication
    Value& operator*(Value& other){

        Value *out = new Value();
        out->data = this->data * other.data;
        out->label = "";
        out->children = {std::make_shared<Value>(*this),std::make_shared<Value>(other)};
        Value &out_ref = *out;
        out->_backward = [&](Value &this_ref){
            //local_out = a*b
            //dlocal_out/da = b
            //dlocal_out/ab = a
            //dloss/dlocal_out = out.grad
            //dloss/da = dloss/dlocal_out * dlocal_out/da = out.grad * b
            //dloss/db = dloss/dlocal_out * dlocal_out/db = out.grad * a
            std::cout << this->data << " " << other.data << " " << this_ref.grad << '\n';
            this->grad += (other.data)*(this_ref.grad);
            other.grad += (this->data)*(this_ref.grad);
        };
        return *out;
    }

    Value& operator*(double val){
        std::cout << "value - double is called\n";
        Value *other = new Value(val);
        return (*this)*(*other);
    }

    friend Value& operator*(double val,Value& other){
        return other * val;
    }


    // substraction
    Value& operator-(Value& other){
        std::cout << "value - value is called\n";
        Value *tmp = new Value();
        *tmp = std::move(other * (-1.0));
        return ((*this) + *tmp);
    }

    Value& operator-(double val) {
        Value *other = new Value(val);
        return ((*this) - (*other));
    }

    friend Value& operator-(double val,Value& other){
        return other - val;
    }

    //power
    Value& operator^(double val){
        Value* new_obj = new Value();
        new_obj->data = std::pow(this->data,val);
        new_obj->children = std::vector<std::shared_ptr<Value>>({std::make_shared<Value>(*this),nullptr});
        new_obj->op = "^";
        new_obj->_backward = [&](Value& this_ref){
            //local_out = a^val
            //dlocal_out/da = val*(a^(val-1))
            //dloss/dlocal_out = out.grad
            //dloss/da = dloss/dlocal_out * dlocal_out/da = out.grad * (val*(a^(val-1)))
            this->grad = (val*(std::pow(this->data,val-1)))* this_ref.grad;
        };
        return *new_obj;
    }

};

int main()
{
    Value v1(2,"a");
    Value v2(3,"b");

    Value v3 = std::move(v1 - v2);
}
