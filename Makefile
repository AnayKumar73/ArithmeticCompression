CXX=clang++
INCLUDES=-Iincludes/
CXX_FLAGS=-std=c++20 -gdwarf-4 -fstandalone-debug -O0 -Wall -Wextra -g -pedantic $(INCLUDES)
#-Werror

exec: bin/exec

bin/exec: src/main.cc src/probability_model.cc src/arithmetic_encoder.cc src/arithmetic_decoder.cc includes/probability_model.h includes/arithmetic_decoder.h includes/arithmetic_encoder.h
	${CXX} ${CXX_FLAGS} src/main.cc src/probability_model.cc src/arithmetic_decoder.cc src/arithmetic_encoder.cc -o bin/exec

.PHONY: clean exec

clean:
	rm -fr bin/* obj/*