#include <bits/stdc++.h>
#include <functional>
#include <memory>

class value
{

public:
    double data;
    std::string label;
    std::vector<std::shared_ptr<value>> children;
    double grad;
    std::function<void(void)> _backward;
    std::string op;

/* disabling the default copy constructor */
/*     value(const value& other) = delete; */
 // copy constructor
    value(const value& other) 
        : data(other.data), 
        label(other.label), 
        children(other.children), 
        grad(other.grad), 
        _backward(other._backward), 
        op(other.op) {
/*         std::cout << "copy constructor called\n"; */
    }
/* explicitely defining the default constuctor */
    value() = default;

/* main constructor */
    value(double _data,std::string _label = "",std::vector<std::shared_ptr<value>> _children = {nullptr,nullptr},std::string _op = "",std::function<void(void)> __backward = nullptr):data(_data),label(std::move(_label)),children(std::move(_children)),op(std::move(_op)),_backward(__backward),grad(double(0.0)) {}

/* move constructor */
    value(value &&other) noexcept : data(std::move(other.data)),label(std::move(other.label)),children(std::move(other.children)),op(std::move(other.op)),_backward(std::move(other._backward)),grad(std::move(other.grad)){std::cout << "move constructor called\n";}


/* assignment move constructor */
    value& operator=(value &&other) noexcept  {

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
    ~value(){
        /* for(std::shared_ptr<value> child: children){
            child.reset();
        } */
    }

/* for printing the value object (right now implemented by overloading the ostream << operator)*/
    friend std::ostream& operator<<(std::ostream &out_stream, value &obj) noexcept {
        out_stream << obj.label << " = " << obj.data << "\n";
        return out_stream;
    }

/* operation definitions for value objects */

    // addition

    value& operator+(value &other){

        value* out = new value();
        out->data = this->data + other.data;
        std::vector<std::shared_ptr<value>> __children = {std::make_shared<value>(*this),std::make_shared<value>(other)};
        out->op = "+";
        out->_backward = [&](void){
            //local_out = a + b
            //dlocal_out/da = 1
            //dlocal_out/db = 1
            //dloss/dlocal_out = out.grad
            //dloss/da = dloss/dlocal_out * dlocal_out/da = out.grad * 1.0
            //dloss/db = dloss/dlocal_out * dlocal_out/db = out.grad * 1.0
            this->grad += double(1.0);
            other.grad += double(1.0); 
        };

        /* return value(new_data,"",__children,__op,__backward); */
        return *out;
    }

    // for supporting the value + double
    value& operator+(double val){

        value* val_obj = new value(val);
        return (*this + *val_obj);
    }

    // for supporting the double + value 
    friend value& operator+(double val,value &other){

        return other + val;
    }


    // multiplication

    value& operator*(value& other){

        value *out = new value();
        out->data = this->data * other.data;
        out->label = "";
        out->children = {std::make_shared<value>(*this),std::make_shared<value>(other)};
        value &out_ref = *out;
        out->_backward = [out_ref,this,&other](void){
            //local_out = a*b
            //dy/da = b
            //dy/ab = a
            //dloss/dlocal_out = out.grad
            //dloss/da = dloss/dlocal_out * dlocal_out/da = out.grad * b
            //dloss/db = dloss/dlocal_out * dlocal_out/db = out.grad * a
            //std::cout << "backward called\n";
            //std::cout << this->data << " " << other.data << " " << out->grad << '\n';
            this->grad += (other.data)*(out_ref.grad);
            other.grad += (this->data)*(out_ref.grad);
        };
        std::cout << "logs : " << &(out->grad) << "\n";
        std::cout << "v1.grad : " << &(this->grad) << "\n";
        std::cout << "v2.grad : " << &(other.grad) << "\n";
        return *out;
    }
};

int main()
{
    value v1(2,"a");
    value v2(3,"b");

    value v3 = std::move(v1 * v2);

    std::cout << "logs : " << &(v3.grad) << "\n";
    std::cout << "v1.grad : " << &(v1.grad) << "\n";
    std::cout << "v2.grad : " << &(v2.grad) << "\n";
    v3.grad = 2.0;
    v3._backward();

    std::cout << v3;
    std::cout << v1.grad << " " << v2.grad << " " << v3.grad << "\n";
}
