objects := main.o classfiles/Player.o classfiles/Projectile.o classfiles/VisibleObject.o classfiles/Game.o classfiles/Background.o classfiles/Enemy.o classfiles/Scoreboard.o
libs := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

all: Game

Game: $(objects)
	g++ -o $@ $^ $(libs)

%.o: %.cpp
	g++ -c -MMD -o $@ $<

-include $(objects:.o=.d)

clean:
	rm -f *.o *.d
