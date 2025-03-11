#include "Value.h"

/* for printing the Value object (right now implemented by overloading the ostream << operator)*/
std::ostream& operator<<(std::ostream &out_stream, Value &obj) noexcept {
    out_stream << obj.ptr->label << " = " << obj.ptr->data << " | grad = " << obj.ptr->grad << std::endl;
    return out_stream;
}

/* operation definitions for Value objects */

// addition
Value Value::operator+(Value &other){
    Value out = Value();
    out.ptr->data = this->ptr->data + other.ptr->data;
    out.ptr->children = {this->ptr,other.ptr};
    out.ptr->op = "+";
    std::weak_ptr<valueData> out_ptr = out.ptr;
    out.ptr->_backward = [out_ptr](){
        //local_out = x + y
        //dlocal_out/dx = 1
        //dlocal_out/dy = 1
        //dloss/dlocal_out = out.grad
        //dloss/dx = dloss/dlocal_out * dlocal_out/dx = out.grad * 1.0
        //dloss/dy = dloss/dlocal_out * dlocal_out/dy = out.grad * 1.0
        if(auto out = out_ptr.lock()){
            out->children[0]->grad += double(1.0) * out->grad;
            out->children[1]->grad += double(1.0) * out->grad;
        }

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
    out.ptr->data = this->ptr->data * other.ptr->data;
    out.ptr->label = "*";
    out.ptr->children = {this->ptr,other.ptr};
    Value &out_ref = out;
    std::weak_ptr<valueData> out_ptr = out.ptr;
    out.ptr->_backward = [out_ptr](){
        //local_out = x * y
        //dlocal_out/dx = y
        //dlocal_out/ay = x
        //dloss/dlocal_out = out.grad
        //dloss/dx = dloss/dlocal_out * dlocal_out/dx = out.grad * y
        //dloss/dy = dloss/dlocal_out * dlocal_out/dy = out.grad * x
        if(auto out = out_ptr.lock()){
            auto x_data = out->children[0]->data;
            auto y_data = out->children[1]->data;
            out->children[0]->grad += (y_data)*(out->grad);
            out->children[1]->grad += (x_data)*(out->grad);
        }
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
    Value tmp = other * (-1.0);
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
    out.ptr->data = std::pow(this->ptr->data,other.ptr->data);
    out.ptr->children = {this->ptr,other.ptr};
    out.ptr->op = "^";
    std::weak_ptr<valueData> out_ptr = out.ptr;
    out.ptr->_backward = [out_ptr](){
        //local_out = x ^ y
        //dlocal_out/dx = y * (x^(y-1))
        //dlocal_out/dy = ln(x) * (x^y)
        //dloss/dlocal_out = out.grad
        //dloss/dx = dloss/dlocal_out * dlocal_out/dx = out.grad * (y*(x^(y-1)))
        //dloss/dy = dloss/dlocal_out * dlocal_out/dy = out.grad * (ln(x)*(x^y))
        if(auto out = out_ptr.lock()){
            auto x_data = out->children[0]->data;
            auto y_data = out->children[1]->data;
            out->children[0]->grad += y_data * std::pow(x_data,y_data-1) * out->grad;
            out->children[1]->grad += (std::log(x_data) * out->data * out->grad);
        }
    };
    return out;
}

Value Value::operator^(double val){
    Value other = Value(val);
    return (*this)^(other);
}

Value operator^(double val,Value& other){
    Value tmp = Value(val);
    return tmp ^ other;
}

//division
Value Value::operator/(Value& other){
    Value tmp = other ^ (-1.0);
    return (*this) * tmp;
}

Value Value::operator/(double val){
    Value other = Value(val);
    return (*this) / (other);
}

Value operator/(double val,Value& other){
    Value tmp = Value(val);
    return tmp / other;
}

//tanh
Value Value::tanh(){
    Value out = Value();
    out.ptr->data = std::tanh(this->ptr->data);
    out.ptr->children = {this->ptr,nullptr};
    out.ptr->op = "tanh";
    std::weak_ptr<valueData> out_ptr = out.ptr;

    out.ptr->_backward = [out_ptr](){
        if(auto out = out_ptr.lock()){
            double this_data = out->data;
            out->children[0]->grad += 1.0 - (this_data * this_data);
        }
    };
    return out;
}
