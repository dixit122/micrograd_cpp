#include "Value.h"
#include <gtest/gtest.h>

namespace {

// Test for Addition Operation
TEST(ValueTest, Addition) {
    Value a(2.0, "a"); // Create Value object with value 2.0
    Value b(3.0, "b"); // Create Value object with value 3.0
    Value c = a + b;   // Perform addition
    c.backward();      // Compute gradients

    EXPECT_DOUBLE_EQ(c.getdata(), 5.0); // Check forward pass: 2 + 3 = 5
    EXPECT_DOUBLE_EQ(a.getGrad(), 1.0); // Gradient of a: dc/da = 1
    EXPECT_DOUBLE_EQ(b.getGrad(), 1.0); // Gradient of b: dc/db = 1
    c.resetAllGrads();

    Value d = a + 2.0;
    d.backward();
    EXPECT_DOUBLE_EQ(d.getdata(), 4.0);  // Check forward pass: 2 + 2 = 4
    EXPECT_DOUBLE_EQ(a.getGrad(), 1.0);  // Gradient of a: dc/da = 1
    d.resetAllGrads();

    Value e = 2.0 + a;
    e.backward();
    EXPECT_DOUBLE_EQ(e.getdata(), 4.0);  // Check forward pass: 2 + 2 = 4
    EXPECT_DOUBLE_EQ(a.getGrad(), 1.0);  // Gradient of a: dc/da = 1
}

// Test for Multiplication Operation
TEST(ValueTest, Multiplication) {
    Value a(2.0, "a"); // Create Value object with value 2.0
    Value b(3.0, "b"); // Create Value object with value 3.0
    Value c = a * b;   // Perform multiplication
    c.backward();      // Compute gradients

    EXPECT_DOUBLE_EQ(c.getdata(), 6.0);  // Check forward pass: 2 * 3 = 6
    EXPECT_DOUBLE_EQ(a.getGrad(), 3.0);  // Gradient of a: dc/da = b = 3
    EXPECT_DOUBLE_EQ(b.getGrad(), 2.0);  // Gradient of b: dc/db = a = 2
    c.resetAllGrads();

    Value d = a * 3.0;
    d.backward();
    EXPECT_DOUBLE_EQ(d.getdata(), 6.0);  // Check forward pass: 2 * 3 = 6
    EXPECT_DOUBLE_EQ(a.getGrad(), 3.0);  // Gradient of a: dc/da = 3
    d.resetAllGrads();

    Value e = 4.0 * a;
    e.backward();
    EXPECT_DOUBLE_EQ(e.getdata(), 8.0);  // Check forward pass: 4 * 2 = 8
    EXPECT_DOUBLE_EQ(a.getGrad(), 4.0);  // Gradient of a: dc/da = 4
}

// Test for Subtraction Operation
TEST(ValueTest, Subtraction) {
    Value a(5.0, "a"); // Create Value object with value 5.0
    Value b(3.0, "b"); // Create Value object with value 3.0
    Value c = a - b;   // Perform subtraction
    c.backward();      // Compute gradients

    EXPECT_DOUBLE_EQ(c.getdata(), 2.0);  // Check forward pass: 5 - 3 = 2
    EXPECT_DOUBLE_EQ(a.getGrad(), 1.0);  // Gradient of a: dc/da = 1
    EXPECT_DOUBLE_EQ(b.getGrad(), -1.0); // Gradient of b: dc/db = -1
    c.resetAllGrads();

    Value d = a - 2.0;
    d.backward();
    EXPECT_DOUBLE_EQ(d.getdata(), 3.0);  // Check forward pass: 5 - 2 = 3
    EXPECT_DOUBLE_EQ(a.getGrad(), 1.0);  // Gradient of a: dc/da = 1
    d.resetAllGrads();

    Value e = 2.0 - a;
    e.backward();
    EXPECT_DOUBLE_EQ(e.getdata(), -3.0);  // Check forward pass: 2 - 5 = -3
    EXPECT_DOUBLE_EQ(a.getGrad(), -1.0);  // Gradient of a: dc/da = -1
}

// Test for Mixed Operations (e.g., multiplication followed by addition)
TEST(ValueTest, MixedOperations) {
    Value a(2.0, "a"); // Create Value object with value 2.0
    Value b(3.0, "b"); // Create Value object with value 3.0
    Value c(4.0, "c"); // Create Value object with value 4.0
    Value d = a * b + c; // Compute d = a * b + c
    d.backward();        // Compute gradients

    EXPECT_DOUBLE_EQ(d.getdata(), 10.0); // Check forward pass: 2 * 3 + 4 = 10
    EXPECT_DOUBLE_EQ(a.getGrad(), 3.0);  // Gradient of a: dd/da = b = 3
    EXPECT_DOUBLE_EQ(b.getGrad(), 2.0);  // Gradient of b: dd/db = a = 2
    EXPECT_DOUBLE_EQ(c.getGrad(), 1.0);  // Gradient of c: dd/dc = 1
}

// Test for Operations with Constants
TEST(ValueTest, OperationsWithConstants) {
    Value a(2.0, "a");  // Create Value object with value 2.0
    Value b = a + 3.0;  // Add a constant (3.0) to a Value object
    Value c = 4.0 * b;  // Multiply a Value object by a constant (4.0)
    c.backward();       // Compute gradients

    EXPECT_DOUBLE_EQ(b.getdata(), 5.0);  // Check b: 2 + 3 = 5
    EXPECT_DOUBLE_EQ(c.getdata(), 20.0); // Check c: 4 * 5 = 20
    EXPECT_DOUBLE_EQ(a.getGrad(), 4.0);  // Gradient of a: dc/da = dc/db * db/da = 4 * 1 = 4
}

} // namespace
