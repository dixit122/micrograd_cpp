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
    out->children = {this,&other};
    out->op = "+";
    out->_backward = [&](Value& this_ref){
        this->grad += double(1.0) * this_ref.grad;
        other.grad += double(1.0) * this_ref.grad;
        std::cout << "HERE: " << this->data << " " << this->grad << "  ---  " << other.data << " "  << other.grad << '\n';
    };
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
    out->children = {this,&other};
    Value &out_ref = *out;
    out->_backward = [&](Value &this_ref){
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

    return val + (-1 * other);
}


//power
Value& Value::operator^(Value& other){

    Value* new_obj = new Value();
    new_obj->data = std::pow(this->data,other.data);
    // new_obj->children = std::vector<Value *>({this,&other});
    new_obj->children = {this,&other};
    new_obj->op = "^";
    new_obj->_backward = [&](Value& this_ref){
        // this->grad += (other.data*(std::pow(this->data,other.data-1)))* this_ref.grad;
        // other.grad += (log(this->data) * new_obj->data)* this_ref.grad;
        this->grad += (std::pow(this->data, other.data) * other.data / this->data) * this_ref.grad;
        other.grad += (std::pow(this->data, other.data) * log(this->data)) * this_ref.grad;
    };
    return *new_obj;
}

Value& Value::operator^(double val){

    Value *val_obj = new Value(val);
    return ((*this) ^ (*val_obj));
}

Value& operator^(double val, Value& other){

    Value *new_val = new Value(val);
    return (*new_val) ^ other;
}

/*Functions and helpers for calculating grad*/
// Topo Sort
void Value::__topoSort(Value &node,  std::vector<Value*> &order){

    for(auto &child: node.children){
        // if(child != nullptr){
            child->__topoSort(*child, order);
        // }
    }
    order.push_back(&node);
}

//Backward 
void Value::backward(){

    std::vector<Value *> order;
    this->__topoSort(*this, order);
    std::reverse(order.begin(), order.end());
    this->grad = 1;
    printf("%p %p\n", &(this->children[0]), &(order[0]->children[0]));
    // printf("%p %p\n", );
    for(auto node: order){
        // std::cout << "This one: " << node->data << '\n';
        if(node->_backward != nullptr){
            node->_backward(*node);
            // std::cout << node->data << " " << node->grad << '\n';
        }
        // node->data = 11;
    }
}