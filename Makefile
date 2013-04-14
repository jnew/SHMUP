objects := main.o Player.o Projectile.o VisibleObject.o Game.o Background.o Enemy.o
libs := -lsfml-graphics -lsfml-window -lsfml-system 

all: Game

Game: $(objects)
	g++ -o $@ $^ $(libs)

%.o: %.cpp
	g++ -c -MMD -o $@ $<

-include $(objects:.o=.d)

clean:
	rm -f *.o *.d
