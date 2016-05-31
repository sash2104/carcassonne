CXX = g++
TARGET = carcassonne
CXXFLAGS = -Wall -std=c++11
SRCS = main.cpp tile.cpp tile_holder.cpp board.cpp
OBJS := $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS)

clean:
	rm -f $(TARGET) $(OBJS)
