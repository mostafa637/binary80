#ifndef BINARY80_H
#define BINARY80_H

#include <stdint.h>
#include <stdbool.h>

// ===== Constants =====
#define BINARY32_BIAS 127
#define BINARY64_BIAS 1023
#define BINARY80_BIAS 16383
#define UINT_SIZE (sizeof(uintptr_t) * 8)

// ===== Structures =====

// Binary32 (float - 32-bit)
typedef struct {
    uint32_t sign;
    uint32_t exponent;
    uint32_t significand;
} binary32_t;

// Binary64 (double - 64-bit)
typedef struct {
    uint64_t sign;
    uint64_t exponent;
    uint64_t significand;
} binary64_t;

// Binary80 (extended precision - 80-bit)
typedef struct {
    uint64_t significand;  // 64 bits (explicit leading bit)
    uint16_t exponent;     // 15 bits
    uint16_t sign;         // 1 bit
} binary80_t;

// Helper structures
typedef struct {
    int exponent;
    uint64_t significand;
} normalized_t;

typedef struct {
    int exponent;
    uint64_t s1;
    uint64_t s2;
} aligned_t;

// ===== Utility Functions =====
int Len64(uint64_t x);
int LeadingZeros64(uint64_t x);
void Add64(uint64_t x, uint64_t y, uint64_t carry, uint64_t* sum, uint64_t* carryOut);
void Sub64(uint64_t x, uint64_t y, uint64_t borrow, uint64_t* diff, uint64_t* borrowOut);
void Mul64(uint64_t x, uint64_t y, uint64_t* hi, uint64_t* lo);
void Div64(uint64_t hi, uint64_t lo, uint64_t y, uint64_t* quo, uint64_t* rem);
void Rem64(uint64_t hi, uint64_t lo, uint64_t y, uint64_t* rem);

// ===== Binary32 Functions =====
binary32_t binary32_unpack(uint32_t bits);
uint32_t binary32_pack(binary32_t x);
binary32_t binary32_from_float(float val);
float binary32_to_float(binary32_t x);

// ===== Binary64 Functions =====
binary64_t binary64_unpack(uint64_t bits);
uint64_t binary64_pack(binary64_t x);
binary64_t binary64_from_double(double val);
double binary64_to_double(binary64_t x);

// ===== Binary80 Core Functions =====
binary80_t binary80_unpack(uint8_t bytes[10]);
void binary80_pack(binary80_t x, uint8_t bytes[10]);

// ===== Binary80 Special Values =====
binary80_t binary80_zero(int sign);
binary80_t binary80_SNaN(void);
binary80_t binary80_QNaN(void);
binary80_t binary80_FPIn(void);
binary80_t binary80_Inf(int sign);

// ===== Binary80 Check Functions =====
bool binary80_is_zero(binary80_t x);
bool binary80_is_SNaN(binary80_t x);
bool binary80_is_QNaN(binary80_t x);
bool binary80_is_NaN(binary80_t x);
bool binary80_is_Inf(binary80_t x, int check_sign);

// ===== Binary80 Conversion Functions =====
double binary80_to_double(binary80_t x);
binary80_t binary80_from_double(double val);
binary80_t binary80_from_float(float val);
binary80_t binary80_from_int(int val);

// ===== Binary80 Normalization =====
normalized_t binary80_normalize(int exponent, uint64_t significand, int bits);
aligned_t binary80_align_exponents(binary80_t a, binary80_t b);

// ===== Binary80 Arithmetic Operations =====
binary80_t binary80_neg(binary80_t x);
binary80_t binary80_add(binary80_t a, binary80_t b);
binary80_t binary80_sub(binary80_t a, binary80_t b);
binary80_t binary80_mul(binary80_t a, binary80_t b);
binary80_t binary80_div(binary80_t a, binary80_t b);

// ===== Binary80 Comparison =====
bool binary80_eq(binary80_t a, binary80_t b);
bool binary80_gt(binary80_t a, binary80_t b);
bool binary80_lt(binary80_t a, binary80_t b);

// ===== Binary80 Print Functions =====
void binary80_print(binary80_t x);
void binary80_print_bits(binary80_t x);

#endif // BINARY80_H
