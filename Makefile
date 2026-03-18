CXX=/opt/homebrew/opt/llvm/bin/clang++

run:
	$(CXX) -O3 -std=c++23 main.cpp
	#-Rpass=loop-vectorize -Rpass-missed=loop-vectorize
	./a.out
debug:
	$(CXX) -O0 -g -fno-omit-frame-pointer -fno-optimize-sibling-calls \
		-std=c++23 -fsanitize=address,undefined,leak \
		main.cpp
	./a.out
clean: 
	rm a.out
	rm -rf a.out.dSYM/
