#include <bits/stdc++.h>
#include "Value.h"

int main()
{
    Value v1(2,"a");
    Value v2(3,"b");

    Value v3 = std::move(v1 - v2);
}
