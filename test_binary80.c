#include "binary80.h"
#include <stdio.h>

int main() {
    printf("=== Binary80 Tests (Using Div64) ===\n\n");
    
    binary80_t a = binary80_from_double(3.14159);
    binary80_t b = binary80_from_double(2.71828);

    printf("a = "); binary80_print(a);
    binary80_print_bits(a);
    
    printf("\nb = "); binary80_print(b);
    binary80_print_bits(b);

    printf("\n--- Addition ---\n");
    binary80_t sum = binary80_add(a, b);
    printf("a + b = "); binary80_print(sum);
    binary80_print_bits(sum);

    printf("\n--- Subtraction ---\n");
    binary80_t diff = binary80_sub(a, b);
    printf("a - b = "); binary80_print(diff);
    binary80_print_bits(diff);

    printf("\n--- Multiplication ---\n");
    binary80_t prod = binary80_mul(a, b);
    printf("a * b = "); binary80_print(prod);
    binary80_print_bits(prod);

    printf("\n--- Division (using Div64) ---\n");
    binary80_t quot = binary80_div(a, b);
    printf("a / b = "); binary80_print(quot);
    binary80_print_bits(quot);

    printf("\n--- Special Division Tests ---\n");
    binary80_t x = binary80_from_double(10.0);
    binary80_t y = binary80_from_double(3.0);
    binary80_t z = binary80_div(x, y);
    printf("10.0 / 3.0 = "); binary80_print(z);
    binary80_print_bits(z);

    printf("\n--- Large Division Test ---\n");
    binary80_t big1 = binary80_from_double(1e15);
    binary80_t big2 = binary80_from_double(7.0);
    binary80_t big_result = binary80_div(big1, big2);
    printf("1e15 / 7.0 = "); binary80_print(big_result);
    binary80_print_bits(big_result);

    printf("\n--- Special Values ---\n");
    binary80_t zero = binary80_zero(0);
    printf("Zero: "); binary80_print(zero);
    
    binary80_t inf = binary80_Inf(0);
    printf("Inf: "); binary80_print(inf);
    
    binary80_t qnan = binary80_QNaN();
    printf("QNaN: "); binary80_print(qnan);
    
    printf("\n--- Comparison ---\n");
    printf("a == b: %d\n", binary80_eq(a, b));
    printf("a > b: %d\n", binary80_gt(a, b));
    printf("a < b: %d\n", binary80_lt(a, b));

    return 0;
}
