#include "decimal.h"

// set get
void cpy(decimal* mry, decimal num) {
  for (int i = 0; i < 4; i++) {
    mry->bits[i] = num.bits[i];
  }
}

int get_bit(unsigned int position, decimal digit) {
  int klast = position / 32;
  int bit = position % 32;
  int a = ((digit.bits[klast] >> bit) & 00000001);
  return a;
}

int set_bit(int don, unsigned int position, decimal* digit) {
  int klast = position / 32;
  int bit = position % 32;

  if (don) {
    digit->bits[klast] = ((don << bit) | digit->bits[klast]);
  } else {
    digit->bits[klast] = digit->bits[klast] & (~(1 << bit));
  }
  return 0;  // digit;
}

unsigned int get_scale(decimal value) {
  unsigned int scale = 0;
  for (int i = 119; i >= 112; i--) {  // 0...28
    int bit = get_bit(i, value);
    scale = scale * 2 + bit;
  }
  return scale;
}

void set_scale(decimal* value, unsigned int scale) {
  for (int i = 112; i <= 119; i++) {
    set_bit(scale % 2, i, value);
    scale = scale / 2;
  }
}

// print

void printforbin(decimal dig) {
  for (int i = (4 * 32); i >= 0; i--) {
    printf("%d", get_bit(i, dig));
    if (i % 8 == 0) {
      printf(" ");
    }
    if (i % 32 == 0) {
      printf("   ");
    }
  }
  printf("\n");
}

// filtration

int reboot(decimal* result) {
  for (int i = 0; i < 4; i++) {
    result->bits[0] = result->bits[1] = result->bits[2] = result->bits[3] = 0;
  }
  return 0;
}

int right(decimal digit) {
  int result = 1;

  for (int i = 96; i < 112 && result; i++) {
    if (get_bit(i, digit) == 1) result = 0;
  }
  for (int i = 120; i < 127 && result; i++) {
    if (get_bit(i, digit) == 1) result = 0;
  }
  int scale = get_scale(digit);
  if (scale < 0 || scale > 28) result = 0;
  return result;
}

// arifmetics

int add_mant(big_decimal value_1, big_decimal value_2,
                 big_decimal* result) {
  int over = 0;

  for (int i = 0; i < 192; i++) {
    if (get_big(i, value_1) == 0 && get_big(i, value_2) == 0) {
      if (over == 0) {
        set_big(0, i, result);
      }
      if (over == 1) {
        set_big(1, i, result);
        over = 0;
      }
    } else if ((get_big(i, value_1) == 1 && get_big(i, value_2) == 1)) {
      if (over == 0) {
        set_big(0, i, result);
      }
      if (over == 1) {
        set_big(1, i, result);
      }
      over = 1;
    }

    else {
      if (over == 0) {
        set_big(1, i, result);
      }
      if (over == 1) {
        set_big(0, i, result);
      }
    }
  }
  return 0;
}

int sub_mant(big_decimal value_1, big_decimal value_2,
                 big_decimal* result) {
  int error = 0;
  int debt = 0;

  for (int i = 0; i < 192; i++) {
    int bit_1 = get_big(i, value_1);
    int bit_2 = get_big(i, value_2);
    if (bit_1 == bit_2) {
      set_big(debt, i, result);
    }

    else {
      if (debt == 0) {
        set_big(1, i, result);
      } else if (debt == 1) {
        set_big(0, i, result);
      }
      debt = bit_2 ? 1 : 0;
    }
  }
  return error;
}

// compare

int sravni_mant(big_decimal digit1, big_decimal digit2) {
  int a = 0;
  int b = 0;
  for (int i = 191; (i >= 0) && (a == b); i--) {
    a = get_big(i, digit1);
    b = get_big(i, digit2);
  }
  return a == b ? 0 : a > b ? 1 : -1;
}

// for bigdecimal

void decimal_to_big_decimal(decimal small, big_decimal* big) {
  big->bits[0] = small.bits[0];
  big->bits[1] = small.bits[1];
  big->bits[2] = small.bits[2];
  big->bits[3] = 0;
  big->bits[4] = 0;
  big->bits[5] = 0;
  big->bits[6] = small.bits[3];
}

