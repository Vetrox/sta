.PHONY: all

CF=-std=c++2a -O0 -g -fsanitize=address,undefined,leak -D_GLIBCXX_DEBUG -Wall -Wextra -pedantic -Wshadow -Wsign-conversion -Wconversion -Wunreachable-code -Wdisabled-optimization# -Werror
SRCcpp=$(shell find src/ -name '*.cpp')
SRCh=$(shell find src/ -name '*.h')
OBJS=$(SRCcpp:.cpp=.o)
PCH=$(SRCh:.h=.h.gch) # precompiled headers .gch

all: dirs build/a.out
	build/a.out

re: clean all

dirs:
	mkdir -p build

clean-objs:
	rm -f $(OBJS)

clean-pch:
	rm -f $(PCH)

clean: clean-objs clean-pch
	rm -rf build

build/a.out: $(OBJS) $(PCH)
	g++ $(OBJS) -o build/a.out $(CF)

src/%.h.gch: src/%.h
	g++ -x c++-header -c $< -o $@ $(CF)

src/%.o: src/%.cpp
	g++ -c $< -o $@ $(CF)
