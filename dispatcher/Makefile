CC = x86_64-w64-mingw32-g++ -std=c++11 -static-libstdc++ -static-libgcc -g


SRCS := $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)


dispatcher: $(OBJS)
	$(CC) -o dispatcher $(OBJS) -lws2_32


$(OBJS): $(SRCS)
	$(CC) -c $(SRCS)

clean:
	rm $(OBJS) dispatcher.exe
