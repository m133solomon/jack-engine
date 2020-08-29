LDFLAGS = -lglfw3 -lGLEW -framework CoreVideo -framework OpenGL -framework IOKit -framework Cocoa -framework Carbon
MOREFLAGS = -fsanitize=address; ASAN_OPTIONS=detect_leaks=1
CXX = clang

PROG = play

SRCS := $(shell find 'src' -name '*.c')
SRCS += main.c
OBJS := $(addprefix ,$(SRCS:%.c=%.o))

all: $(PROG)

%.o: src/%.c src/%.h main.c
	$(CXX) -c $<

$(PROG): $(OBJS)
	$(CXX) -g $(OBJS) -o $(PROG) $(LDFLAGS) $(MOREFLAGS)

clean:
	rm play src/*.o
