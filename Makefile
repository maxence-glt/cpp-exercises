CXX=clang++
CPP_FLAGS = -std=c++23 -I /usr/local/include

build:
	$(CXX) -O3 $(CPP_FLAGS) -lpthread main.cpp
debug:
	$(CXX) -O0 -g $(CPP_FLAGS) \
		-fno-omit-frame-pointer -fno-optimize-sibling-calls \
		-std=c++23 -pthread -fsanitize=address,undefined,leak \
		main.cpp
run:
	./a.out
clean: 
	rm a.out
	rm -rf a.out.dSYM/
