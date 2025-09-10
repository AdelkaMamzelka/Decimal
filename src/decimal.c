#include "decimal.h"

// arifmetics

int sub(decimal value_1, decimal value_2, decimal *result) {
  if (result == NULL || !right(value_1) || !right(value_2)) return 4;
  reboot(result);
  big_decimal big_1 = {0};
  big_decimal big_2 = {0};
  big_decimal big_result = {0};
  decimal_to_big_decimal(value_1, &big_1);
  decimal_to_big_decimal(value_2, &big_2);
  decimal_to_big_decimal(*result, &big_result);
  int return_cod = 0;
  int sign_1 = get_bit(127, value_1);
  int sign_2 = get_bit(127, value_2);
  norm_two_big_decimal(&big_1, &big_2);
  int resultat = sravni_mant(big_1, big_2);
  if (sign_1 == sign_2) {
    if (resultat < 0) {
      big_decimal bufer = {0};
      big_cpy(&bufer, big_2);
      big_cpy(&big_2, big_1);
      big_cpy(&big_1, bufer);
      sign_1 = 1 - sign_1;
    }
    sub_mant(big_1, big_2, &big_result);
    int scale = get_big_scale(big_2);
    set_big_scale(&big_result, scale);
    return_cod = normalize_bigdecimal(&big_result);
    big_decimal_to_decimal(big_result, result);
  } else if (sign_1 == 0 && sign_2 == 1) {
    set_bit(0, 127, &value_2);
    return_cod = add(value_1, value_2, result);
  } else if (sign_1 == 1 && sign_2 == 0) {
    set_bit(0, 127, &value_1);
    return_cod = add(value_1, value_2, result) == 0 ? 0 : 2;
  }

  set_bit(sign_1, 127, result);
  return return_cod;
}

int add(decimal value_1, decimal value_2, decimal *result) {
  if (result == NULL || !right(value_1) || !right(value_2)) return 4;
  reboot(result);
  int err = 0;
  big_decimal big_1 = {0};
  big_decimal big_2 = {0};
  big_decimal big_result = {0};
  decimal_to_big_decimal(value_1, &big_1);
  decimal_to_big_decimal(value_2, &big_2);
  decimal_to_big_decimal(*result, &big_result);
  int sign_1 = get_bit(127, value_1);
  int sign_2 = get_bit(127, value_2);

  norm_two_big_decimal(&big_1, &big_2);
  if (sign_1 == sign_2) {
    add_mant(big_1, big_2, &big_result);

    int scale = get_big_scale(big_2);
    set_big_scale(&big_result, scale);
    if (normalize_bigdecimal(&big_result)) {
      err = sign_1 == 1 ? 2 : 1;
    }
    big_decimal_to_decimal(big_result, result);
    set_bit(sign_1, 127, result);
  } else {
    decimal value_11 = {0};
    decimal value_22 = {0};
    cpy(&value_11, value_1);
    cpy(&value_22, value_2);
    set_bit(0, 127, &value_11);
    set_bit(0, 127, &value_22);
    if (sign_1 == 0) {
      sub(value_11, value_22, result);
    } else {
      sub(value_22, value_11, result);
    }
  }
  return err;
}

int mul(decimal value_1, decimal value_2, decimal *result) {
  if (result == NULL || !right(value_1) || !right(value_2)) {
    return 4;
  }
  reboot(result);
  big_decimal big_value_1 = {0};
  big_decimal big_value_2 = {0};
  big_decimal big_result = {0};
  int sign_1 = get_bit(127, value_1);
  int sign_2 = get_bit(127, value_2);
  reboot(result);
  decimal_to_big_decimal(value_1, &big_value_1);
  decimal_to_big_decimal(value_2, &big_value_2);
  int scale1 = get_big_scale(big_value_1);
  int scale2 = get_big_scale(big_value_2);

  int flag = 0;
  mul_big(big_value_1, big_value_2, &big_result);
  int result_sign = sign_1 ^ sign_2;
  int scale = scale1 + scale2;
  set_big_scale(&big_result, scale);
  set_big(result_sign, 223, &big_result);
  flag = normalize_bigdecimal(&big_result);
  big_decimal_to_decimal(big_result, result);
  return flag;
}

