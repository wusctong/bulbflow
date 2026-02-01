CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra
TARGET_NCURSES = bulbflow
SRC_NCURSES = bulbflow.cpp

all: $(TARGET_NCURSES)

$(TARGET_NCURSES): $(SRC_NCURSES)
	$(CXX) $(CXXFLAGS) $(SRC_NCURSES) -o $(TARGET_NCURSES) -lncurses

clean:
	rm -f $(TARGET_NCURSES)

run: $(TARGET_NCURSES)
	./$(TARGET_NCURSES)

.PHONY: all clean run

