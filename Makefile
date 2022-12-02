CXX		= c++
CXXFLAGS = -g -std=c++98 -Wall -Wextra -Werror -Wno-unused-parameter
test : main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o test

main.cpp: vector.hpp map.hpp stack.hpp

.PHONY: re

clean : 
	$(RM) test

re	: clean test
