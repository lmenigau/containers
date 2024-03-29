$(if $(filter re as, $(MAKECMDGOALS)),\
	$(shell $(RM) arte/*.o ft std))

CXX		= clang++
CXXFLAGS := -g -std=c++98 -Wall -Wextra -Werror -Wno-unused-parameter
STD_SRC = clear.cpp\
					insert_fill.cpp\
					insert_single.cpp\
					main.cpp

FT_SRC := $(STD_SRC)

OBJ_STD := $(addprefix arte/std_, $(STD_SRC:.cpp=.o))
OBJ_FT := $(addprefix arte/ft_, $(FT_SRC:.cpp=.o))

DEPS := $(addprefix arte/, $(FT_SRC:.cpp=.d))

ARTE := $(OBJ_FT) $(OBJ_STD) $(DEPS)

.PHONY: all

all : ft std

$(ARTE): | arte

arte:
	mkdir -p arte

arte/%.d: %.cpp
	@set -e; rm -f $@; \
		$(CXX) -M $(CPPFLAGS) $< > $@.$$$$; \
		sed 's,\($*\)\.o[ :]*, arte/ft_\1.o arte/std_\1.o $@ : ,g' < $@.$$$$ > $@; \
		rm -f $@.$$$$

include $(DEPS)

arte/std.out: std
	./std > arte/std.out

arte/ft.out: ft
	./ft > arte/ft.out

$(OBJ_FT): arte/ft_%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(OBJ_STD): arte/std_%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

ft: $(OBJ_FT)
	$(CXX) $(CXXFLAGS) $(ASANFLAGS) $(OBJ_FT) -o $@

std: CPPFLAGS += -D IS_STD=true
std: $(OBJ_STD)
	$(CXX) $(CXXFLAGS) $(ASANFLAGS) $(OBJ_STD) -o $@

.PHONY: as clean re

clean:
	rm -rf ft std $(wildcard arte/*.o)

re: all

as: ASANFLAGS += -fsanitize=address
as: CXXFLAGS := $(ASANFLAGS) $(CXXFLAGS)
as: all