// comparison
int is_less(decimal value_1, decimal value_2) {  //<

  if ((!right(value_1)) || (!right(value_2))) return 4;
  int err_flag = 0;
  big_decimal big_1 = {0};
  big_decimal big_2 = {0};
  decimal_to_big_decimal(value_1, &big_1);
  decimal_to_big_decimal(value_2, &big_2);
  norm_two_big_decimal(&big_1, &big_2);
  int sign_1 = get_big(223, big_1);
  int sign_2 = get_big(223, big_2);
  if (sign_1 == sign_2) {
    if (sign_1 ^ (sravni_mant(big_1, big_2) == -1)) {
      err_flag = 1;
    }
  } else if (sign_1) {
    err_flag = 1;
  }
  return err_flag;
}

int is_less_or_equal(decimal value_1, decimal value_2) {  //<=
  if (!right(value_1) || !right(value_2)) return 4;
  int err_flag = 0;
  big_decimal big_1 = {0};
  big_decimal big_2 = {0};
  decimal_to_big_decimal(value_1, &big_1);
  decimal_to_big_decimal(value_2, &big_2);
  norm_two_big_decimal(&big_1, &big_2);
  int sign_1 = get_big(223, big_1);
  int sign_2 = get_big(223, big_2);
  if (sign_1 == sign_2) {
    int a = sravni_mant(big_1, big_2);
    if ((sign_1 && a > -1) || (!sign_1 && a < 1)) {
      err_flag = 1;
    }
  } else if (sign_1) {
    err_flag = 1;
  }
  if (is_zero(big_1) && is_zero(big_2)) {
    err_flag = 1;
  }
  return err_flag;
}

int is_greater(decimal value_1, decimal value_2) {  //>
  if (!right(value_1) || !right(value_2)) return 4;
  int err_flag = 0;
  big_decimal big_1 = {0};
  big_decimal big_2 = {0};
  decimal_to_big_decimal(value_1, &big_1);
  decimal_to_big_decimal(value_2, &big_2);
  norm_two_big_decimal(&big_1, &big_2);
  int sign_1 = get_big(223, big_1);
  int sign_2 = get_big(223, big_2);
  if (sign_1 == sign_2) {
    int a = sravni_mant(big_1, big_2);
    if ((sign_1 && a == -1) || (!sign_1 && a == 1)) {
      err_flag = 1;
    }
  } else if (sign_2) {
    err_flag = 1;
  }
  if (is_zero(big_1) && is_zero(big_2)) {
    err_flag = 0;
  }
  return err_flag;
}

int is_greater_or_equal(decimal value_1, decimal value_2) {  //>=
  int err_flag = 0;
  if (!right(value_1) || !right(value_2)) return 4;
  big_decimal big_1 = {0};
  big_decimal big_2 = {0};
  decimal_to_big_decimal(value_1, &big_1);
  decimal_to_big_decimal(value_2, &big_2);
  norm_two_big_decimal(&big_1, &big_2);
  int sign_1 = get_big(223, big_1);
  int sign_2 = get_big(223, big_2);
  if (sign_1 == sign_2) {
    int a = sravni_mant(big_1, big_2);
    if ((sign_1 && a < 1) || (!sign_1 && a > -1)) {
      err_flag = 1;
    }
  } else if (sign_2) {
    err_flag = 1;
  }
  if (is_zero(big_1) && is_zero(big_2)) {
    err_flag = 1;
  }
  return err_flag;
}

int is_equal(decimal value_1, decimal value_2) {  //==
  int err_flag = 0;
  if (!_right(value_1) || !_right(value_2)) return 4;
  big_decimal big_1 = {0};
  big_decimal big_2 = {0};
  decimal_to_big_decimal(value_1, &big_1);
  decimal_to_big_decimal(value_2, &big_2);
  norm_two_big_decimal(&big_1, &big_2);
  if (_is_zero(big_1)) big_1.bits[6] = 0;
  if (is_zero(big_2)) big_2.bits[6] = 0;
  int sign_1 = get_big(223, big_1);
  int sign_2 = get_big(223, big_2);

  if ((sravni_mant(big_1, big_2) == 0) && ((sign_1 == sign_2))) {
    err_flag = 1;
  }

  return err_flag;
}

