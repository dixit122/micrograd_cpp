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
    std::shared_ptr<valueData> out_ptr = out.ptr;
    out.ptr->_backward = [out_ptr](){
        out_ptr->children[0]->grad += (out_ptr->children[1]->data)*(out_ptr->grad);
        out_ptr->children[1]->grad += (out_ptr->children[0]->data)*(out_ptr->grad);
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
    Value tmp = Value(val);
    return tmp - other;
}

//power
Value Value::operator^(Value& other){

    Value out = Value();
    out.ptr = std::make_shared<valueData>();
    out.ptr->data = std::pow(this->ptr->data,other.ptr->data);
    out.ptr->label = "^";
    out.ptr->children = {this->ptr, other.ptr};
    std::shared_ptr<valueData> out_ptr = out.ptr;
    out.ptr->_backward = [out_ptr](){
        int this_data = out_ptr->children[0]->data;
        int other_data = out_ptr->children[1]->data;
        out_ptr->children[0]->grad += (std::pow(this_data, other_data) * other_data / this_data) * out_ptr->grad;
        out_ptr->children[1]->grad += (std::pow(this_data, other_data) * log(this_data)) * out_ptr->grad;
    };
    return out;
}

Value Value::operator^(double val){

    Value other = Value(val);
    return ((*this) ^ (other));
}

Value operator^(double val, Value& other){

    Value tmp = Value(val);
    return tmp ^ other;
}