int big_decimal_to_decimal(big_decimal big, decimal* small) {
  int error = 0;
  int sign = get_big(223, big);

  small->bits[0] = big.bits[0];
  small->bits[1] = big.bits[1];
  small->bits[2] = big.bits[2];
  small->bits[3] = big.bits[6];

  if (sign && error) {
    error = 2;
  }
  return error;
}

int get_big(unsigned int position, big_decimal digit) {
  int klast = position / 32;
  int bit = position % 32;
  int a = ((digit.bits[klast] >> bit) & 00000001);
  return a;
}

int set_big(int don, unsigned int position, big_decimal* digit) {
  int klast = position / 32;
  int bit = position % 32;

  if (don) {
    digit->bits[klast] = ((don << bit) | digit->bits[klast]);
  } else {
    digit->bits[klast] = digit->bits[klast] & (~(1 << bit));
  }
  return 0;
}

void set_big_scale(big_decimal* dec, int scale) {
  if (scale < 0) {
    set_big(1, 216, dec);
    scale = -scale;
  } else {
    set_big(0, 216, dec);
  }
  for (int i = 208; i < 216; i++) {
    set_big(scale % 2, i, dec);
    scale /= 2;
  }
}

int get_big_scale(big_decimal dec) {
  unsigned int scale = 0;
  for (int i = 215; i >= 208; i--) {
    int val = get_big(i, dec);
    scale = scale * 2 + val;
  }
  if (get_big(216, dec) == 1) {
    scale = -scale;
  }
  return scale;
}

int is_zero(big_decimal num) {
  int zero = 1;
  for (int i = 0; i < 6; i++) {
    if (num.bits[i] != 0) {
      zero = 0;
    }
  }
  return zero;
}

int check_overflow(big_decimal num) {
  int overflow = 0;
  for (int i = 5; i >= 3; i--) {
    if (num.bits[i] != 0) {
      overflow = 1;
    }
  }
  return overflow;
}

int normalize_bigdecimal(big_decimal* num) {
  int overflow_detected = check_overflow(*num);
  int scale_factor = get_big_scale(*num);
  int zero_flag = is_zero(*num);
  int err = 0;

  if (zero_flag) {
    num->bits[6] = 0;
  } else {
    while ((overflow_detected && (scale_factor > 0)) || (scale_factor > 28)) {
      int ostatok = div_by_10(num);
      big_decimal ostat = {{ostatok, 0, 0, 0, 0, 0, 0}};
      bigbank_round(num, ostat);

      set_big_scale(num, --scale_factor);
      overflow_detected = check_overflow(*num);
    }

    while ((!overflow_detected) && (scale_factor < 0)) {
      overflow_detected = mul_by_10(num);
      set_big_scale(num, ++scale_factor);
    }

    overflow_detected = check_overflow(*num);
  }

  if (overflow_detected || (scale_factor < 0) || (28 < scale_factor)) {
    if (get_big(223, *num) == 1) {
      err = 2;
    } else {
      err = 1;
    }
  }
  return err;
}

//дочинить
int norm_two_big_decimal(big_decimal* num_1, big_decimal* num_2) {
  int error_overflow = 0;
  int scale_1 = get_big_scale(*num_1);
  int scale_2 = get_big_scale(*num_2);

  while (scale_1 < scale_2 && error_overflow == 0) {
    error_overflow = mul_by_10(num_1);
    if (error_overflow == 0) {
      set_big_scale(num_1, ++scale_1);
    }
  }
  while ((scale_1 < scale_2) && error_overflow == 1) {
    error_overflow = div_by_10(num_2);
    set_big_scale(num_2, --scale_2);
  }

  while ((scale_2 < scale_1) && error_overflow == 0) {
    error_overflow = mul_by_10(num_2);
    if (error_overflow == 0) set_big_scale(num_2, ++scale_2);
  }
  while ((scale_2 < scale_1) && error_overflow == 1) {
    error_overflow = div_by_10(num_1);
    set_big_scale(num_1, --scale_1);
  }

  if (error_overflow == 0) error_overflow = scale_1 == scale_2 ? 0 : 1;
  return error_overflow;
}

