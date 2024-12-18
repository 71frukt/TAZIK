FLAGS = -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs \
		-Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security \
		-Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo \
		-Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE

SRCS = $(wildcard src/*.cpp)
# OBJS = $(SRCS:.cpp=.o)
OBJS = $(SRCS:.cpp=.o)
INCLUDE = -I lib

# OBJDIR = bin
# $(OBJDIR)/%.o: $(SRCDIR)/%.cpp
#     $(CC) $(CXXFLAGS) $< -c -o $@

all: $(OBJS)	

%.o: %.cpp %.h
	g++ $(FLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -r tree/src/*.o