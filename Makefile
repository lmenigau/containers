CXX		= clang++
#SFLAGS = -fsanitize=address
CXXFLAGS := $(SFLAGS) -g -std=c++98 -Wall -Wextra -Werror -Wno-unused-parameter
STD_SRC = \
clear.cpp\
insert_fill.cpp\
insert_single.cpp\
map.cpp\
main.cpp

FT_SRC := $(STD_SRC)

OBJ_STD := $(addprefix std_, $(STD_SRC:.cpp=.o))
OBJ_FT := $(addprefix ft_, $(FT_SRC:.cpp=.o))

.PHONY: test

diff : std.out ft.out
	diff std.out ft.out > $@ | exit 0

DEPS = $(FT_SRC:.cpp=.d)
%.d: %.cpp
	@set -e; rm -f $@; \
		$(CXX) -MM $(CXXFLAGS) $< > $@.$$$$; \
		sed 's,\($*\)\.o[ :]*,ft_\1.o std_\1.o $@ : ,g' < $@.$$$$ > $@; \
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

ft: $(OBJ_FT)
	$(CXX) $(CXXFLAGS) $(OBJ_FT) -o $@
std: CXXFLAGS += -D IS_STD=true
std: $(OBJ_STD)
	$(CXX) $(CXXFLAGS) $(OBJ_STD) -o $@

.PHONY: re

clean:
	$(RM) *.d *.o std.out ft.out std ft

re	: clean diff
