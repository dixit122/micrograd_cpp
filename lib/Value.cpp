#include "Value.h"


/* for printing the Value object (right now implemented by overloading the ostream << operator)*/
std::ostream& operator<<(std::ostream &out_stream, Value &obj) noexcept {
    out_stream << obj.label << " = " << obj.data << "\n";
    return out_stream;
}

/* operation definitions for Value objects */

// addition
Value& Value::operator+(Value &other){

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
Value& Value::operator+(double val){

    Value *val_obj = new Value(val);
    return (*this + *val_obj);
}

// for supporting the double + Value
Value& operator+(double val,Value &other){

    return other + val;
}


// multiplication
Value& Value::operator*(Value& other){

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

Value& Value::operator*(double val){
    Value *other = new Value(val);
    return (*this)*(*other);
}

Value& operator*(double val,Value& other){
    return other * val;
}


// substraction
Value& Value::operator-(Value& other){
    Value *tmp = new Value();
    *tmp = std::move(other * (-1.0));
    return ((*this) + *tmp);
}

Value& Value::operator-(double val) {
    Value *other = new Value(val);
    return ((*this) - (*other));
}

Value& operator-(double val,Value& other){
    return other - val;
}

//power
Value& Value::operator^(double val){
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
