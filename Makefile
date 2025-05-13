CXXFLAGS = -g3 -w -I/opt/homebrew/include

OBJS = util.o game.o gamecontroller.o player.o car.o npc.o passenger.o package.o fuelstation.o rolechangestation.o obstacle.o board.o leaderboard.o

LIBS = -framework OpenGL -framework GLUT -lGLEW -lfreeimage -L/opt/homebrew/lib

TARGET = game

$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all: $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

