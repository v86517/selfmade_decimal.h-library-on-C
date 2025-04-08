#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int error = 0;
  dec_to_zero(dst);
  if (dst == NULL || src == S21INF || src == -S21INF || src == S21NAN ||
      src == -S21NAN) {
    error = 1;
  } else {
    if (src < 0) {
      src = -src;
      setbit(dst, 127);
    }
    dst->bits[0] = src;  // записываем int src в bits[0]
  }
  return error;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int error = 0;

  if (dst == NULL || src.bits[2] != 0 || src.bits[1] != 0) {
    error = 1;
  }

  int scale = getscale(src);
  int sign = getsign(src);
  s21_decimal ten = {{10, 0, 0, 0}};

  while (scale > 0) {
    s21_decimal trash = {0};
    div_bit_process(src, ten, &src, &trash);
    scale--;
  }

  unsetbit(&src, 31);  // обнуляем знаковый бит будущего int
  *dst = src.bits[0];  // записвыаем полученный bits[0] в dst int
  if (sign == 1) {
    *dst = -(*dst);
  }
  return error;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int error = 0;
  dec_to_zero(dst);

  if ((src < 1e-28 && src > 0) || (src > -1e-28 && src < 0) || dst == NULL ||
      src == S21INF || src == -S21INF || src == S21NAN || src == -S21NAN) {
    return 1;
  }
  s21_decimal digit;
  s21_decimal ten = {{10, 0, 0, 0}};
  char strFloat[100];
  // записываем float в строку strFloat. точность 28+6
  get_ready_float_str(snprintf(strFloat, sizeof(strFloat), "%.34f", src),
                      strFloat);
  // считаем количество цифр после запятой для setscale
  int cntDigitsAfterPoint = count_digits_after_point(strFloat);

  for (int i = 0; i < s21_strlen(strFloat); i++) {
    if (strFloat[i] != '.') {
      // каждую цифру из строки переводим в децимал и прибавляем к dst,
      s21_from_int_to_decimal(strFloat[i] - '0', &digit);
      add_bit_process(*dst, digit, dst);
      if (strFloat[i + 1] != '\0' &&
          !(strFloat[i + 1] == '.' && strFloat[i + 2] == '\0')) {
        // если следующий символ строки не точка и за ним не конец строки, то
        // умножаем на 10
        error = s21_mul(*dst, ten, dst);
      }
    }
  }
  setsign(dst, src < 0 ? 1 : 0);
  setscale(dst, cntDigitsAfterPoint);

  return error;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  long double intPart = 0;
  long double floatPart = 0;
  int scale;
  s21_decimal decIntPart = {0};
  s21_decimal decFloatPart = {0};

  // выделяем целую часть decimal src
  s21_truncate(src, &decIntPart);
  // преобразуем эту часть в double
  for (int i = 0; i < 96; i++) {
    if (getbit(decIntPart, i) == 1) {
      intPart += pow2(i);
    }
  }
  // выделяем дробную часть decimal src
  s21_sub(src, decIntPart, &decFloatPart);
  // преобразуем эту часть в double
  for (int i = 0; i < 96; i++) {
    if (getbit(decFloatPart, i) == 1) {
      floatPart += pow2(i);
    }
  }
  scale = getscale(src);
  // доводим запятую в дробной части до нужной позиции
  while (scale > 0) {
    *dst = floatPart /= 10;
    scale--;
  }
  // складываем полученные целую и дробную части
  *dst = *dst + intPart;

  if (getsign(src) == 1) {
    *dst = -(*dst);
  }
  return 0;
}

