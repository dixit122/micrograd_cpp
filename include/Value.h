#pragma once
#include <bits/stdc++.h>
#include <memory>
#include <vector>


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
    void topoSort(std::shared_ptr<valueData>& currNode,std::vector<std::shared_ptr<valueData>> &topoSequence,std::set<std::shared_ptr<valueData>> &visited){
        if(currNode == nullptr){
            return;
        }
        visited.insert(currNode);
        for(auto child: currNode->children){
            if(!visited.count(child)){
                topoSort(child, topoSequence, visited);
            }
        }
        topoSequence.push_back(currNode);
    }


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
        std::vector<std::shared_ptr<valueData>> topoSequence;
        std::set<std::shared_ptr<valueData>> visited;
        topoSort(this->ptr,topoSequence,visited);
        for(auto it=topoSequence.rbegin();it != topoSequence.rend();++it){
            if((*it)->_backward)
                (*it)->_backward();
        }
    }

    /* reset all grads function */
    void resetAllGrads(){
        std::vector<std::shared_ptr<valueData>> topoSequence;
        std::set<std::shared_ptr<valueData>> visited;
        topoSort(this->ptr,topoSequence,visited);
        for(auto it=topoSequence.rbegin();it != topoSequence.rend();++it){
            (*it)->grad = 0.0;
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
};
