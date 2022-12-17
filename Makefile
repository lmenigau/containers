CXX		= c++
SFLAGS = -fsanitize=address
CXXFLAGS := $(SFLAGS) -g -std=c++98 -Wall -Wextra -Werror -Wno-unused-parameter
SRC = clear.cpp main.cpp
OBJ_FT := $(addprefix ft_, $(SRC:.cpp=.o))
OBJ_STD := $(addprefix std_, $(SRC:.cpp=.o))

.PHONY: test

diff : std.out ft.out
	diff std.out ft.out > $@ | exit 0

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

$(OBJ_FT): ft_%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJ_STD): std_%.o: %.cpp 
	$(CXX) -c $(CXXFLAGS) $< -o $@

ft: CXXFLAGS += -D NS=ft
ft: $(OBJ_FT)
	$(CXX) $(CXXFLAGS) $(OBJ_FT) -o $@
std: CXXFLAGS += -D NS=std
std: $(OBJ_STD)
	$(CXX) $(CXXFLAGS) $(OBJ_STD) -o $@

.PHONY: re

clean:
	$(RM) std ft std.out ft.out *.o

re	: clean diff
