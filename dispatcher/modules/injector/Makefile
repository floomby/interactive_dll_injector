CC = x86_64-w64-mingw32-g++ -g


SRCS := $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)


injector: $(OBJS)
	$(CC) -shared -o injector.dll $(OBJS)


$(OBJS): $(SRCS)
	$(CC) -c $(SRCS)


clean:
	rm *.o *.dll