void get_ready_float_str(int lenth, char *strFloat) {
  int cnt = 0, midZeroFlag = 0, dotFlag = 0, roundFlag = 0;

  // если есть минус, убираем его из строки
  if (strFloat[0] == '-') {
    for (int i = 0; i < lenth; i++) {
      strFloat[i] = strFloat[i + 1];
    }
    lenth--;
  }
  // выделяем 7 значащих цифр в строке оставляя точку и округляя число если цифр
  // больше семи
  for (int i = 0; i < lenth && cnt <= 7; i++) {
    if (strFloat[i] >= '1' && strFloat[i] <= '9') {
      cnt++;
      midZeroFlag = 1;
    }
    if (strFloat[i] == '0' && midZeroFlag == 1) {
      cnt++;
    }
    if (strFloat[i] == '.') {
      dotFlag = i;
    }
    if (cnt == 8 && dotFlag == 0) {
      int j = i;
      while (strFloat[i] != '.') {
        i++;
        dotFlag = i;
      }
      strFloat[j + 1] = '\0';
      round_float_str(strFloat);
      roundFlag = 1;
      while (j != i) {
        strFloat[j] = '0';
        j++;
      }
      strFloat[j] = '.';
      strFloat[j + 1] = '\0';
    }
    if (dotFlag != 0) {
      if (cnt == 8) {
        strFloat[i + 1] = '\0';
      }
    }
  }
  if (cnt == 8 && roundFlag == 0) {
    round_float_str(strFloat);
  }
}

void round_float_str(char *str) {
  // если последний символ 5 и более
  if (str[s21_strlen(str) - 1] > '4') {
    // берем предпоследний символ
    for (int i = s21_strlen(str) - 2; i >= 0; i--) {
      // если точка, идем дальше влево
      if (str[i] == '.') {
        i--;
      }
      // если не девять, то + 1 и выход
      if (str[i] != '9') {
        str[i] = str[i] + 1;
        break;
        // если девять и это не первый символ, то ставим его в ноль
      } else if (i != 0) {
        str[i] = '0';
        // если девять и это первый символ, то сдвигаем всю строку вправо на 1
        // символ, первым символом ставим 1, вторым ставим 0, и выход
      } else if (str[i] != '0') {
        for (i = s21_strlen(str); i > 0; i--) {
          str[i] = str[i - 1];
        }
        str[1] = '0';
        str[0] = '1';
        break;
      }
    }
  }
  str[s21_strlen(str) - 1] = '\0';
}

// возвращает количество цифр в дробной части
int count_digits_after_point(char *strFloat) {
  int cntDigitsAfterPoint = 0, zeroFlag = 0;

  for (int i = (int)s21_strlen(strFloat) - 1; i >= 0; i--) {
    if (strFloat[i] == '0' && zeroFlag == 0) {
      strFloat[i] = '\0';
      continue;
    } else {
      zeroFlag = 1;
    }
    if (strFloat[i] == '.') {
      break;
    }
    cntDigitsAfterPoint++;
  }
  return cntDigitsAfterPoint;
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int error = 0;
  int sign1 = getsign(value_1);
  int sign2 = getsign(value_2);
  get_same_scale(&value_1, &value_2);
  int scale = getscale(value_1);

  // если знаки слагаемых одинаковы
  if ((sign1 ^ sign2) == 0) {
    error = add_bit_process(value_1, value_2, result);
    setsign(result, sign1);
    if (error != 0 && sign1 == 1 && sign2 == 1) {
      error = 2;
      dec_to_zero(result);
    }
    // если знаки слагаемых разные и 1 меньше 2
  } else if (is_less_or_equal_bit_process(value_1, value_2) == 1) {
    sub_bit_process(value_2, value_1, result);
    setsign(result, sign2);
    // если знаки слагаемых разные и 2 меньше 1
  } else {
    sub_bit_process(value_1, value_2, result);
    setsign(result, sign1);
  }

  if (error != 0 && getscale(value_1) != 0) {
    bank_round(&value_1);
    bank_round(&value_2);
    error = s21_add(value_1, value_2, result);
  } else {
    setscale(result, scale);
  }

  return error;
}

