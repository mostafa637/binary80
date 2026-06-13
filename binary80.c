#include "binary80.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// ===== Lookup Table =====
const uint8_t len8tab[256] = {
    0x00, 0x01, 0x02, 0x02, 0x03, 0x03, 0x03, 0x03, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08
};

// ===== Utility Functions =====
int Len64(uint64_t x) {
    int n = 0;
    if (x >= (1ULL << 32)) {
        x >>= 32;
        n = 32;
    }
    if (x >= (1ULL << 16)) {
        x >>= 16;
        n += 16;
    }
    if (x >= (1ULL << 8)) {
        x >>= 8;
        n += 8;
    }
    return n + len8tab[(uint8_t)x];
}

int LeadingZeros64(uint64_t x) {
    return 64 - Len64(x);
}

void Add64(uint64_t x, uint64_t y, uint64_t carry, uint64_t* sum, uint64_t* carryOut) {
    *sum = x + y + carry;
    *carryOut = ((x & y) | ((x | y) & ~(*sum))) >> 63;
}

void Sub64(uint64_t x, uint64_t y, uint64_t borrow, uint64_t* diff, uint64_t* borrowOut) {
    *diff = x - y - borrow;
    *borrowOut = ((~x & y) | (~(x ^ y) & *diff)) >> 63;
}

void Mul64(uint64_t x, uint64_t y, uint64_t* hi, uint64_t* lo) {
    const uint64_t mask32 = (1ULL << 32) - 1;
    uint64_t x0 = x & mask32;
    uint64_t x1 = x >> 32;
    uint64_t y0 = y & mask32;
    uint64_t y1 = y >> 32;
    uint64_t w0 = x0 * y0;
    uint64_t t = x1 * y0 + (w0 >> 32);
    uint64_t w1 = t & mask32;
    uint64_t w2 = t >> 32;
    w1 += x0 * y1;
    *hi = x1 * y1 + w2 + (w1 >> 32);
    *lo = x * y;
}

void Div64(uint64_t hi, uint64_t lo, uint64_t y, uint64_t* quo, uint64_t* rem) {
    if (y == 0) {
        fprintf(stderr, "Error: division by zero\n");
        exit(1);
    }
    if (y <= hi) {
        fprintf(stderr, "Error: overflow\n");
        exit(1);
    }
    if (hi == 0) {
        *quo = lo / y;
        *rem = lo % y;
        return;
    }

    const uint64_t two32 = 1ULL << 32;
    const uint64_t mask32 = two32 - 1;

    uint32_t s = LeadingZeros64(y);
    y <<= s;

    uint64_t yn1 = y >> 32;
    uint64_t yn0 = y & mask32;
    uint64_t un32 = (hi << s) | (lo >> (64 - s));
    uint64_t un10 = lo << s;
    uint64_t un1 = un10 >> 32;
    uint64_t un0 = un10 & mask32;
    uint64_t q1 = un32 / yn1;
    uint64_t rhat = un32 - q1 * yn1;

    while (q1 >= two32 || q1 * yn0 > two32 * rhat + un1) {
        q1--;
        rhat += yn1;
        if (rhat >= two32) break;
    }

    uint64_t un21 = un32 * two32 + un1 - q1 * y;
    uint64_t q0 = un21 / yn1;
    rhat = un21 - q0 * yn1;

    while (q0 >= two32 || q0 * yn0 > two32 * rhat + un0) {
        q0--;
        rhat += yn1;
        if (rhat >= two32) break;
    }

    *quo = q1 * two32 + q0;
    *rem = (un21 * two32 + un0 - q0 * y) >> s;
}

void Rem64(uint64_t hi, uint64_t lo, uint64_t y, uint64_t* rem) {
    uint64_t quo;
    Div64(hi % y, lo, y, &quo, rem);
}

// ===== Binary32 Functions =====
binary32_t binary32_unpack(uint32_t bits) {
    binary32_t result;
    result.sign = (bits >> 31) & 0x1;
    result.exponent = (bits >> 23) & 0xFF;
    result.significand = bits & 0x7FFFFF;
    return result;
}

uint32_t binary32_pack(binary32_t x) {
    return ((uint32_t)x.sign << 31) | 
           ((uint32_t)x.exponent << 23) | 
           (x.significand & 0x7FFFFF);
}

binary32_t binary32_from_float(float val) {
    uint32_t bits;
    memcpy(&bits, &val, sizeof(float));
    return binary32_unpack(bits);
}

float binary32_to_float(binary32_t x) {
    uint32_t bits = binary32_pack(x);
    float result;
    memcpy(&result, &bits, sizeof(float));
    return result;
}

