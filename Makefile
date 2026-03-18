CXX=clang++
CPP_FLAGS = -std=c++23 -I /usr/local/include

run:
	$(CXX) -O3 $(CPP_FLAGS) main.cpp
	./a.out
debug:
	$(CXX) -O0 -g -fno-omit-frame-pointer -fno-optimize-sibling-calls \
		-std=c++23 -fsanitize=address,undefined,leak \
		main.cpp
	./a.out
clean: 
	rm a.out
	rm -rf a.out.dSYM/
