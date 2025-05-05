CXXFLAGS = -g3 -w -I/opt/homebrew/include

OBJS = util.o game.o board.o

LIBS = -framework OpenGL -framework GLUT -lGLEW -lfreeimage -L/opt/homebrew/lib

TARGET = game

$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all: $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