// ===== Binary64 Functions =====
binary64_t binary64_unpack(uint64_t bits) {
    binary64_t result;
    result.sign = (bits >> 63) & 0x1;
    result.exponent = (bits >> 52) & 0x7FF;
    result.significand = bits & 0xFFFFFFFFFFFFFULL;
    return result;
}

uint64_t binary64_pack(binary64_t x) {
    return ((uint64_t)x.sign << 63) | 
           ((uint64_t)x.exponent << 52) | 
           (x.significand & 0xFFFFFFFFFFFFFULL);
}

binary64_t binary64_from_double(double val) {
    uint64_t bits;
    memcpy(&bits, &val, sizeof(double));
    return binary64_unpack(bits);
}

double binary64_to_double(binary64_t x) {
    uint64_t bits = binary64_pack(x);
    double result;
    memcpy(&result, &bits, sizeof(double));
    return result;
}

// ===== Binary80 Core Functions =====
binary80_t binary80_unpack(uint8_t bytes[10]) {
    binary80_t result;
    result.significand = 0;
    for (int i = 0; i < 8; i++) {
        result.significand |= ((uint64_t)bytes[i]) << (i * 8);
    }
    
    uint16_t exp_sign = bytes[8] | ((uint16_t)bytes[9] << 8);
    result.exponent = exp_sign & 0x7FFF;
    result.sign = (exp_sign >> 15) & 0x1;
    
    return result;
}

void binary80_pack(binary80_t x, uint8_t bytes[10]) {
    for (int i = 0; i < 8; i++) {
        bytes[i] = (x.significand >> (i * 8)) & 0xFF;
    }
    
    uint16_t exp_sign = (x.exponent & 0x7FFF) | ((uint16_t)x.sign << 15);
    bytes[8] = exp_sign & 0xFF;
    bytes[9] = (exp_sign >> 8) & 0xFF;
}

// ===== Binary80 Special Values =====
binary80_t binary80_zero(int sign) {
    binary80_t result;
    result.significand = 0;
    result.exponent = 0;
    result.sign = sign & 0x1;
    return result;
}

binary80_t binary80_SNaN(void) {
    binary80_t result;
    result.significand = (0b10ULL << 62) + 1;
    result.exponent = 0x7FFF;
    result.sign = 0;
    return result;
}

binary80_t binary80_QNaN(void) {
    binary80_t result;
    result.significand = (0b11ULL << 62) + 1;
    result.exponent = 0x7FFF;
    result.sign = 0;
    return result;
}

binary80_t binary80_FPIn(void) {
    binary80_t result;
    result.significand = (0b11ULL << 62);
    result.exponent = 0x7FFF;
    result.sign = 1;
    return result;
}

binary80_t binary80_Inf(int sign) {
    binary80_t result;
    result.significand = (0b10ULL << 62);
    result.exponent = 0x7FFF;
    result.sign = sign & 0x1;
    return result;
}

// ===== Binary80 Check Functions =====
bool binary80_is_zero(binary80_t x) {
    return x.exponent == 0 && x.significand == 0;
}

bool binary80_is_SNaN(binary80_t x) {
    return x.exponent == 0x7FFF &&
           (x.significand & ((1ULL << 62) - 1)) != 0 &&
           (x.significand >> 62) == 0b10;
}

bool binary80_is_QNaN(binary80_t x) {
    return x.exponent == 0x7FFF &&
           (x.significand >> 62) == 0b11;
}

bool binary80_is_NaN(binary80_t x) {
    return binary80_is_SNaN(x) || binary80_is_QNaN(x);
}

bool binary80_is_Inf(binary80_t x, int check_sign) {
    bool is_inf = x.exponent == 0x7FFF &&
                  (x.significand >> 62) == 0b10 &&
                  (x.significand & ((1ULL << 62) - 1)) == 0;
    
    if (check_sign < 0) return is_inf;
    return is_inf && x.sign == check_sign;
}

// ===== Binary80 Conversion Functions =====
double binary80_to_double(binary80_t x) {
    if (x.exponent == 0 && x.significand == 0) {
        return x.sign ? -0.0 : 0.0;
    }

    if (x.exponent == 0x7FFF) {
        uint64_t int_bit = x.significand >> 63;
        uint64_t frac = x.significand & 0x7FFFFFFFFFFFFFFFULL;

        if (int_bit == 1 && frac == 0) {
            return x.sign ? -INFINITY : INFINITY;
        }
        return NAN;
    }

    if (x.exponent == 0) {
        return x.sign ? -0.0 : 0.0;
    }

    int true_exponent = (int)x.exponent - BINARY80_BIAS;
    uint64_t significand_52 = (x.significand >> 11) & 0xFFFFFFFFFFFFFULL;

    int exp64 = true_exponent + BINARY64_BIAS;
    if (exp64 <= 0) return x.sign ? -0.0 : 0.0;
    if (exp64 >= 0x7FF) return x.sign ? -INFINITY : INFINITY;

    binary64_t d;
    d.sign = x.sign;
    d.exponent = (uint64_t)exp64;
    d.significand = significand_52;

    return binary64_to_double(d);
}

