CFLAGS=-g -Wall -Wno-sign-compare -std=c++98
INCLUDE= -I./include
SRC=test/TestSuite.cpp 
OBJ=Libtest

.PHONY: test
test:
	${CXX} ${CFLAGS} ${SRC} ${INCLUDE} -o ${OBJ} && ./${OBJ}

.PHONY: leak-check
leak-check:
	# TODO: check for valgrind
	${CXX} ${CFLAGS} ${SRC} ${INCLUDE} -o ${OBJ} && valgrind --leak-check=full --show-leak-kinds=all ./${OBJ}

#.PHONY: benchmark
#benchmark: TODO

.PHONY: clean
clean:
	rm -f Libtest