int add_bit_process(s21_decimal dval1, s21_decimal dval2, s21_decimal *result) {
  dec_to_zero(result);
  int carry = 0;
  for (int i = 0; i < 96; i++) {
    int bit1 = getbit(dval1, i);
    int bit2 = getbit(dval2, i);

    // 0 и 0
    if (bit1 == 0 && bit2 == 0) {
      // если есть перенос, то ставим 1, перенос обнуляем
      if (carry == 1) {
        setbit(result, i);
        carry = 0;
        // если нет переноса, то ставим 0
      } else {
        unsetbit(result, i);
      }
      // 1 и 0 либо 0 и 1
    } else if (bit1 != bit2) {
      // если есть перенос, то ставим 0, перенос сохраняем
      if (carry == 1) {
        unsetbit(result, i);
        carry = 1;
        // если нет переноса, то ставим 1
      } else {
        setbit(result, i);
      }
      // 1 и 1
    } else {
      // если есть перенос, то ставим 1, перенос сохраняем
      if (carry == 1) {
        setbit(result, i);
        carry = 1;
        // если нет переноса, то ставим 0, выставляем перенос
      } else {
        unsetbit(result, i);
        carry = 1;
      }
    }
  }
  return carry;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int sign_1 = getsign(value_1);
  int sign_2 = getsign(value_2);
  int sign = sign_1 == sign_2 ? 0 : 1;
  int scale = getscale(value_1) + getscale(value_2);
  int error = 0;

  dec_to_zero(result);

  for (int i = 0; i < 96 && error == 0; i++) {
    // ищем биты равные 1 во втором множителе
    if (getbit(value_2, i) == 1) {
      // когда нашли, пишем первый множитель в temp
      s21_decimal temp = value_1;
      int shift = 0;
      // и сдвигаем temp на i позиций для сложения в столбик
      while (shift < i) {
        if (shift_left(&temp) == 1) {
          error = 1;
          break;
        }
        shift++;
      }
      // если сдвиг не выдал ошибку, плюсуем temp в result
      if (error == 0) {
        error = add_bit_process(temp, *result, result);
      }
      if (error != 0) {
        break;
      }
    }
  }
  if (scale > 28 || error == 1) {
    error = 1;
    if (sign == 1) error = 2;
  }
  if (error == 0) {
    setsign(result, sign);
    setscale(result, scale);
  }
  return error;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  s21_decimal zero = {{0, 0, 0, 0}};
  s21_decimal mzero = {{0, 0, 0, 2147483648}};  // минус ноль
  s21_decimal ten = {{10, 0, 0, 0}};
  s21_decimal tmp_res = {{0, 0, 0, 0}};
  s21_decimal ostatok = {{0, 0, 0, 0}};
  int sign = getsign(value_1) == getsign(value_2) ? 0 : 1;
  int error = 0;

  if (s21_is_equal(value_2, zero) == 0 && s21_is_equal(value_2, mzero) == 0) {
    int scale = getscale(value_1) - getscale(value_2);
    setscale(&value_1, 0);
    setscale(&value_2, 0);
    setsign(&value_1, 0);
    setsign(&value_2, 0);
    div_bit_process(value_1, value_2, &tmp_res, &ostatok);

    *result = tmp_res;

    s21_decimal max = {{4294967295u, 4294967295u, 4294967295u, 0}};
    setscale(&max, 1);
    int exp = 0;

    // разбираемся с остатком
    while (exp < 28 && s21_is_equal(ostatok, zero) != 1) {
      if (s21_is_less(*result, max) != 1) break;
      s21_mul(ostatok, ten, &ostatok);
      div_bit_process(ostatok, value_2, &tmp_res, &ostatok);
      s21_mul(*result, ten, result);
      add_bit_process(*result, tmp_res, result);
      exp++;
    }
    scale += exp;

    while (scale > 28) {
      s21_decimal trash = {0};
      div_bit_process(*result, ten, result, &trash);
      scale--;
    }
    while (scale < 0) {
      s21_mul(*result, ten, result);
      scale++;
    }
    setsign(result, sign);
    setscale(result, scale);
  } else {
    error = 3;
    dec_to_zero(result);
  }
  return error;
}

void div_bit_process(s21_decimal value_1, s21_decimal value_2,
                     s21_decimal *result, s21_decimal *tmp) {
  dec_to_zero(result);
  dec_to_zero(tmp);
  int startbit = 95;
  // убираем все нули слева у делимого
  for (; startbit >= 0 && getbit(value_1, startbit) == 0; startbit--) {
  }
  // Начиная со старшего разряда делимого, пошагово ищем значение tmp, которое
  // имеет минимальную разность с делителем
  for (int i = startbit; i >= 0; i--) {
    if (getbit(value_1, i) == 1) {
      setbit(tmp, 0);
    }
    // если tmp больше или равно делителю, вычитаем делитель из tmp и пишем 1 в
    // частное
    if (is_greater_or_equal_bit_process(*tmp, value_2) == 1) {
      sub_bit_process(*tmp, value_2, tmp);
      if (i != 0) {
        shift_left(tmp);
      }
      shift_left(result);
      setbit(result, 0);
      // если tmp меньше делителя, пишем ноль в частное
    } else {
      shift_left(result);
      if (i != 0) {
        shift_left(tmp);
      }
    }
  }
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int error = 0;
  setsign(&value_2, !getsign(value_2));
  error = s21_add(value_1, value_2, result);

  return error;
}

