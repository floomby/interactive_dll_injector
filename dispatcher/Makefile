CC = x86_64-w64-mingw32-g++ -std=c++11 -static-libstdc++ -static-libgcc -g


SRCS := $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

FW_SRCS := $(wildcard ../framework/*.cpp)
FW_OBJS := $(notdir $(FW_SRCS:.cpp=.o))


dispatcher: $(OBJS) framework
	$(CC) -o dispatcher $(OBJS) $(FW_OBJS) -lws2_32


$(OBJS): $(SRCS)
	$(CC) -c $(SRCS)

framework: $(FW_SRCS)
	$(CC) -c $(FW_SRCS)

#modules:
#	

clean:
	rm $(OBJS) $(FW_OBJS) dispatcher.exe
