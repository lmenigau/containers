CXXFLAGS = -std=c++98 -Wall -Wextra -Werror -Wno-unused-parameter
test : main.cpp
	$(CXX) $(CXXFLAGS) main.cpp

main.cpp: vector.hpp map.hpp stack.hpp
