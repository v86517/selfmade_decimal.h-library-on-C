CC = gcc
OS = $(shell uname)
ifeq ($(OS), Linux)
	LIBS = -lcheck -pthread -lrt -lm -lsubunit
else
	LIBS = -lcheck
endif

CFLAGS = -c -Wall -Werror -Wextra -std=c11 $(LIBS)

all: s21_decimal.a

test: gcov_s21_decimal.a
	$(CC) $(CFLAGS) test.c
	$(CC) --coverage *.o s21_decimal.a $(LIBS) -o s21_decimal_test
	./s21_decimal_test

clean:
	rm -rf *.o *.info s21_*_test *.gcda *.gcno *.css *.html *.gcov *.a report

s21_decimal.a:
	$(CC) $(CFLAGS) s21_*.c
	ar rc s21_decimal.a s21_*.o
	ranlib s21_decimal.a

gcov_s21_decimal.a:
	rm -f s21_decimal.a
	$(CC) --coverage $(CFLAGS) s21_*.c
	ar rc s21_decimal.a s21_*.o
	ranlib s21_decimal.a

gcov_report: test
	lcov -c -d ./ -o s21_test.info --rc lcov_branch_coverage=1    
	genhtml -o ./report s21_test.info --rc lcov_branch_coverage=1

rebuild: clean all

style_fix:
	clang-format -i *.c *.h --style=Google

style:
	clang-format -n *.c *.h --style=Google

.PHONY: all test clean gcov_report rebuild style style_fix
