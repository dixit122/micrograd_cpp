#include <iostream>
#include "Value.h"


void printer(std::vector<Value> v){
    std::cout << "Data:\t\t";
    for(int i = 0; i < v.size(); i++){
        std::cout << v[i].data << " ";
    }
    std::cout << "\nData Addr:\t";
    for(int i = 0; i < v.size(); i++){
        std::cout << &v[i].data << " ";
    }
    std::cout << "\nAddress:\t";
    for(int i = 0; i < v.size(); i++){
        std::cout << &v[i] << " ";
    }
    std::cout << "\nGrad:\t\t";
    for(int i = 0; i < v.size(); i++){
        std::cout << v[i].grad << " | ";
    }
    std::cout << '\n';
}

int main()
{
    // Value v0(2,"a");
    // // Value v1(2,"b");
    // Value v1 = v0^v0;
    // Value v2(4, "b");
    // Value v3 = v1 + v2;
    // v3.backward();
    // printer({v0, v1, v2, v3});
    // Value v1(2,"b");
    
    Value v0(2, "a");
    Value v1(3, "b");
    Value v2(4, "c");
    Value v3(5, "d");

    Value e = v0+v1;
    Value f = v2*v3;
    Value g = e*f;
    g.backward();
    printer({v0,v1,v2,v3,e,f,g});
    // printer({v0, v
}
