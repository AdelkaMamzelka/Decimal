K?=autoreport
CC=gcc  -Wall -Wextra -Werror -std=c11
#CFLAGS= -Wall -Wextra -Werror -std=c11
UBUNTU_FLAGS = -lsubunit -lrt -lpthread -lm
SRC = decimal.c supporting_functions.c 
OS = $(shell uname)
ifeq ($(OS), Darwin)
	det_OS = -lcheck
else
	det_OS = -lcheck -lsubunit -lrt -lpthread -lm 
endif

all:  clean decimal.a test gcov_report

decimal.a:
	$(CC)  s21*.c -c
	ar rcs decimal.a *.o
	ranlib decimal.a

gcov_report:
	rm -rf *_tests.gcno *_tests.gcda test-supporting_functions.gc*
	lcov -t "result" -o res.info -c -d . --rc branch_coverage=0
	genhtml -o report res.info --rc branch_coverage=0
	open report/index.html

test:
	$(CC)   --coverage $(SRC) tests/*.c -o test $(det_OS)
	./test

commit:
	git add s21*.c decimal.h tests/* 
	git commit -m '$(K)'    
	git push origin develop

clean:
	rm -rf report *.gc* *.o *.info *.a test.dSYM test