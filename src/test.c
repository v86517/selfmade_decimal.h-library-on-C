#include <check.h>

#include "s21_decimal.h"

//////////////////////////int_to_decimal////////////////////////////
START_TEST(int_to_decimal_1) {
  int src = 2147483647;
  s21_decimal check = {{2147483647, 0, 0, 0}};
  s21_decimal result;
  int return_value = s21_from_int_to_decimal(src, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(int_to_decimal_2) {
  int src = -2147483648;
  s21_decimal check = {{2147483648, 0, 0, 0}};
  setsign(&check, 1);
  s21_decimal result;
  int return_value = s21_from_int_to_decimal(src, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(int_to_decimal_3) {
  int src = -49135648;
  s21_decimal check = {{49135648, 0, 0, 0}};
  setsign(&check, 1);
  s21_decimal result;
  int return_value = s21_from_int_to_decimal(src, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

//////////////////////////float_to_decimal////////////////////////////
START_TEST(float_to_decimal_1) {
  s21_decimal value = {{0, 0, 0, 0}};
  s21_decimal value_2 = {{9, 0, 0, 0}};
  setscale(&value_2, 1);
  setsign(&value_2, 1);
  float a = -0.9;
  int res = s21_from_float_to_decimal(a, &value);
  ck_assert_int_eq(s21_is_equal(value_2, value), 1);
  ck_assert_int_eq(res, 0);
}
END_TEST

//////////////////////////decimal_to_float////////////////////////////
START_TEST(decimal_to_float_1) {
  s21_decimal a = {{5, 0, 0, 2147483648}};
  float dec_to_float_a = 0;

  s21_from_decimal_to_float(a, &dec_to_float_a);
  ck_assert_float_eq(dec_to_float_a, -5.0);
}
END_TEST

START_TEST(decimal_to_float_2) {
  s21_decimal a = {{13, 0, 0, 0}};
  float dec_to_float_a = 0;

  s21_from_decimal_to_float(a, &dec_to_float_a);
  ck_assert_float_eq(dec_to_float_a, 13.0);
}
END_TEST

START_TEST(decimal_to_float_3) {
  s21_decimal value = {{9, 0, 0, 0}};
  setscale(&value, 1);
  setsign(&value, 1);
  float a = -0.9, b;
  int res = s21_from_decimal_to_float(value, &b);
  ck_assert_float_eq(a, b);
  ck_assert_int_eq(res, 0);
}
END_TEST

//////////////////////////decimal_to_int////////////////////////////
START_TEST(decimal_to_int_1) {
  s21_decimal a = {{123, 0, 0, 0}};
  setscale(&a, 1);
  int int_a;

  s21_from_decimal_to_int(a, &int_a);
  ck_assert_int_eq(int_a, 12);
}
END_TEST

START_TEST(decimal_to_int_2) {
  s21_decimal value = {{49135648, 0, 0, 0}};
  setsign(&value, 1);
  setscale(&value, 1);
  int dst;
  int check = -4913564;
  int return_value = s21_from_decimal_to_int(value, &dst);
  ck_assert_int_eq(dst, check);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(decimal_to_int_3) {
  s21_decimal value = {{49135648, 0, 0, 0}};
  setscale(&value, 1);
  int dst;
  int check = 4913564;
  int return_value = s21_from_decimal_to_int(value, &dst);
  ck_assert_int_eq(dst, check);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

///////////////////////////less_greater_equal////////////////////////////
START_TEST(less_or_equal_1) {
  s21_decimal a = {{0, 12345, 6789, 101111}}, b = {{0, 12345, 6789, 101111}};
  setscale(&a, 5);
  setscale(&b, 5);

  ck_assert_int_eq(s21_is_less_or_equal(a, b), 1);
}
END_TEST

START_TEST(less_or_equal_2) {
  s21_decimal value_1 = {{123457u, 654u, 0xFFFFFFFF, 0}};
  s21_decimal value_2 = {{123456u, 654u, 0xFFFFFFFF, 0}};
  int return_value = s21_is_less_or_equal(value_1, value_2);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(less_or_equal_3) {
  s21_decimal value_1 = {{123457u, 654u, 0xFFFFFFFF, 0}};
  s21_decimal value_2 = {{123456u, 654u, 0xFFFFFFFF, 0}};
  setsign(&value_1, 1);
  int return_value = s21_is_less_or_equal(value_1, value_2);
  ck_assert_int_eq(return_value, 1);
}
END_TEST

START_TEST(less_or_equal_4) {
  s21_decimal value_1 = {{123456u, 654u, 0xFFFFFFFF, 0}};
  s21_decimal value_2 = {{123457u, 654u, 0xFFFFFFFF, 0}};
  setsign(&value_2, 1);
  int return_value = s21_is_less_or_equal(value_1, value_2);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(less_or_equal_5) {
  s21_decimal value_1 = {{0, 0, 0, 0}};
  s21_decimal value_2 = {{0, 0, 0, 0}};
  setsign(&value_2, 1);
  int return_value = s21_is_less_or_equal(value_1, value_2);
  ck_assert_int_eq(return_value, 1);
}
END_TEST

START_TEST(less_or_equal_6) {
  s21_decimal value_1 = {{123456u, 654u, 0xFFFFFFFF, 0}};
  s21_decimal value_2 = {{123457u, 654u, 0xFFFFFFFF, 0}};
  setsign(&value_1, 1);
  setsign(&value_2, 1);
  int return_value = s21_is_less_or_equal(value_1, value_2);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(less_or_equal_7) {
  s21_decimal value_1 = {{123456u, 654u, 0xFFFFFFFF, 0}};
  s21_decimal value_2 = {{123456u, 654u, 0xFFFFFFFF, 0}};
  setscale(&value_2, 2);
  int return_value = s21_is_less_or_equal(value_1, value_2);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(less_or_equal_8) {
  s21_decimal value_1 = {{123456u, 654u, 0xFFFFFFFF, 0}};
  s21_decimal value_2 = {{123456u, 654u, 0xFFFFFFFF, 0}};
  int return_value = s21_is_less_or_equal(value_1, value_2);
  ck_assert_int_eq(return_value, 1);
}
END_TEST

START_TEST(greater_1) {
  s21_decimal a = {{0, 12345, 6889, 101112}}, b = {{0, 12345, 6789, 101112}};
  setscale(&a, 5);
  setscale(&b, 5);

  ck_assert_int_eq(s21_is_greater(a, b), 1);
}
END_TEST

START_TEST(greater_2) {
  s21_decimal value_1 = {{123457u, 654u, 0xFFFFFFFF, 0}};
  s21_decimal value_2 = {{123456u, 654u, 0xFFFFFFFF, 0}};
  int return_value = s21_is_greater(value_1, value_2);
  ck_assert_int_eq(return_value, 1);
}
END_TEST

START_TEST(greater_3) {
  s21_decimal value_1 = {{123457u, 654u, 0xFFFFFFFF, 0}};
  s21_decimal value_2 = {{123456u, 654u, 0xFFFFFFFF, 0}};
  setsign(&value_1, 1);
  int return_value = s21_is_greater(value_1, value_2);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(greater_4) {
  s21_decimal value_1 = {{123456u, 654u, 0xFFFFFFFF, 0}};
  s21_decimal value_2 = {{123457u, 654u, 0xFFFFFFFF, 0}};
  setsign(&value_2, 1);
  int return_value = s21_is_greater(value_1, value_2);
  ck_assert_int_eq(return_value, 1);
}
END_TEST

START_TEST(greater_5) {
  s21_decimal value_1 = {{0, 0, 0, 0}};
  s21_decimal value_2 = {{0, 0, 0, 0}};
  setsign(&value_2, 1);
  int return_value = s21_is_greater(value_1, value_2);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(greater_6) {
  s21_decimal value_1 = {{123456u, 654u, 0xFFFFFFFF, 0}};
  s21_decimal value_2 = {{123457u, 654u, 0xFFFFFFFF, 0}};
  setsign(&value_1, 1);
  setsign(&value_2, 1);
  int return_value = s21_is_greater(value_1, value_2);
  ck_assert_int_eq(return_value, 1);
}
END_TEST

START_TEST(greater_7) {
  s21_decimal value_1 = {{123456u, 654u, 0xFFFFFFFF, 0}};
  s21_decimal value_2 = {{123456u, 654u, 0xFFFFFFFF, 0}};
  setscale(&value_2, 10);
  int return_value = s21_is_greater(value_1, value_2);
  ck_assert_int_eq(return_value, 1);
}
END_TEST

START_TEST(greater_8) {
  s21_decimal value_1 = {{234, 0, 0, 0}};
  s21_decimal value_2 = {{2, 0, 0, 0}};
  setscale(&value_1, 2);
  int return_value = s21_is_greater(value_1, value_2);
  ck_assert_int_eq(return_value, 1);
}
END_TEST

START_TEST(greater_or_equal_1) {
  s21_decimal a = {{0, 12345, 6789, 101112}}, b = {{0, 12345, 6789, 101112}};
  setscale(&a, 5);
  setscale(&b, 5);

  ck_assert_int_eq(s21_is_greater_or_equal(a, b), 1);
}
END_TEST

START_TEST(greater_or_equal_2) {
  s21_decimal value_1 = {{123457u, 654u, 0xFFFFFFFF, 0}};
  s21_decimal value_2 = {{123456u, 654u, 0xFFFFFFFF, 0}};
  int return_value = s21_is_greater_or_equal(value_1, value_2);
  ck_assert_int_eq(return_value, 1);
}
END_TEST

START_TEST(greater_or_equal_3) {
  s21_decimal value_1 = {{123457u, 654u, 0xFFFFFFFF, 0}};
  s21_decimal value_2 = {{123456u, 654u, 0xFFFFFFFF, 0}};
  setsign(&value_1, 1);
  int return_value = s21_is_greater_or_equal(value_1, value_2);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(greater_or_equal_4) {
  s21_decimal value_1 = {{123456u, 654u, 0xFFFFFFFF, 0}};
  s21_decimal value_2 = {{123457u, 654u, 0xFFFFFFFF, 0}};
  setsign(&value_2, 1);
  int return_value = s21_is_greater_or_equal(value_1, value_2);
  ck_assert_int_eq(return_value, 1);
}
END_TEST

START_TEST(greater_or_equal_5) {
  s21_decimal value_1 = {{0, 0, 0, 0}};
  s21_decimal value_2 = {{0, 0, 0, 0}};
  setsign(&value_2, 1);
  int return_value = s21_is_greater_or_equal(value_1, value_2);
  ck_assert_int_eq(return_value, 1);
}
END_TEST

START_TEST(greater_or_equal_6) {
  s21_decimal value_1 = {{123456u, 654u, 0xFFFFFFFF, 0}};
  s21_decimal value_2 = {{123457u, 654u, 0xFFFFFFFF, 0}};
  setsign(&value_1, 1);
  setsign(&value_2, 1);
  int return_value = s21_is_greater_or_equal(value_1, value_2);
  ck_assert_int_eq(return_value, 1);
}
END_TEST

START_TEST(greater_or_equal_7) {
  s21_decimal value_1 = {{123456u, 654u, 0xFFFFFFFF, 0}};
  s21_decimal value_2 = {{123456u, 654u, 0xFFFFFFFF, 0}};
  setscale(&value_2, 2);
  int return_value = s21_is_greater_or_equal(value_1, value_2);
  ck_assert_int_eq(return_value, 1);
}
END_TEST

START_TEST(greater_or_equal_8) {
  s21_decimal value_1 = {{123456u, 654u, 0xFFFFFFFF, 0}};
  s21_decimal value_2 = {{123456u, 654u, 0xFFFFFFFF, 0}};
  int return_value = s21_is_greater_or_equal(value_1, value_2);
  ck_assert_int_eq(return_value, 1);
}
END_TEST

START_TEST(not_equal_1) {
  s21_decimal a = {{0, 12346, 12, 101113}}, b = {{0, 12345, 6789, 101112}};
  setscale(&a, 5);
  setscale(&b, 5);
  setsign(&a, 1);
  setsign(&b, 1);

  ck_assert_int_eq(s21_is_not_equal(a, b), 1);
}
END_TEST

START_TEST(not_equal_2) {
  s21_decimal value_1 = {{123456u, 654u, 0xFFFFFFFF, 80000000}};
  s21_decimal value_2 = {{123456u, 654u, 0xFFFFFFFF, 80000000}};
  int return_value = s21_is_not_equal(value_1, value_2);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(not_equal_3) {
  s21_decimal value_1 = {{123453u, 654u, 0xFFFFFFFF, 80000000}};
  s21_decimal value_2 = {{123456u, 654u, 0xFFFFFFFF, 80000000}};
  int return_value = s21_is_not_equal(value_1, value_2);
  ck_assert_int_eq(return_value, 1);
}
END_TEST

START_TEST(not_equal_4) {
  s21_decimal value_1 = {{0, 0, 0, 80000000}};
  s21_decimal value_2 = {{0, 0, 0, 0}};
  int return_value = s21_is_not_equal(value_1, value_2);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(equal_1) {
  s21_decimal value_1 = {{123456u, 654u, 0xFFFFFFFF, 80000000}};
  s21_decimal value_2 = {{123456u, 654u, 0xFFFFFFFF, 80000000}};
  int return_value = s21_is_equal(value_1, value_2);
  ck_assert_int_eq(return_value, 1);
}
END_TEST

START_TEST(equal_2) {
  s21_decimal value_1 = {{123453u, 654u, 0xFFFFFFFF, 80000000}};
  s21_decimal value_2 = {{123456u, 654u, 0xFFFFFFFF, 80000000}};
  int return_value = s21_is_equal(value_1, value_2);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(equal_3) {
  s21_decimal value_1 = {{0, 0, 0, 0}};
  s21_decimal value_2 = {{0, 0, 0, 0x80000000}};
  int return_value = s21_is_equal(value_1, value_2);
  ck_assert_int_eq(return_value, 1);
}
END_TEST

START_TEST(equal_4) {
  s21_decimal value_1 = {{2, 0, 0, 0}};
  s21_decimal value_2 = {{20, 0, 0, 0}};
  setscale(&value_2, 1);
  int return_value = s21_is_equal(value_1, value_2);
  ck_assert_int_eq(return_value, 1);
}
END_TEST

START_TEST(less_1) {
  s21_decimal value_1 = {{123457u, 654u, 0xFFFFFFFF, 0}};
  s21_decimal value_2 = {{123456u, 654u, 0xFFFFFFFF, 0}};
  int return_value = s21_is_less(value_1, value_2);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(less_2) {
  s21_decimal value_1 = {{123457u, 654u, 0xFFFFFFFF, 0}};
  s21_decimal value_2 = {{123456u, 654u, 0xFFFFFFFF, 0}};
  setsign(&value_1, 1);
  int return_value = s21_is_less(value_1, value_2);
  ck_assert_int_eq(return_value, 1);
}
END_TEST

START_TEST(less_3) {
  s21_decimal value_1 = {{123456u, 654u, 0xFFFFFFFF, 0}};
  s21_decimal value_2 = {{123457u, 654u, 0xFFFFFFFF, 0}};
  setsign(&value_2, 1);
  int return_value = s21_is_less(value_1, value_2);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(less_4) {
  s21_decimal value_1 = {{0, 0, 0, 0}};
  s21_decimal value_2 = {{0, 0, 0, 0}};
  setsign(&value_2, 1);
  int return_value = s21_is_less(value_1, value_2);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(less_5) {
  s21_decimal value_1 = {{123456u, 654u, 0xFFFFFFFF, 0}};
  s21_decimal value_2 = {{123457u, 654u, 0xFFFFFFFF, 0}};
  setsign(&value_1, 1);
  setsign(&value_2, 1);
  int return_value = s21_is_less(value_1, value_2);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(less_6) {
  s21_decimal value_1 = {{123456u, 654u, 0xFFFFFFFF, 0}};
  s21_decimal value_2 = {{123456u, 654u, 0xFFFFFFFF, 0}};
  setscale(&value_2, 2);
  int return_value = s21_is_less(value_1, value_2);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(add_1) {
  s21_decimal value_1 = {{13, 0, 0, 0}};
  setscale(&value_1, 1);
  s21_decimal value_2 = {{286, 0, 0, 0}};
  setscale(&value_2, 2);
  s21_decimal result;
  s21_decimal check = {{416, 0, 0, 0}};
  setscale(&check, 2);
  int return_value = s21_add(value_1, value_2, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(add_2) {
  s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal value_2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  setsign(&value_2, 1);
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal check = {{0, 0, 0, 0}};
  int return_value = s21_add(value_1, value_2, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(add_3) {
  s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0, 0}};
  setscale(&value_1, 1);
  s21_decimal value_2 = {{2, 0, 0, 0}};
  setscale(&value_2, 1);
  s21_decimal result;
  s21_decimal check = {{0x1, 0x0, 0x1, 0}};
  setscale(&check, 1);
  int return_value = s21_add(value_1, value_2, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(add_4) {
  s21_decimal value_1 = {{13, 0, 0, 0}};
  setscale(&value_1, 2);
  s21_decimal value_2 = {{286, 0, 0, 0}};
  setscale(&value_2, 1);
  s21_decimal result;
  s21_decimal check = {{2873, 0, 0, 0}};
  setscale(&check, 2);
  int return_value = s21_add(value_1, value_2, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(add_5) {
  s21_decimal value_1 = {{1, 0, 0, 0}};
  s21_decimal value_2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  setscale(&value_2, 1);
  s21_decimal result;
  s21_decimal check = {{0x9999999a, 0x99999999, 0x19999999, 0}};
  int return_value = s21_add(value_1, value_2, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(add_6) {
  s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  setscale(&value_1, 1);
  s21_decimal value_2 = {{1, 0, 0, 0}};
  s21_decimal result;
  s21_decimal check = {{0x9999999a, 0x99999999, 0x19999999, 0}};
  int return_value = s21_add(value_1, value_2, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(add_7) {
  s21_decimal value_1 = {{1, 0, 0, 0}};
  s21_decimal value_2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal result;
  s21_decimal check = {{0, 0, 0, 0}};
  int return_value = s21_add(value_1, value_2, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 1);
}
END_TEST

START_TEST(add_8) {
  s21_decimal value_1 = {{13, 0, 0, 0}};
  setscale(&value_1, 2);
  setsign(&value_1, 1);
  s21_decimal value_2 = {{286, 0, 0, 0}};
  setscale(&value_2, 1);
  s21_decimal result;
  s21_decimal check = {{2847, 0, 0, 0}};
  setscale(&check, 2);
  int return_value = s21_add(value_1, value_2, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(add_9) {
  s21_decimal value_1 = {{13, 0, 0, 0}};
  setscale(&value_1, 1);
  setsign(&value_1, 1);
  s21_decimal value_2 = {{286, 0, 0, 0}};
  setscale(&value_2, 2);
  setsign(&value_2, 1);
  s21_decimal result;
  s21_decimal check = {{416, 0, 0, 0}};
  setscale(&check, 2);
  setsign(&check, 1);
  int return_value = s21_add(value_1, value_2, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(add_10) {
  s21_decimal value_1 = {{1614, 0, 0, 0}};
  setscale(&value_1, 3);
  s21_decimal value_2 = {{46071, 0, 0, 0}};
  setscale(&value_2, 2);
  s21_decimal result;
  s21_decimal check = {{462324, 0, 0, 0}};
  setscale(&check, 3);
  int return_value = s21_add(value_1, value_2, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(sub_1) {
  s21_decimal value_1 = {{13, 0, 0, 0}};
  setscale(&value_1, 1);
  setsign(&value_1, 1);
  s21_decimal value_2 = {{286, 0, 0, 0}};
  setscale(&value_2, 2);
  setsign(&value_2, 1);
  s21_decimal result;
  s21_decimal check = {{156, 0, 0, 0}};
  setscale(&check, 2);
  int return_value = s21_sub(value_1, value_2, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(sub_2) {
  s21_decimal value_1 = {{13, 0, 0, 0}};
  setscale(&value_1, 1);
  setsign(&value_1, 1);
  s21_decimal value_2 = {{286, 0, 0, 0}};
  setscale(&value_2, 2);
  s21_decimal result;
  s21_decimal check = {{416, 0, 0, 0}};
  setscale(&check, 2);
  setsign(&check, 1);
  int return_value = s21_sub(value_1, value_2, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(sub_3) {
  s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  setsign(&value_1, 1);
  s21_decimal value_2 = {{2, 0, 0, 0}};
  s21_decimal result;
  s21_decimal check = {{0, 0, 0, 0}};
  int return_value = s21_sub(value_1, value_2, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 2);
}
END_TEST

START_TEST(sub_4) {
  s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  setsign(&value_1, 1);
  s21_decimal value_2 = {{2, 0, 0, 0}};
  setsign(&value_2, 1);
  s21_decimal result;
  s21_decimal check = {{0xFFFFFFFD, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  int return_value = s21_sub(value_1, value_2, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(sub_5) {
  s21_decimal value_1 = {{64071, 0, 0, 0}};
  setscale(&value_1, 4);
  s21_decimal value_2 = {{5919, 0, 0, 0}};
  setscale(&value_2, 1);
  s21_decimal result;
  s21_decimal check = {{5854929, 0, 0, 0}};
  setscale(&check, 4);
  setsign(&check, 1);
  int return_value = s21_sub(value_1, value_2, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(mul_1) {
  s21_decimal value_1 = {{5, 0, 0, 0}};
  s21_decimal value_2 = {{7, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal check = {{35, 0, 0, 0}};
  int return_value = s21_mul(value_1, value_2, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(mul_2) {
  s21_decimal value_1 = {{0xFFFFFFFF, 0, 0, 0}};
  s21_decimal value_2 = {{0xFFFFFFFF, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal check = {{1, 0xFFFFFFFE, 0, 0}};
  int return_value = s21_mul(value_1, value_2, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(mul_3) {
  s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal value_2 = {{2, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal check = {{0, 0, 0, 0}};
  int return_value = s21_mul(value_1, value_2, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 1);
}
END_TEST

START_TEST(mul_4) {
  s21_decimal value_1 = {{123456u, 123u, 0, 0}};
  s21_decimal value_2 = {{654321u, 654u, 0, 0}};
  setsign(&value_2, 1);
  setscale(&value_1, 2);
  setscale(&value_2, 3);
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal check = {{0xcedabe40, 0x99c0c5d, 0x13a3a, 0x80050000}};
  int return_value = s21_mul(value_1, value_2, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(mul_5) {
  s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal value_2 = {{2, 0, 0, 0}};
  setsign(&value_2, 1);
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal check = {{0, 0, 0, 0}};
  int return_value = s21_mul(value_1, value_2, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 2);
}
END_TEST

START_TEST(mul_6) {
  s21_decimal value_1 = {{17, 0, 0, 0}};
  setsign(&value_1, 1);
  s21_decimal value_2 = {{0, 0, 0, 0}};
  s21_decimal result;
  s21_decimal check = {{0, 0, 0, 0}};
  int return_value = s21_mul(value_1, value_2, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(div_1) {
  s21_decimal value_1 = {{35, 0, 0, 0}};
  s21_decimal value_2 = {{5, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal check = {{70, 0, 0, 0}};
  setscale(&value_1, 1);
  setscale(&value_2, 2);
  int return_value = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(div_2) {
  s21_decimal value_1 = {{0x88888888, 0x88888888, 0x88888888, 0}};
  s21_decimal value_2 = {{0x2, 0, 0, 0}};
  setsign(&value_2, 1);
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal check = {{0x44444444, 0x44444444, 0x44444444, 0}};
  setsign(&check, 1);
  int return_value = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(div_3) {
  s21_decimal value_1 = {{10, 0, 0, 0}};
  setsign(&value_1, 1);
  s21_decimal value_2 = {{8, 0, 0, 0}};
  setsign(&value_2, 1);
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal check = {{125, 0, 0, 0}};
  setscale(&check, 2);
  int return_value = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(div_4) {
  s21_decimal value_1 = {{15, 0, 0, 0}};
  setsign(&value_1, 1);
  s21_decimal value_2 = {{0, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal check = {{0, 0, 0, 0}};
  int return_value = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 3);
}
END_TEST

START_TEST(div_5) {
  s21_decimal value_1 = {{10, 0, 0, 0}};
  s21_decimal value_2 = {{1, 0, 0, 0}};
  setscale(&value_2, 2);
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal check = {{1000u, 0, 0, 0}};
  int return_value = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(truncate_1) {
  s21_decimal value_1 = {{35, 0, 0, 0}};
  s21_decimal check = {{3, 0, 0, 0}};
  setscale(&value_1, 1);
  s21_truncate(value_1, &value_1);
  ck_assert_int_eq(s21_is_equal(value_1, check), 1);
}
END_TEST

START_TEST(truncate_2) {
  s21_decimal value_1 = {{123456, 0, 0, 0}};
  setscale(&value_1, 3);
  setsign(&value_1, 1);
  s21_decimal check = {{123, 0, 0, 0}};
  setsign(&check, 1);
  s21_truncate(value_1, &value_1);
  ck_assert_int_eq(s21_is_equal(value_1, check), 1);
}
END_TEST

START_TEST(negate_1) {
  s21_decimal value_1 = {{5, 0xFFFFFFFF, 0, 0}};
  setsign(&value_1, 1);
  s21_decimal check = {{5, 0xFFFFFFFF, 0, 0}};
  s21_decimal result;
  int return_value = s21_negate(value_1, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(negate_2) {
  s21_decimal value_1 = {{1, 1, 1, 0}};
  s21_decimal check = {{1, 1, 1, 0x80000000}};
  s21_decimal result;
  int return_value = s21_negate(value_1, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(round_1) {
  s21_decimal value_1 = {{7464923, 0, 0, 0}};
  setscale(&value_1, 5);
  s21_decimal check = {{75, 0, 0, 0}};
  s21_decimal result;
  int return_value = s21_round(value_1, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(round_2) {
  s21_decimal value_1 = {{7444923, 0, 0, 0}};
  setscale(&value_1, 5);
  s21_decimal check = {{74, 0, 0, 0}};
  s21_decimal result;
  int return_value = s21_round(value_1, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(round_3) {
  s21_decimal value_1 = {{7464923, 0, 0, 0}};
  setsign(&value_1, 1);
  setscale(&value_1, 5);
  s21_decimal check = {{75, 0, 0, 0}};
  setsign(&check, 1);
  s21_decimal result;
  int return_value = s21_round(value_1, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(round_4) {
  s21_decimal value_1 = {{7444923, 0, 0, 0}};
  setsign(&value_1, 1);
  setscale(&value_1, 5);
  s21_decimal check = {{74, 0, 0, 0}};
  setsign(&check, 1);
  s21_decimal result;
  int return_value = s21_round(value_1, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(floor_1) {
  s21_decimal value_1 = {{7444923, 0, 0, 0}};
  setscale(&value_1, 5);
  s21_decimal check = {{74, 0, 0, 0}};
  s21_decimal result;
  int return_value = s21_floor(value_1, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(floor_2) {
  s21_decimal value_1 = {{7444923, 0, 0, 0}};
  setscale(&value_1, 5);
  setsign(&value_1, 1);
  s21_decimal check = {{75, 0, 0, 0}};
  setsign(&check, 1);
  s21_decimal result;
  int return_value = s21_floor(value_1, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

Suite *s21_from_int_to_decimal_suite(void) {
  Suite *s = suite_create("*********int_to_decimal*******");
  TCase *tc = tcase_create("decimal");

  tcase_add_test(tc, int_to_decimal_1);
  tcase_add_test(tc, int_to_decimal_2);
  tcase_add_test(tc, int_to_decimal_3);

  suite_add_tcase(s, tc);

  return s;
}

Suite *s21_from_float_to_decimal_suite(void) {
  Suite *s = suite_create("*********float_to_decimal*******");
  TCase *tc = tcase_create("decimal");

  tcase_add_test(tc, float_to_decimal_1);

  suite_add_tcase(s, tc);

  return s;
}

Suite *s21_from_decimal_to_float_suite(void) {
  Suite *s = suite_create("*********decimal_to_float*******");
  TCase *tc = tcase_create("decimal");

  tcase_add_test(tc, decimal_to_float_1);
  tcase_add_test(tc, decimal_to_float_2);
  tcase_add_test(tc, decimal_to_float_3);

  suite_add_tcase(s, tc);

  return s;
}

Suite *s21_from_decimal_to_int_suite(void) {
  Suite *s = suite_create("*********decimal_to_int*******");
  TCase *tc = tcase_create("decimal");

  tcase_add_test(tc, decimal_to_int_1);
  tcase_add_test(tc, decimal_to_int_2);
  tcase_add_test(tc, decimal_to_int_3);

  suite_add_tcase(s, tc);

  return s;
}

Suite *s21_is_less_or_equal_suite(void) {
  Suite *s = suite_create("*********less_or_equal*******");
  TCase *tc = tcase_create("decimal");

  tcase_add_test(tc, less_or_equal_1);
  tcase_add_test(tc, less_or_equal_2);
  tcase_add_test(tc, less_or_equal_3);
  tcase_add_test(tc, less_or_equal_4);
  tcase_add_test(tc, less_or_equal_5);
  tcase_add_test(tc, less_or_equal_6);
  tcase_add_test(tc, less_or_equal_7);
  tcase_add_test(tc, less_or_equal_8);

  suite_add_tcase(s, tc);

  return s;
}

Suite *s21_is_greater_suite(void) {
  Suite *s = suite_create("*********greater*******");
  TCase *tc = tcase_create("decimal");

  tcase_add_test(tc, greater_1);
  tcase_add_test(tc, greater_2);
  tcase_add_test(tc, greater_3);
  tcase_add_test(tc, greater_4);
  tcase_add_test(tc, greater_5);
  tcase_add_test(tc, greater_6);
  tcase_add_test(tc, greater_7);
  tcase_add_test(tc, greater_8);

  suite_add_tcase(s, tc);

  return s;
}

Suite *s21_is_greater_or_equal_suite(void) {
  Suite *s = suite_create("*********greater_or_equal*******");
  TCase *tc = tcase_create("decimal");

  tcase_add_test(tc, greater_or_equal_1);
  tcase_add_test(tc, greater_or_equal_2);
  tcase_add_test(tc, greater_or_equal_3);
  tcase_add_test(tc, greater_or_equal_4);
  tcase_add_test(tc, greater_or_equal_5);
  tcase_add_test(tc, greater_or_equal_6);
  tcase_add_test(tc, greater_or_equal_7);
  tcase_add_test(tc, greater_or_equal_8);

  suite_add_tcase(s, tc);

  return s;
}

Suite *s21_is_not_equal_suite(void) {
  Suite *s = suite_create("*********not_equal*******");
  TCase *tc = tcase_create("decimal");

  tcase_add_test(tc, not_equal_1);
  tcase_add_test(tc, not_equal_2);
  tcase_add_test(tc, not_equal_3);
  tcase_add_test(tc, not_equal_4);

  suite_add_tcase(s, tc);

  return s;
}

Suite *s21_is_equal_suite(void) {
  Suite *s = suite_create("*********equal*******");
  TCase *tc = tcase_create("decimal");

  tcase_add_test(tc, equal_1);
  tcase_add_test(tc, equal_2);
  tcase_add_test(tc, equal_3);
  tcase_add_test(tc, equal_4);

  suite_add_tcase(s, tc);

  return s;
}

Suite *s21_is_less_suite(void) {
  Suite *s = suite_create("*********less*******");
  TCase *tc = tcase_create("decimal");

  tcase_add_test(tc, less_1);
  tcase_add_test(tc, less_2);
  tcase_add_test(tc, less_3);
  tcase_add_test(tc, less_4);
  tcase_add_test(tc, less_5);
  tcase_add_test(tc, less_6);

  suite_add_tcase(s, tc);

  return s;
}

Suite *s21_add_suite(void) {
  Suite *s = suite_create("*********add*******");
  TCase *tc = tcase_create("decimal");

  tcase_add_test(tc, add_1);
  tcase_add_test(tc, add_2);
  tcase_add_test(tc, add_3);
  tcase_add_test(tc, add_4);
  tcase_add_test(tc, add_5);
  tcase_add_test(tc, add_6);
  tcase_add_test(tc, add_7);
  tcase_add_test(tc, add_8);
  tcase_add_test(tc, add_9);
  tcase_add_test(tc, add_10);

  suite_add_tcase(s, tc);

  return s;
}

Suite *s21_sub_suite(void) {
  Suite *s = suite_create("*********sub*******");
  TCase *tc = tcase_create("decimal");

  tcase_add_test(tc, sub_1);
  tcase_add_test(tc, sub_2);
  tcase_add_test(tc, sub_3);
  tcase_add_test(tc, sub_4);
  tcase_add_test(tc, sub_5);

  suite_add_tcase(s, tc);

  return s;
}

Suite *s21_mul_suite(void) {
  Suite *s = suite_create("*********mul*******");
  TCase *tc = tcase_create("decimal");

  tcase_add_test(tc, mul_1);
  tcase_add_test(tc, mul_2);
  tcase_add_test(tc, mul_3);
  tcase_add_test(tc, mul_4);
  tcase_add_test(tc, mul_5);
  tcase_add_test(tc, mul_6);

  suite_add_tcase(s, tc);

  return s;
}

Suite *s21_div_suite(void) {
  Suite *s = suite_create("*********div*******");
  TCase *tc = tcase_create("decimal");

  tcase_add_test(tc, div_1);
  tcase_add_test(tc, div_2);
  tcase_add_test(tc, div_3);
  tcase_add_test(tc, div_4);
  tcase_add_test(tc, div_5);

  suite_add_tcase(s, tc);

  return s;
}

Suite *s21_truncate_suite(void) {
  Suite *s = suite_create("*********truncate*******");
  TCase *tc = tcase_create("decimal");

  tcase_add_test(tc, truncate_1);
  tcase_add_test(tc, truncate_2);

  suite_add_tcase(s, tc);

  return s;
}

Suite *s21_negate_suite(void) {
  Suite *s = suite_create("*********negate*******");
  TCase *tc = tcase_create("decimal");

  tcase_add_test(tc, negate_1);
  tcase_add_test(tc, negate_2);

  suite_add_tcase(s, tc);

  return s;
}

Suite *s21_round_suite(void) {
  Suite *s = suite_create("*********round*******");
  TCase *tc = tcase_create("decimal");

  tcase_add_test(tc, round_1);
  tcase_add_test(tc, round_2);
  tcase_add_test(tc, round_3);
  tcase_add_test(tc, round_4);

  suite_add_tcase(s, tc);

  return s;
}

Suite *s21_floor_suite(void) {
  Suite *s = suite_create("*********floor*******");
  TCase *tc = tcase_create("decimal");

  tcase_add_test(tc, floor_1);
  tcase_add_test(tc, floor_2);

  suite_add_tcase(s, tc);

  return s;
}

int s21_decimal_tests(Suite *s) {
  int failed_count;
  SRunner *runner = srunner_create(s);

  srunner_run_all(runner, CK_NORMAL);

  failed_count = srunner_ntests_failed(runner);
  srunner_free(runner);

  if (failed_count != 0) {
    failed_count = 1;
  }

  return failed_count;
}

int main(void) {
  int result = 0;

  result += s21_decimal_tests(s21_from_int_to_decimal_suite());
  result += s21_decimal_tests(s21_from_float_to_decimal_suite());
  result += s21_decimal_tests(s21_from_decimal_to_float_suite());
  result += s21_decimal_tests(s21_from_decimal_to_int_suite());
  result += s21_decimal_tests(s21_is_less_or_equal_suite());
  result += s21_decimal_tests(s21_is_greater_suite());
  result += s21_decimal_tests(s21_is_greater_or_equal_suite());
  result += s21_decimal_tests(s21_is_not_equal_suite());
  result += s21_decimal_tests(s21_is_equal_suite());
  result += s21_decimal_tests(s21_is_less_suite());
  result += s21_decimal_tests(s21_add_suite());
  result += s21_decimal_tests(s21_sub_suite());
  result += s21_decimal_tests(s21_mul_suite());
  result += s21_decimal_tests(s21_div_suite());
  result += s21_decimal_tests(s21_truncate_suite());
  result += s21_decimal_tests(s21_negate_suite());
  result += s21_decimal_tests(s21_round_suite());
  result += s21_decimal_tests(s21_floor_suite());

  printf("Failed tests: %d\n", result);

  return 0;
}
