#include <stdio.h>

#define S21INF (1.0 / 0.0)
#define S21NAN (0.0 / 0.0)

typedef struct {
  int bits[4];
} s21_decimal;

int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

void dec_to_zero(s21_decimal *number);
void get_ready_float_str(int lenth, char *strFloat);
void round_float_str(char *str);
int count_digits_after_point(char *strFloat);

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int add_bit_process(s21_decimal dval1, s21_decimal dval2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
void sub_bit_process(s21_decimal dval1, s21_decimal dval2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
void div_bit_process(s21_decimal value_1, s21_decimal value_2,
                     s21_decimal *result, s21_decimal *tmp);
void get_same_scale(s21_decimal *dval1, s21_decimal *dval2);
void get_same_scale_process(s21_decimal *dval1, s21_decimal *dval2, int scale1,
                            int scale2);
void bank_round(s21_decimal *dec);

int s21_is_greater_or_equal(s21_decimal dval1, s21_decimal dval2);
int is_greater_or_equal_bit_process(s21_decimal dval1, s21_decimal dval2);
int s21_is_greater(s21_decimal dval1, s21_decimal dval2);
int s21_is_greater_bit_process(s21_decimal dval1, s21_decimal dval2);
int s21_is_not_equal(s21_decimal dval1, s21_decimal dval2);
int is_not_equal_bit_process(s21_decimal dval1, s21_decimal dval2);
int s21_is_equal(s21_decimal dval1, s21_decimal dval2);
int is_equal_bit_process(s21_decimal dval1, s21_decimal dval2);
int s21_is_less_or_equal(s21_decimal dval1, s21_decimal dval2);
int is_less_or_equal_bit_process(s21_decimal dval1, s21_decimal dval2);
int s21_is_less(s21_decimal dval1, s21_decimal dval2);
int is_less_bit_process(s21_decimal dval1, s21_decimal dval2);

int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);

int getbit(s21_decimal number, int bit);
void setbit(s21_decimal *value, const int position);
void unsetbit(s21_decimal *value, const int position);
int getsign(s21_decimal number);
void setsign(s21_decimal *number, int sign);
int getscale(s21_decimal number);
void setscale(s21_decimal *number, int scale);
int shift_left(s21_decimal *number);
int pow2(int i);
int s21_strlen(const char *str);

// void print_decimal(s21_decimal num);
// void print_float(float num);