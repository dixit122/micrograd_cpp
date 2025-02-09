#include <bits/stdc++.h>
#include "Value.h"

int main()
{
    Value a(2,"a");
    Value b(3,"b");
    Value c(4,"c");
    Value d(5,"d");

/*     Value e = std::move(a); */
    Value e = a + b;e.ptr->label = "e";
    Value f = c * d;f.ptr->label = "f";
    Value g = e * f;g.ptr->label = "g";

    g.backward();

    std::cout << a << b << c << d << e << f << g;
}