void sub_bit_process(s21_decimal dval1, s21_decimal dval2,
                     s21_decimal *result) {
  dec_to_zero(result);
  int bit1, bit2, nextbit;

  for (int i = 0; i < 96; i++) {
    bit1 = getbit(dval1, i);
    bit2 = getbit(dval2, i);
    (bit1 ^ bit2) == 0 ? unsetbit(result, i) : setbit(result, i);

    // если бит уменьшаемого ноль, то нужно занимать бит в старших разрядах
    if (bit1 == 0 && bit2 != 0) {
      nextbit = i + 1;
      while ((bit1 = getbit(dval1, nextbit)) != 1) {
        setbit(&dval1, nextbit);
        nextbit++;
      }
      unsetbit(&dval1, nextbit);
    }
  }
}

void dec_to_zero(s21_decimal *number) {
  for (int i = 0; i < 128; i++) {
    unsetbit(number, i);
  }
}

int getbit(s21_decimal number, const int position) {
  return ((number.bits[position / 32] & (1 << position % 32)) != 0);
}

void setbit(s21_decimal *value, const int position) {
  value->bits[position / 32] |= (1 << (position % 32));
}

void unsetbit(s21_decimal *value, const int position) {
  value->bits[position / 32] &= ~(1 << position % 32);
}

int getsign(s21_decimal number) {
  return (number.bits[3] & (1 << 31)) == 0 ? 0 : 1;
}

void setsign(s21_decimal *number, int sign) {
  if (sign == 1) {
    number->bits[3] = number->bits[3] | (1 << 31);
  } else {
    number->bits[3] = number->bits[3] & ~(1 << 31);
  }
}

int getscale(s21_decimal number) {
  int scale = 0, i = 0;
  for (int shift = 16; shift <= 23; shift++, i++) {
    int bit = ((number.bits[3] & (1 << shift)) != 0);
    scale += bit * pow2(i);
  }
  return scale;
}

void setscale(s21_decimal *number, int scale) {
  int sign = getsign(*number);
  number->bits[3] = 0;
  (*number).bits[3] += scale << 16;
  if (sign == 1) setsign(number, 1);
}

int shift_left(s21_decimal *number) {
  int error = getbit(*number, 95);
  for (int i = 95; i >= 0; i--) {
    if (i != 0) {
      getbit(*number, i - 1) == 0 ? unsetbit(number, i) : setbit(number, i);
    } else {
      unsetbit(number, i);
    }
  }
  return error;
}

int pow2(int i) {
  int pow2 = 1;
  for (int p = 1; p <= i; p++) pow2 *= 2;

  return pow2;
}

int s21_strlen(const char *str) {
  int i;
  for (i = 0; str[i] != 0; i++) {
  }
  return i;
}

void get_same_scale(s21_decimal *dval1, s21_decimal *dval2) {
  int scale1 = getscale(*dval1);
  int scale2 = getscale(*dval2);
  if (scale1 < scale2) {
    get_same_scale_process(dval1, dval2, scale1, scale2);
  } else {
    get_same_scale_process(dval2, dval1, scale2, scale1);
  }
}

void get_same_scale_process(s21_decimal *dval1, s21_decimal *dval2, int scale1,
                            int scale2) {
  s21_decimal ten = {{10, 0, 0, 0}};
  s21_decimal trash = {0};
  if (getbit(*dval2, 93) == 0 && getbit(*dval2, 94) == 0 &&
      getbit(*dval2, 95) == 0) {
    for (int i = 0; i < (scale2 - scale1); i++) {
      s21_mul(*dval1, ten, dval1);
    }
    setscale(dval1, scale2);
  } else {
    for (int i = 0; i < (scale2 - scale1); i++) {
      div_bit_process(*dval2, ten, dval2, &trash);
    }
    setscale(dval2, scale1);
  }
}

