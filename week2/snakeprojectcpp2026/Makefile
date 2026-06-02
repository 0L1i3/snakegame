# 엔커시스 뱀 게임을 빌드하기 위한 컴파일러와 링커 설정이다.
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic
LDFLAGS = -lncurses
TARGET = snake_game

# 모든 구현 파일을 나열해 각 Manager 객체가 함께 링크되도록 한다.
SOURCES = main.cpp game.cpp map.cpp snake.cpp item.cpp portal.cpp pacman.cpp score.cpp mission.cpp stage.cpp
OBJECTS = $(SOURCES:.cpp=.o)

# 기본 빌드 대상이다.
all: $(TARGET)

# 컴파일된 object 파일들을 실행 파일로 링크한다.
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# 각 .cpp 파일을 대응되는 .o 파일로 컴파일한다.
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

# 빌드로 생성된 파일들을 삭제한다.
clean:
	rm -f $(OBJECTS) $(TARGET)
