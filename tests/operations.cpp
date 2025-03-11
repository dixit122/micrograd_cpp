#include "Value.h"
#include <cmath>
#include <gtest/gtest.h>

namespace {

// Test for Addition Operation
TEST(ValueTest, Addition) {
    Value a(2.0, "a");
    Value b(3.0, "b");
    Value c = a + b;
    c.backward();

    EXPECT_DOUBLE_EQ(c.getdata(), 5.0);
    EXPECT_DOUBLE_EQ(a.getGrad(), 1.0);
    EXPECT_DOUBLE_EQ(b.getGrad(), 1.0);
    c.resetAllGrads();

    Value d = a + 2.0;
    d.backward();
    EXPECT_DOUBLE_EQ(d.getdata(), 4.0);
    EXPECT_DOUBLE_EQ(a.getGrad(), 1.0);
    d.resetAllGrads();

    Value e = 2.0 + a;
    e.backward();
    EXPECT_DOUBLE_EQ(e.getdata(), 4.0);
    EXPECT_DOUBLE_EQ(a.getGrad(), 1.0);
}

// Test for Multiplication Operation
TEST(ValueTest, Multiplication) {
    Value a(2.0, "a");
    Value b(3.0, "b");
    Value c = a * b;
    c.backward();

    EXPECT_DOUBLE_EQ(c.getdata(), 6.0);
    EXPECT_DOUBLE_EQ(a.getGrad(), 3.0);
    EXPECT_DOUBLE_EQ(b.getGrad(), 2.0);
    c.resetAllGrads();

    Value d = a * 3.0;
    d.backward();
    EXPECT_DOUBLE_EQ(d.getdata(), 6.0);
    EXPECT_DOUBLE_EQ(a.getGrad(), 3.0);
    d.resetAllGrads();

    Value e = 4.0 * a;
    e.backward();
    EXPECT_DOUBLE_EQ(e.getdata(), 8.0);
    EXPECT_DOUBLE_EQ(a.getGrad(), 4.0);
}

// Test for Subtraction Operation
TEST(ValueTest, Subtraction) {
    Value a(5.0, "a");
    Value b(3.0, "b");
    Value c = a - b;
    c.backward();

    EXPECT_DOUBLE_EQ(c.getdata(), 2.0);
    EXPECT_DOUBLE_EQ(a.getGrad(), 1.0);
    EXPECT_DOUBLE_EQ(b.getGrad(), -1.0);
    c.resetAllGrads();

    Value d = a - 2.0;
    d.backward();
    EXPECT_DOUBLE_EQ(d.getdata(), 3.0);
    EXPECT_DOUBLE_EQ(a.getGrad(), 1.0);
    d.resetAllGrads();

    Value e = 2.0 - a;
    e.backward();
    EXPECT_DOUBLE_EQ(e.getdata(), -3.0);
    EXPECT_DOUBLE_EQ(a.getGrad(), -1.0);
}

// Test for Subtraction Operation
TEST(ValueTest, Power) {
    Value a(5.0, "a");
    Value b(2.0, "b");
    Value c = a ^ b;
    c.backward();

    EXPECT_DOUBLE_EQ(c.getdata(), 25.0);
    EXPECT_DOUBLE_EQ(a.getGrad(), 10.0);
    EXPECT_DOUBLE_EQ(b.getGrad(), log(5.0) * 25.0);
    c.resetAllGrads();

    Value d = a ^ 2.0;
    d.backward();
    EXPECT_DOUBLE_EQ(d.getdata(), 25.0);
    EXPECT_DOUBLE_EQ(a.getGrad(), 10.0);
    d.resetAllGrads();

    Value e = 2.0 ^ a;
    e.backward();
    EXPECT_DOUBLE_EQ(e.getdata(), 32.0);
    EXPECT_DOUBLE_EQ(a.getGrad(), log(2.0) * 32.0);
}

// Test for Subtraction Operation
TEST(ValueTest, Division) {
    Value a(10.0, "a");
    Value b(5.0, "b");
    Value c = a / b;
    c.backward();

    EXPECT_DOUBLE_EQ(c.getdata(), 2.0);
    EXPECT_DOUBLE_EQ(a.getGrad(), 0.2);
    EXPECT_DOUBLE_EQ(b.getGrad(), -0.4);
    c.resetAllGrads();

    Value d = a / 2.0;
    d.backward();
    EXPECT_DOUBLE_EQ(d.getdata(), 5.0);
    EXPECT_DOUBLE_EQ(a.getGrad(), 0.5);
    d.resetAllGrads();

    Value e = 2.0 / a;
    e.backward();
    EXPECT_DOUBLE_EQ(e.getdata(), 0.2);
    EXPECT_DOUBLE_EQ(a.getGrad(), -0.02);
}

// Test for tanh Operation
TEST(ValueTest, tanh) {
    Value a(10.0, "a");
    Value b(5.0, "b");
    Value c = a / b;
    Value d = c.tanh();
    d.backward();

    EXPECT_DOUBLE_EQ(d.getdata(), std::tanh(2.0));
    EXPECT_DOUBLE_EQ(c.getGrad(), 1.0 - d.getdata()*d.getdata());
    EXPECT_DOUBLE_EQ(a.getGrad(), 0.2 * c.getGrad());
    EXPECT_DOUBLE_EQ(b.getGrad(), -0.4 * c.getGrad());
}



// Test for Mixed Operations (e.g., multiplication followed by addition)
TEST(ValueTest, MixedOperations) {
    Value a(2.0, "a");
    Value b(3.0, "b");
    Value c(4.0, "c");
    Value d = a * b + c;
    d.backward();

    EXPECT_DOUBLE_EQ(d.getdata(), 10.0);
    EXPECT_DOUBLE_EQ(a.getGrad(), 3.0);
    EXPECT_DOUBLE_EQ(b.getGrad(), 2.0);
    EXPECT_DOUBLE_EQ(c.getGrad(), 1.0);
}

// Test for Operations with Constants
TEST(ValueTest, OperationsWithConstants) {
    Value a(2.0, "a");
    Value b = a + 3.0;
    Value c = 4.0 * b;
    c.backward();

    EXPECT_DOUBLE_EQ(b.getdata(), 5.0);
    EXPECT_DOUBLE_EQ(c.getdata(), 20.0);
    EXPECT_DOUBLE_EQ(a.getGrad(), 4.0);
}

}
