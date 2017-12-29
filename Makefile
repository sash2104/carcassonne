CXX = g++
TARGET = carcassonne
TEST = test
CXXFLAGS = -Wall -std=c++11
SRCS = score_sheet.cpp tile_factory.cpp game_context.cpp region.cpp segment.cpp tile_position_map.cpp tile.cpp board.cpp game.cpp player.cpp
MAIN_SRCS = main.cpp $(SRCS)
TEST_SRCS = unit_tests.cpp test_util.cpp $(SRCS)
OBJS := $(MAIN_SRCS:.cpp=.o)
TEST_OBJS := $(TEST_SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS)

$(TEST): $(TEST_OBJS)
	$(CXX) -o $@ $(TEST_OBJS)
	./$@

clean:
	rm -f $(TARGET) $(TEST) $(OBJS) $(TEST_OBJS)
