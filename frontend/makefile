FLAGS = -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs \
		-Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security \
		-Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo \
		-Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE

SOURCES = $(wildcard src/*.cpp) $(wildcard ../tree/src*.cpp)

OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = main
INCLUDE = -I lib

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	g++ $(FLAGS) $^ -o $@

%.o : %.cpp
	g++ -c $(FLAGS) $(INCLUDE) $< -o $@

clean:
	rm -rf *.exe
	rm -rf *.o