void bank_round(s21_decimal *dec) {
  int bits3 = dec->bits[3];
  int scale = getscale(*dec) - 1;
  s21_decimal ten = {{10, 0, 0, 0}};
  s21_decimal one = {{1, 0, 0, 0}};
  s21_decimal two = {{2, 0, 0, 0}};
  s21_decimal even_odd = {{0, 0, 0, 0}};
  s21_decimal ostatok = {{0, 0, 0, 0}};
  s21_decimal trash = {0};

  //  получаем последнюю цифру (дробная часть)
  div_bit_process(*dec, ten, dec, &trash);
  s21_decimal last_digit = *dec;
  // если больше 5, то увеличиваем её на 1
  if (last_digit.bits[0] > 5) {
    add_bit_process(*dec, one, dec);
    // если равно 5, то смотрим четность целой части
  } else if (last_digit.bits[0] == 5) {
    // получаем остаток от деления на 2(чет/нечет) целой части
    // even_odd = div_bit_process(*dec, two, NULL);
    div_bit_process(*dec, two, &trash, &ostatok);
    even_odd = ostatok;

    // если нечет то увеличиваем её на 1
    if (is_equal_bit_process(one, even_odd) == 1)
      add_bit_process(*dec, one, dec);
  }
  dec->bits[3] = bits3;
  setscale(dec, scale);
}

int s21_is_less(s21_decimal dval1, s21_decimal dval2) {
  s21_decimal copydec1 = dval1;
  s21_decimal copydec2 = dval2;
  int is_less;
  int sign1 = getsign(dval1);
  int sign2 = getsign(dval2);

  if ((sign1 ^ sign2) == 1) {
    is_less = (sign1 == 1) ? 1 : 0;
  } else {
    if (getscale(dval1) != getscale(dval2)) {
      get_same_scale(&copydec1, &copydec2);
    }
    is_less = is_less_bit_process(copydec1, copydec2);
    is_less = (sign1 == 0) ? is_less : !is_less;
  }
  return is_less;
}

int is_less_bit_process(s21_decimal dval1, s21_decimal dval2) {
  int is_less = 0;
  for (int i = 95; i >= 0; i--) {
    if ((getbit(dval1, i) ^ getbit(dval2, i)) == 1) {
      is_less = getbit(dval2, i);
      break;
    }
  }
  return is_less;
}

int s21_is_equal(s21_decimal dval1, s21_decimal dval2) {
  int is_equal;
  int sign1 = getsign(dval1);
  int sign2 = getsign(dval2);
  s21_decimal zero = {{0, 0, 0, 0}};

  if ((sign1 ^ sign2) == 1) {
    is_equal = 0;
    if ((is_equal_bit_process(dval1, zero)) == 1 &&
        (is_equal_bit_process(dval2, zero)) == 1) {
      is_equal = 1;
    }
  } else {
    get_same_scale(&dval1, &dval2);
    is_equal = is_equal_bit_process(dval1, dval2);
  }
  return is_equal;
}

int is_equal_bit_process(s21_decimal dval1, s21_decimal dval2) {
  int is_equal = 1;
  for (int i = 95; i >= 0; i--) {
    if (getbit(dval1, i) != getbit(dval2, i)) {
      is_equal = 0;
      break;
    }
  }
  return is_equal;
}

int s21_is_less_or_equal(s21_decimal dval1, s21_decimal dval2) {
  int is_less_or_equal = 0;
  if (s21_is_less(dval1, dval2) == 1 || s21_is_equal(dval1, dval2) == 1) {
    is_less_or_equal = 1;
  }
  return is_less_or_equal;
}

int is_less_or_equal_bit_process(s21_decimal dval1, s21_decimal dval2) {
  int is_less_or_equal_simple = 0;
  if (is_less_bit_process(dval1, dval2) == 1 ||
      is_equal_bit_process(dval1, dval2) == 1) {
    is_less_or_equal_simple = 1;
  }
  return is_less_or_equal_simple;
}

int s21_is_greater(s21_decimal dval1, s21_decimal dval2) {
  return !s21_is_less_or_equal(dval1, dval2);
}
int s21_is_greater_bit_process(s21_decimal dval1, s21_decimal dval2) {
  return !is_less_or_equal_bit_process(dval1, dval2);
}

int s21_is_greater_or_equal(s21_decimal dval1, s21_decimal dval2) {
  int is_greater_or_equal = 0;
  if (s21_is_greater(dval1, dval2) == 1 || s21_is_equal(dval1, dval2) == 1) {
    is_greater_or_equal = 1;
  }
  return is_greater_or_equal;
}

