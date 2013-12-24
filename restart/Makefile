CC = x86_64-w64-mingw32-g++ -g
FLAGS = -std=c++11 -static-libstdc++ -static-libgcc


SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

THINGS_SRCS = $(wildcard things/*.cpp)
THINGS_OBJS = $(notdir $(THINGS_SRCS:.cpp=.o))

FW_SRCS = $(wildcard ../framework2/*.cpp)
FW_OBJS = $(notdir $(FW_SRCS:.cpp=.o))

injector: $(OBJS) $(THINGS_OBJS) framework
	$(CC) $(FLAGS) -o injector $(OBJS) $(THINGS_OBJS) $(FW_OBJS) -lws2_32

$(OBJS): $(SRCS)
	$(CC) $(FLAGS) -c $(SRCS)

$(THINGS_OBJS): $(THINGS_SRCS)
	$(CC) $(FLAGS) -c $(THINGS_SRCS)

framework: $(FW_SRCS)
	$(CC) $(FLAGS) -c $(FW_SRCS)

dll: dll/dll.cpp
	$(CC) -shared -o dll.dll dll/dll.cpp

clean:
	rm dll.dll *.o injector.exe
