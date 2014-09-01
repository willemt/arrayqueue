GCOV_OUTPUT = *.gcda *.gcno *.gcov 
GCOV_CCFLAGS = -fprofile-arcs -ftest-coverage
CC     = gcc
CCFLAGS = -Itests -I. -g -O2 -Wall -Werror -W -fno-omit-frame-pointer -fno-common -fsigned-char $(GCOV_CCFLAGS)


all: test

main.c:
	sh tests/make-tests.sh tests/test_*.c > main.c

test: main.c arrayqueue.o tests/test_arrayqueue.c tests/CuTest.c main.c
	$(CC) $(CCFLAGS) -o $@ $^
	./test
	gcov main.c test_arrayqueue.c arrayqueue.c

arrayqueue.o: arrayqueue.c
	$(CC) $(CCFLAGS) -c -o $@ $^

clean:
	rm -f main.c arrayqueue.o test $(GCOV_OUTPUT)
