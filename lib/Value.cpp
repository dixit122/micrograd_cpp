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
    std::shared_ptr<valueData> out_ptr = out.ptr;
    out.ptr->_backward = [out_ptr](){
        //local_out = a + b
        //dlocal_out/da = 1
        //dlocal_out/db = 1
        //dloss/dlocal_out = out.grad
        //dloss/da = dloss/dlocal_out * dlocal_out/da = out.grad * 1.0
        //dloss/db = dloss/dlocal_out * dlocal_out/db = out.grad * 1.0
        out_ptr->children[0]->grad += double(1.0) * out_ptr->grad;
        out_ptr->children[1]->grad += double(1.0) * out_ptr->grad;
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
    std::shared_ptr<valueData> out_ptr = out.ptr;
    out.ptr->_backward = [out_ptr, this](){
        //local_out = a*b
        //dlocal_out/da = b
        //dlocal_out/ab = a
        //dloss/dlocal_out = out.grad
        //dloss/da = dloss/dlocal_out * dlocal_out/da = out.grad * b
        //dloss/db = dloss/dlocal_out * dlocal_out/db = out.grad * a
        out_ptr->children[0]->grad += (out_ptr->children[1]->data)*(out_ptr->grad);
        out_ptr->children[1]->grad += (this->ptr->data)*(out_ptr->grad);
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

