#include <bits/stdc++.h>
#include "Value.h"

int main()
{
    Value a(2,"a");
    Value b(3,"b");
    Value c(4,"c");
    Value d(5,"d");

    Value e = a + b;
    Value f = a * b;
    Value g = e * f;

/*     g.backward(); */

/*     std::cout << a << b << c << d << e << f << g; */
}
