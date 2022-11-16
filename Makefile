CXXFLAGS = -Wall -Wextra -Werror
test : main.cpp
	$(CXX) $(CXXFLAGS) main.cpp

main.cpp: vector.hpp map.hpp stack.hpp