int is_not_equal(decimal value_1, decimal value_2) {  //!=
  int err_flag = 0;
  if (!right(value_1) || !right(value_2)) return 4;
  big_decimal big_1 = {0};
  big_decimal big_2 = {0};
  decimal_to_big_decimal(value_1, &big_1);
  decimal_to_big_decimal(value_2, &big_2);
  norm_two_big_decimal(&big_1, &big_2);
  int sign_1 = get_big(223, big_1);
  int sign_2 = get_big(223, big_2);
  if (((sravni_mant(big_1, big_2) != 0) || (sign_1 != sign_2)) &&
      (!is_zero(big_1) || !is_zero(big_2))) {
    err_flag = 1;
  }

  return err_flag;
}
// transformation

int from_decimal_to_float(decimal src, float *dst) {
  if (dst == NULL) return 1;
  int err_result = 1;
  int flag_is_right = right(src);

  if (flag_is_right) {
    int scale = get_scale(src);
    decimal r = {0};
    int sign = get_bit(127, src);
    int zero = !is_equal(src, r);
    if (zero == 0) {
      if (sign == 1)
        *dst = -0.0f;
      else
        *dst = 0.0f;
      err_result = 0;
    } else if (0 <= scale && scale < 29) {
      double t_res = 0.0f;
      for (int i = 95; i >= 0; --i) {
        int num = get_bit(i, src);
        t_res = t_res * 2 + num;
      }
      for (; scale > 0; --scale) {
        t_res /= 10;
      }
      if (sign == 1) t_res = -t_res;

      *dst = (float)t_res;
      err_result = 0;
    }
  }
  return err_result;
}

int from_int_to_decimal(int src, decimal *dst) {
  if (dst == NULL) return 1;
  reboot(dst);
  unsigned int c = src;
  if (src < 0) {
    c = -(long)src;
    set_bit(1, 127, dst);
  }
  dst->bits[0] = c;
  return 0;
}

int from_decimal_to_int(decimal src, int *dst) {
  if (dst == NULL || !right(src)) return 1;
  int error = 0;
  decimal temp = {0};
  truncate(src, &temp);
  if (temp.bits[1] != 0 || temp.bits[2] != 0 ||
      (temp.bits[3] & 0x7FFFFFFF || (temp.bits[0] > 0x80000000)) != 0) {
    error = 1;
  } else {
    *dst = temp.bits[0];
    if (get_bit(127, temp)) {
      *dst *= -1;
    }
  }
  return error;
}

// other

int negate(decimal value, decimal *result) {
  if (result == NULL || !right(value)) {
    return 1;
  }
  reboot(result);
  *result = value;
  int znak = get_bit(127, value);
  if (znak == 0) {
    set_bit(1, 127, result);
  } else {
    set_bit(0, 127, result);
  }
  return 0;
}

int floor(decimal value, decimal *result) {
  if (result == NULL || !right(value)) return 1;
  reboot(result);
  int sign = get_bit(127, value);
  int scale = get_scale(value);
  int count = scale;
  if (scale == 0) cpy(result, value);
  for (int i = 0; i < count; i++) {
    set_scale(&value, 0);
    int ostatok = bigdiv_by_10(&value);
    if (sign == 1 && ostatok != 0) {
      decimal dec1 = {{1, 0, 0, 0x80000000}};
      add(value, dec1, &value);
    }
    set_scale(&value, --scale);
  }
  cpy(result, value);
  set_bit(sign, 127, result);
  return 0;
}

int round(decimal value, decimal *result) {
  if (result == NULL || !right(value)) return 1;
  reboot(result);

  int sign = get_bit(127, value);
  int scale = get_scale(value);
  int count = scale;

  if (scale == 0) cpy(result, value);
  for (int i = 0; i < count; i++) {
    int ostatok = bigdiv_by_10(&value);
    decimal ostat = {{ostatok, 0, 0, 0}};
    bank_round(&value, ostat);
    set_scale(&value, --scale);
    cpy(result, value);
  }
  set_bit(sign, 127, result);
  return 0;
}

int truncate(decimal value, decimal *result) {
  if (result == NULL || !right(value)) return 1;
  reboot(result);
  big_decimal big_1 = {0};
  decimal_to_big_decimal(value, &big_1);

  int scale = get_big_scale(big_1);
  while (scale > 0) {
    div_by_10(&big_1);
    scale--;
  }
  set_big_scale(&big_1, 0);
  big_decimal_to_decimal(big_1, &value);
  *result = value;
  return 0;
}
