#pragma once
#include <memory>
#include <vector>
#include <functional>
#include <iostream>


class valueData{

public:
    double data;
    std::string label;
    std::vector<std::shared_ptr<valueData>> children;
    double grad;
    std::function<void()>_backward;
    std::string op;

    /* default constructor */
    valueData() = default;

    /* main constructor */
    valueData(double _data,std::string _label = "",std::vector<std::shared_ptr<valueData>> _children = {nullptr,nullptr},std::string _op = "",std::function<void()> __backward = nullptr):data(_data),label(std::move(_label)),children(std::move(_children)),op(std::move(_op)),_backward(__backward),grad(double(0.0)) {}

    /* destructor */
    ~valueData(){}
};

class Value
{
private:
    inline static std::vector<std::shared_ptr<valueData>> tmpObjs;

public:
    std::shared_ptr<valueData> ptr;

    /* explicitely defining the default constuctor */
    Value() = default;

    /* main constructor */
    Value(double _data,std::string _label = "",std::vector<std::shared_ptr<valueData>> _children = {nullptr,nullptr},std::string _op = "",std::function<void()> __backward = nullptr){
        ptr = std::make_shared<valueData>(_data,_label,_children,_op,__backward);
    }

    /* move constructor */
    Value(Value&& other){
        this->ptr = other.ptr;
        other.ptr.reset();
    }

    // copy constructor
    Value(const Value& other){
        this->ptr = std::make_shared<valueData>(*(other.ptr));
    }

    /* assignment copy constructor */
    Value& operator=(const Value& other){
        if(this != &other){
            std::cout << "assignment operator called\n";
            if(this->ptr != nullptr){
                Value::tmpObjs.push_back(this->ptr);
            }
            this->ptr = other.ptr;
        }
        return *this;
    }

    /* assignment move constructor */
    Value operator=(Value&& other) noexcept {
        if(this != &other){
            std::cout << "assignment move constructor called\n";
            if(this->ptr != nullptr){
                Value::tmpObjs.push_back(this->ptr);
            }
            this->ptr = other.ptr;
            other.ptr.reset();
        }
        return *this;
    }

    /* destructor */
    ~Value(){}

    /* getter functions */

    double getdata(){
        return this->ptr->data;
    }

    double getGrad(){
        return this->ptr->grad;
    }

    /* for printing the Value object (right now implemented by overloading the ostream << operator)*/
    friend std::ostream& operator<<(std::ostream&, Value&) noexcept;

    /* backward function */
    void backward(){

        this->ptr->grad = 1.0;
        std::queue<std::shared_ptr<valueData>> q;

        q.push(std::make_shared<valueData>(*(this->ptr)));

        while(!q.empty()){
            std::shared_ptr<valueData> top_child = q.front();
            q.pop();

            if(top_child->_backward != nullptr){
                top_child->_backward();

                for(auto child: top_child->children){
                    if(child != nullptr){
                        q.push(child);
                    }
                }
            }
        }
    }

    /* operation definitions for Value objects */

    // addition
    Value operator+(Value&);
    Value operator+(double);
    friend Value operator+(double, Value&);

    // multiplication
    Value operator*(Value&);
    Value operator*(double);
    friend Value operator*(double, Value&);

    // substraction
    Value operator-(Value&);
    Value operator-(double);
    friend Value operator-(double, Value&);

    //power
    Value operator^(Value&);
    Value operator^(double);
    friend Value operator^(double, Value&);
};