binary80_t binary80_from_double(double val) {
    binary64_t d = binary64_from_double(val);
    
    binary80_t result;
    result.sign = d.sign;
    result.exponent = d.exponent - BINARY64_BIAS + BINARY80_BIAS;
    result.significand = (d.significand << (63 - 52)) | (1ULL << 63);
    
    return result;
}

binary80_t binary80_from_float(float val) {
    binary32_t f = binary32_from_float(val);
    
    binary80_t result;
    result.sign = f.sign;
    result.exponent = f.exponent - BINARY32_BIAS + BINARY80_BIAS;
    result.significand = (((uint64_t)f.significand) << (63 - 23)) | (1ULL << 63);
    
    return result;
}

binary80_t binary80_from_int(int val) {
    return binary80_from_double((double)val);
}

// ===== Binary80 Normalization =====
normalized_t binary80_normalize(int exponent, uint64_t significand, int bits) {
    normalized_t result;

    if ((significand >> bits) > 1) {
        significand >>= 1;
        exponent += 1;
    }

    while (significand != 0 && (significand >> bits) == 0) {
        significand <<= 1;
        exponent -= 1;
    }

    result.exponent = exponent;
    result.significand = significand;
    return result;
}

aligned_t binary80_align_exponents(binary80_t a, binary80_t b) {
    aligned_t r;
    uint64_t s1 = a.significand, s2 = b.significand;
    int e1 = a.exponent, e2 = b.exponent;
    int d = e1 - e2;

    if (d >= 0) {
        s2 = (d >= 64) ? 0 : (s2 >> d);
        r.exponent = e1;
    } else {
        int sh = -d;
        s1 = (sh >= 64) ? 0 : (s1 >> sh);
        r.exponent = e2;
    }

    r.s1 = s1;
    r.s2 = s2;
    return r;
}

// ===== Binary80 Arithmetic Operations =====
binary80_t binary80_neg(binary80_t x) {
    x.sign = !x.sign;
    return x;
}

binary80_t binary80_add(binary80_t a, binary80_t b) {
    if (a.sign && !b.sign) {
        return binary80_sub(b, binary80_neg(a));
    }
    if (!a.sign && b.sign) {
        return binary80_sub(a, binary80_neg(b));
    }

    int sign = a.sign;
    aligned_t aligned = binary80_align_exponents(a, b);
    
    int exponent = aligned.exponent;
    
    uint64_t S, c;
    Add64(aligned.s1, aligned.s2, 0, &S, &c);

    if (c) {
        S = (S >> 1) | (1ULL << 63);
        exponent += 1;
    } else {
        normalized_t norm = binary80_normalize(exponent, S, 63);
        exponent = norm.exponent;
        S = norm.significand;
    }
    
    binary80_t result;
    result.significand = S;
    result.exponent = exponent;
    result.sign = sign;
    return result;
}

binary80_t binary80_sub(binary80_t a, binary80_t b) {
    if (a.sign) {
        if (b.sign) {
            return binary80_sub(binary80_neg(b), binary80_neg(a));
        }
        return binary80_neg(binary80_add(binary80_neg(a), b));
    } else {
        if (b.sign) {
            return binary80_add(a, binary80_neg(b));
        }
    }

    aligned_t aligned = binary80_align_exponents(a, b);

    if (aligned.s1 == aligned.s2) {
        return binary80_zero(0);
    }

    uint64_t abs_S, borrowOut;
    int sign;

    if (aligned.s1 > aligned.s2) {
        Sub64(aligned.s1, aligned.s2, 0, &abs_S, &borrowOut);
        sign = 0;
    } else {
        Sub64(aligned.s2, aligned.s1, 0, &abs_S, &borrowOut);
        sign = 1;
    }

    normalized_t norm = binary80_normalize(aligned.exponent, abs_S, 63);

    binary80_t result;
    result.significand = norm.significand;
    result.exponent = (uint16_t)norm.exponent;
    result.sign = (uint16_t)sign;
    return result;
}

