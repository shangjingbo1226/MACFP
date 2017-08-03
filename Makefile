export PYTHON = pypy
export CC  = gcc
export CXX = g++
export CFLAGS = -std=c++11 -Wall -O3 -msse2 -I..

BIN = ./bin/MACFP ./bin/TDP
.PHONY: clean all

all: ./bin $(BIN)

./bin/MACFP: src/methods/MACFP.cpp src/utils/*.h src/config/*.h
./bin/TDP: src/methods/TDP.cpp src/utils/*.h src/config/*.h

./bin:
	mkdir bin

export LDFLAGS= -lm -Wno-unused-result -Wno-sign-compare -Wno-unused-variable -Wno-parentheses -Wno-format

$(BIN) :
	$(CXX) $(CFLAGS) $(LDFLAGS) -o $@ $(filter %.cpp %.o %.c, $^)
$(OBJ) :
	$(CXX) -c $(CFLAGS) -o $@ $(firstword $(filter %.cpp %.c, $^) )

clean :
	rm -rf bin tmp
