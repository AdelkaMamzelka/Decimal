#ifndef DECIMAL_H
#define DECIMAL_H

#include <math.h>
#include <stdio.h>

typedef struct  //для децимал
{
  unsigned int bits[4];
} decimal;

int get_bit(unsigned int position, decimal digit);
int set_bit(int don, unsigned int position, decimal* digit);
void printforbin(decimal dig);
int reboot(decimal* result);
int add(decimal value_1, decimal value_2, decimal* result);
int sub(decimal value_1, decimal value_2, decimal* result);
unsigned int get_scale(decimal value);
void set_scale(decimal* value, unsigned int scale);
int from_int_to_decimal(int src, decimal* dst);
int negate(decimal value, decimal* result);
int right(decimal digit);
void cpy(decimal* mry, decimal num);
int mul(decimal value_1, decimal value_2, decimal* result);
int is_less(decimal value_1, decimal value_2);
int is_less_or_equal(decimal value_1, decimal value_2);
int is_greater(decimal value_1, decimal value_2);
int is_greater_or_equal(decimal value_1, decimal value_2);
int is_equal(decimal value_1, decimal value_2);
int is_not_equal(decimal value_1, decimal value_2);
int round(decimal value, decimal* result);
int bank_round(decimal* div, decimal ostatok);
int bigdiv_by_10(decimal* num);
int floor(decimal value, decimal* result);
int truncate(decimal value, decimal* result);
int from_decimal_to_int(decimal src, int* dst);
int from_decimal_to_float(decimal src, float* dst);
int from_float_to_decimal(float src, decimal* dst);
int div(decimal value_1, decimal value_2, decimal* result);
// bigdec
typedef struct {
  unsigned int bits[7];
} big_decimal;

int sravni_mant(big_decimal digit1, big_decimal digit2);
int sub_mant(big_decimal value_1, big_decimal value_2,
                 big_decimal* result);
int add_mant(big_decimal value_1, big_decimal value_2,
                 big_decimal* result);
void decimal_to_big_decimal(decimal small, big_decimal* big);
int big_decimal_to_decimal(big_decimal big, decimal* small);
int get_big(unsigned int position, big_decimal digit);
int set_big(int don, unsigned int position, big_decimal* digit);
void set_big_scale(big_decimal* dec, int scale);
int get_big_scale(big_decimal dec);
int is_zero(big_decimal num);
int check_overflow(big_decimal num);
int normalize_bigdecimal(big_decimal* num);
int div_by_10(big_decimal* num);
int norm_two_big_decimal(big_decimal* num_1, big_decimal* num_2);
int mul_by_10(big_decimal* num);
int mul_big(big_decimal big_value_1, big_decimal big_value_2,
                big_decimal* big_result);
int bigbank_round(big_decimal* div, big_decimal ostatok);
void big_cpy(big_decimal* mry, big_decimal num);
int big_decimal_shift_left(big_decimal* value);
#endif