binary80_t binary80_mul(binary80_t a, binary80_t b) {
    if (binary80_is_zero(a) || binary80_is_zero(b)) {
        return binary80_zero(0);
    }

    int sign = a.sign ^ b.sign;
    int exponent = (int)a.exponent + (int)b.exponent - BINARY80_BIAS;

    uint64_t P_hi, P_lo;
    Mul64(a.significand, b.significand, &P_hi, &P_lo);

    uint64_t S;
    if (P_hi & (1ULL << 63)) {
        S = P_hi;
        exponent += 1;
    } else {
        S = (P_hi << 1) | (P_lo >> 63);
    }

    binary80_t result;
    result.significand = S;
    result.exponent = (uint16_t)exponent;
    result.sign = (uint16_t)sign;
    return result;
}

binary80_t binary80_div(binary80_t a, binary80_t b) {
    if (binary80_is_NaN(a) || binary80_is_NaN(b)) {
        return binary80_SNaN();
    }

    int sign = a.sign ^ b.sign;

    if (binary80_is_Inf(a, -1)) {
        if (binary80_is_Inf(b, -1)) {
            fprintf(stderr, "FPInvalidArithmeticOperand: Inf/Inf\n");
            exit(1);
        }
        return binary80_Inf(sign);
    } else if (binary80_is_zero(a)) {
        if (binary80_is_zero(b)) {
            fprintf(stderr, "FPInvalidArithmeticOperand: 0/0\n");
            exit(1);
        }
        return binary80_zero(sign);
    } else if (binary80_is_zero(b)) {
        fprintf(stderr, "FPInvalidArithmeticOperand: division by zero\n");
        exit(1);
    }

    uint64_t s1 = a.significand;
    uint64_t s2 = b.significand;
    int e1 = a.exponent;
    int e2 = b.exponent;

    int exponent = e1 - e2 + BINARY80_BIAS;

    uint64_t dividend_hi = s1 >> 1;
    uint64_t dividend_lo = s1 << 63;
    
    uint64_t S, rem;
    
    if (dividend_hi >= s2) {
        dividend_lo = (dividend_hi << 63) | (dividend_lo >> 1);
        dividend_hi = dividend_hi >> 1;
        exponent += 1;
    }
    
    Div64(dividend_hi, dividend_lo, s2, &S, &rem);

    normalized_t norm = binary80_normalize(exponent, S, 63);

    binary80_t result;
    result.significand = norm.significand;
    result.exponent = norm.exponent;
    result.sign = sign;
    return result;
}

// ===== Binary80 Comparison =====
bool binary80_eq(binary80_t a, binary80_t b) {
    if (binary80_is_NaN(a) || binary80_is_NaN(b)) {
        return false;
    }

    if (binary80_is_zero(a) && binary80_is_zero(b)) {
        return true;
    }

    return a.sign == b.sign &&
           a.exponent == b.exponent &&
           a.significand == b.significand;
}

bool binary80_gt(binary80_t a, binary80_t b) {
    if (binary80_is_NaN(a) || binary80_is_NaN(b)) {
        return false;
    }
    if (binary80_is_Inf(a, 0)) {
        return true;
    }
    if (binary80_is_Inf(a, 1)) {
        return false;
    }

    if (!a.sign) {
        if (b.sign) return true;
        if (a.exponent > b.exponent) return true;
        if (a.exponent < b.exponent) return false;
        return a.significand > b.significand;
    }

    if (!b.sign) return false;
    if (a.exponent < b.exponent) return true;
    if (a.exponent > b.exponent) return false;
    return a.significand < b.significand;
}

bool binary80_lt(binary80_t a, binary80_t b) {
    if (binary80_is_NaN(a) || binary80_is_NaN(b)) {
        return false;
    }
    if (binary80_is_Inf(a, 0)) {
        return false;
    }
    if (binary80_is_Inf(a, 1)) {
        return true;
    }

    if (!a.sign) {
        if (b.sign) return false;
        if (a.exponent < b.exponent) return true;
        if (a.exponent > b.exponent) return false;
        return a.significand < b.significand;
    }

    if (!b.sign) return true;
    if (a.exponent < b.exponent) return false;
    if (a.exponent > b.exponent) return true;
    return a.significand > b.significand;
}

// ===== Binary80 Print Functions =====
void binary80_print(binary80_t x) {
    printf("%+.016f\n", binary80_to_double(x));
}

void binary80_print_bits(binary80_t x) {
    printf("sign=%u, exponent=0x%04x (%d), significand=0x%016llx\n", 
           x.sign, x.exponent, x.exponent, (unsigned long long)x.significand);
}
