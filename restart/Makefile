CC = x86_64-w64-mingw32-g++ -g
FLAGS = -std=c++11 -static-libstdc++ -static-libgcc


SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

THINGS_SRCS = $(wildcard things/*.cpp)
THINGS_OBJS = $(notdir $(THINGS_SRCS:.cpp=.o))

injector: $(OBJS) $(THINGS_OBJS)
	$(CC) $(FLAGS) -o injector $(OBJS) $(THINGS_OBJS)

$(OBJS): $(SRCS)
	$(CC) $(FLAGS) -c $(SRCS)

$(THINGS_OBJS): $(THINGS_SRCS)
	$(CC) $(FLAGS) -c $(THINGS_SRCS)

dll: dll/dll.cpp
	$(CC) -shared -o dll.dll dll/dll.cpp

clean:
	rm dll.dll *.o injector.exe
