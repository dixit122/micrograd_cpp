#include "Value.h"
#include <iostream>
#include <memory>


/* for printing the Value object (right now implemented by overloading the ostream << operator)*/
std::ostream& operator<<(std::ostream &out_stream, Value &obj) noexcept {
    out_stream << obj.ptr->label << " = " << obj.ptr->data << " | grad = " << obj.ptr->grad << std::endl;
    return out_stream;
}

/* operation definitions for Value objects */

// addition
Value Value::operator+(Value &other){
    Value out = Value();
    out.ptr = std::make_shared<valueData>();
    out.ptr->data = this->ptr->data + other.ptr->data;
    out.ptr->children = {this->ptr,other.ptr};
    out.ptr->op = "+";
    out.ptr->_backward = [&](valueData& this_ref){
        //local_out = a + b
        //dlocal_out/da = 1
        //dlocal_out/db = 1
        //dloss/dlocal_out = out.grad
        //dloss/da = dloss/dlocal_out * dlocal_out/da = out.grad * 1.0
        //dloss/db = dloss/dlocal_out * dlocal_out/db = out.grad * 1.0
        this->ptr->grad += double(1.0) * this_ref.grad;
        other.ptr->grad += double(1.0) * this_ref.grad;
    };
    return out;
}

// for supporting the Value + double
Value Value::operator+(double val){
    Value val_obj = Value(val);
    return (*this + val_obj);
}

// for supporting the double + Value
Value operator+(double val,Value &other){
    return other + val;
}


// multiplication
Value Value::operator*(Value& other){
    Value out = Value();
    out.ptr = std::make_shared<valueData>();
    out.ptr->data = this->ptr->data * other.ptr->data;
    out.ptr->label = "*";
    out.ptr->children = {this->ptr,other.ptr};
    Value &out_ref = out;
    out.ptr->_backward = [&](valueData &this_ref){
        //local_out = a*b
        //dlocal_out/da = b
        //dlocal_out/ab = a
        //dloss/dlocal_out = out.grad
        //dloss/da = dloss/dlocal_out * dlocal_out/da = out.grad * b
        //dloss/db = dloss/dlocal_out * dlocal_out/db = out.grad * a
        this->ptr->grad += (other.ptr->data)*(this_ref.grad);
        other.ptr->grad += (this->ptr->data)*(this_ref.grad);
    };
    return out;
}

Value Value::operator*(double val){
    Value other = Value(val);
    return (*this)*(other);
}

Value operator*(double val,Value& other){
    return other * val;
}


// substraction
Value Value::operator-(Value& other){
    Value tmp = Value();
    tmp = other * (-1.0);
    return ((*this) + tmp);
}

Value Value::operator-(double val) {
    Value other = Value(val);
    return ((*this) - (other));
}

Value operator-(double val,Value& other){
    return other - val;
}

//power
Value Value::operator^(double val){
    Value out = Value();
    out.ptr = std::make_shared<valueData>();
    out.ptr->data = std::pow(this->ptr->data,val);
    out.ptr->children = {this->ptr,nullptr};
    out.ptr->op = "^";
    out.ptr->_backward = [&](valueData& this_ref){
        //local_out = a^val
        //dlocal_out/da = val*(a^(val-1))
        //dloss/dlocal_out = out.grad
        //dloss/da = dloss/dlocal_out * dlocal_out/da = out.grad * (val*(a^(val-1)))
        this->ptr->grad = (val*(std::pow(this->ptr->data,val-1)))* this_ref.grad;
    };
    return out;
}
