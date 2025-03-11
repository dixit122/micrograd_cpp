#include <bits/stdc++.h>
#include "Value.h"

int main()
{
    Value a(10,"a");
    Value b(5,"b");

    Value c = a + b;
    Value d = c + c;

    d.backward();
    std::cout << a << b << c << d << std::endl;
}