int div_by_10(big_decimal* num) {
  unsigned long temp_value = 0;
  for (int i = 5; i >= 0; i--) {
    temp_value = (temp_value << 32) | num->bits[i];
    num->bits[i] = temp_value / 10;
    temp_value %= 10;
  }

  return (int)temp_value;
}

// round

int bigdiv_by_10(decimal* num) {
  big_decimal big = {0};
  decimal_to_big_decimal(*num, &big);
  int result = div_by_10(&big);
  big_decimal_to_decimal(big, num);
  return result;
}

int bank_round(decimal* div, decimal ostatok) {
  big_decimal temp_div = {{0, 0, 0, 0, 0, 0, 0}};
  big_decimal temp_ostatok = {{0, 0, 0, 0, 0, 0, 0}};
  decimal_to_big_decimal(*div, &temp_div);
  decimal_to_big_decimal(ostatok, &temp_ostatok);
  bigbank_round(&temp_div, temp_ostatok);
  big_decimal_to_decimal(temp_div, div);
  return 0;
}

int bigbank_round(big_decimal* div, big_decimal ostatok) {
  int bit_6 = get_big(0, *div);
  big_decimal bdec5 = {{5, 0, 0, 0, 0, 0, 0}};
  int ostatok_5 = sravni_mant(ostatok, bdec5);
  if (ostatok_5 > 0 || (ostatok_5 == 0 && bit_6 == 1)) {
    big_decimal bdec1 = {{1, 0, 0, 0, 0, 0, 0}};
    add_mant(*div, bdec1, div);
  }
  return 0;
}

int mul_by_10(big_decimal* num) {
  big_decimal constant_ten = {{10, 0, 0, 0, 0, 0, 0}};
  unsigned long carry_over = 0;
  for (int i = 0; i < 6; i++) {
    unsigned long product =
        (unsigned long)num->bits[i] * constant_ten.bits[0] + carry_over;
    num->bits[i] = (unsigned int)(product & 0xFFFFFFFF);
    carry_over = product >> 32;
  }
  return carry_over != 0;
}
void big_cpy(big_decimal* mry, big_decimal num) {
  for (int i = 0; i < 7; i++) {
    mry->bits[i] = num.bits[i];
  }
}

int big_decimal_shift_left(big_decimal* value) {
  int bit_31 = get_big(31, *value);
  int bit_63 = get_big(63, *value);
  int bit_95 = get_big(95, *value);
  int bit_127 = get_big(127, *value);
  int bit_159 = get_big(159, *value);
  int bit_191 = get_big(191, *value);
  value->bits[0] = value->bits[0] << 1;
  value->bits[1] = value->bits[1] << 1;
  value->bits[2] = value->bits[2] << 1;
  value->bits[3] = value->bits[3] << 1;
  value->bits[4] = value->bits[4] << 1;
  value->bits[5] = value->bits[5] << 1;
  if (bit_31) set_big(1, 32, value);
  if (bit_63) set_big(1, 64, value);
  if (bit_95) set_big(1, 96, value);
  if (bit_127) set_big(1, 128, value);
  if (bit_159) set_big(1, 160, value);
  return bit_191;
}

int mul_big(big_decimal big_value_1, big_decimal big_value_2,
                big_decimal* big_result) {
  big_decimal temp_result = {0};

  for (int i = 0; i < 192; ++i) {
    if (get_big(i, big_value_2)) {
      temp_result = big_value_1;
      for (int j = 0; j < i; ++j) {
        big_decimal_shift_left(&temp_result);
      }
      if (add_mant(*big_result, temp_result, big_result)) {
        return 1;
      }
    }
  }
  return 0;
}

int from_float_to_decimal(float src, decimal* dst) {
  (void)src;
  (void)dst;
  return 0;
}
int div(decimal value_1, decimal value_2, decimal* result) {
  (void)value_1;
  (void)value_2;
  (void)result;
  return 0;
}