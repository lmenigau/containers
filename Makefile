CXX		= c++
SANFLAGS = -fsanitize=address
CXXFLAGS := $(SANFLAGS) -g -std=c++98 -Wall -Wextra -Werror -Wno-unused-parameter
SRC = main.cpp
.PHONY: test
OBJ = $(SRC:.c=.o)

test.out : std.out ft.out
	diff std.out ft.out > test.out | exit 0

DEPS = $(SRC:.cpp=.d)
%.d: %.cpp
	@set -e; rm -f $@; \
		$(CXX) -MM $(CXXFLAGS) $< > $@.$$$$; \
		sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
		rm -f $@.$$$$

include $(DEPS)


std.out: std
	./std > std.out

ft.out: ft
	./ft > ft.out

std : $(OBJ)
	$(CXX) $(CXXFLAGS) -D NS=std $(OBJ) -o std

ft : $(OBJ)
	$(CXX) $(CXXFLAGS) -D NS=ft $(OBJ) -o ft


.PHONY: re

clean : 
	$(RM) std ft std.out ft.out

re	: clean test