int is_greater_or_equal_bit_process(s21_decimal dval1, s21_decimal dval2) {
  int is_greater_or_equal_simple = 0;
  if (s21_is_greater_bit_process(dval1, dval2) == 1 ||
      is_equal_bit_process(dval1, dval2) == 1) {
    is_greater_or_equal_simple = 1;
  }
  return is_greater_or_equal_simple;
}

int s21_is_not_equal(s21_decimal dval1, s21_decimal dval2) {
  return !s21_is_equal(dval1, dval2);
}
int is_not_equal_bit_process(s21_decimal dval1, s21_decimal dval2) {
  return !is_equal_bit_process(dval1, dval2);
}

// Округляет указанное Decimal число до ближайшего целого числа в сторону
// отрицательной бесконечности
int s21_floor(s21_decimal value, s21_decimal *result) {
  s21_decimal one = {{1, 0, 0, 0}};
  int sign = getsign(value);
  s21_truncate(value, result);

  if (sign == 1 && !s21_is_equal(value, *result)) {
    add_bit_process(*result, one, result);
  }
  setsign(result, sign);
  return 0;
}

// Округляет Decimal до ближайшего целого числа
int s21_round(s21_decimal value, s21_decimal *result) {
  int res = 0;
  int sign = getsign(value);
  s21_decimal val_intpart = {{0, 0, 0, 0}};
  s21_decimal val_no_intpart = {{0, 0, 0, 0}};
  s21_decimal five = {{5, 0, 0, 0}};
  s21_decimal one = {{1, 0, 0, 0}};
  setsign(&value, 0);
  setscale(&five, 1);

  if (result == NULL) {
    res = 1;
  } else {
    dec_to_zero(result);
    s21_truncate(value, &val_intpart);
    s21_sub(value, val_intpart, &val_no_intpart);

    if (s21_is_greater_or_equal(val_no_intpart, five) == 1) {
      s21_add(val_intpart, one, result);
      setscale(result, 0);
      setsign(result, sign);
    } else {
      *result = val_intpart;
      setscale(result, 0);
      setsign(result, sign);
    }
  }
  return res;
}

// Возвращает целые цифры указанного Decimal числа; любые дробные цифры
// отбрасываются, включая конечные нули
int s21_truncate(s21_decimal value, s21_decimal *result) {
  int sign = getsign(value);
  int scale = getscale(value);
  s21_decimal ten = {{10, 0, 0, 0}};
  s21_decimal trash = {0};
  *result = value;

  while (scale != 0) {
    div_bit_process(*result, ten, result, &trash);
    scale--;
  }
  setsign(result, sign);
  return 0;
}

// Возвращает результат умножения указанного Decimal на -1
int s21_negate(s21_decimal value, s21_decimal *result) {
  *result = value;
  setsign(result, !getsign(value));
  return 0;
}
/*
void print_decimal(s21_decimal num) {
  char str[149];
  str[149] = '\0';
  int cnt = 148;

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 32; j++) {
      if (j == 0 && i != 0) {
        str[cnt] = ' ';
        cnt--;
      }
      str[cnt] = (char)(((num.bits[i] >> j) & 1u) + 48);
      cnt--;
      if (j == 7 || j == 15 || j == 23 || (j == 31 && i != 3)) {
        str[cnt] = ' ';
        cnt--;
      }
      if (j == 31 && i != 3) {
        str[cnt] = '|';
        cnt--;
      }
    }
  }
  printf(
      "127-120  119-112  111-104   103-96  |  95-88    87-80    79-72    "
      "71-64 "
      "  |  63-56    55-48    47-40    39-32   |  31-24    23-16     15-8    "
      " "
      "7-0   \n");
}
void print_float(float num) {
  int float_bits = *((unsigned int *)&num);
  char str[34];
  str[34] = '\0';
  int cnt = 33;
  for (int j = 0; j < 32; j++) {
    if (j == 23 || j == 31) {
      str[cnt] = ' ';
      cnt--;
    }
    str[cnt] = (char)(((float_bits >> j) & 1u) + 48);
    cnt--;
  }
  printf("%s\n", str);
  printf("31   30-23         22-0        \n");
}
*/