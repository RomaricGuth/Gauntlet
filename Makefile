OBJDIR = obj/
EXECDIR = bin/
SRCDIR = src/
EXECUTABLES = $(EXECDIR)test
OBJS = $(OBJDIR)main.o  $(OBJDIR)Manche.o $(OBJDIR)Manche1.o $(OBJDIR)Manche2.o $(OBJDIR)Manche3.o $(OBJDIR)Manche4.o $(OBJDIR)Manche5.o $(OBJDIR)BOSS.o $(OBJDIR)SDL_Jeu.o $(OBJDIR)Jeu.o $(OBJDIR)Personnage.o $(OBJDIR)Terrain.o $(OBJDIR)Position2D.o
MANCHES = $(SRCDIR)Manche1.h $(SRCDIR)Manche2.h $(SRCDIR)Manche3.h $(SRCDIR)Manche4.h $(SRCDIR)Manche5.h $(SRCDIR)BOSS.h
FLAGS = -g -ggdb -Wall
INCLUDE = -I./usr/include/SDL2
LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf

all: $(EXECUTABLES)


$(EXECDIR)test: $(OBJS)
	g++ $(FLAGS) $(OBJS) -o $(EXECDIR)test $(LIBS)

$(OBJDIR)main.o: $(SRCDIR)main.cpp  $(SRCDIR)SDL/SDL_Jeu.h
	g++ $(FLAGS) -c $(SRCDIR)main.cpp -o $(OBJDIR)main.o $(INCLUDE)


$(OBJDIR)SDL_Jeu.o: $(SRCDIR)SDL/SDL_Jeu.cpp $(MANCHES) $(SRCDIR)Jeu.h
	g++ $(FLAGS) -c $(SRCDIR)SDL/SDL_Jeu.cpp -o $(OBJDIR)SDL_Jeu.o $(INCLUDE)

$(OBJDIR)Manche1.o: $(SRCDIR)Manche1.cpp $(SRCDIR)Manche.h  
	g++ $(FLAGS) -c $(SRCDIR)Manche1.cpp -o $(OBJDIR)Manche1.o 

$(OBJDIR)Manche2.o: $(SRCDIR)Manche2.cpp $(SRCDIR)Manche.h  
	g++ $(FLAGS) -c $(SRCDIR)Manche2.cpp -o $(OBJDIR)Manche2.o

$(OBJDIR)Manche3.o: $(SRCDIR)Manche3.cpp $(SRCDIR)Manche.h $(SRCDIR)Position2D.h 
	g++ $(FLAGS) -c $(SRCDIR)Manche3.cpp -o $(OBJDIR)Manche3.o

$(OBJDIR)Manche4.o: $(SRCDIR)Manche4.cpp $(SRCDIR)Manche.h $(SRCDIR)Terrain.h
	g++ $(FLAGS) -c $(SRCDIR)Manche4.cpp -o $(OBJDIR)Manche4.o

$(OBJDIR)Manche5.o: $(SRCDIR)Manche5.cpp $(SRCDIR)Manche.h $(SRCDIR)Terrain.h
	g++ $(FLAGS) -c $(SRCDIR)Manche5.cpp -o $(OBJDIR)Manche5.o

$(OBJDIR)BOSS.o: $(SRCDIR)BOSS.cpp $(SRCDIR)Manche.h $(SRCDIR)Manche4.h $(SRCDIR)Manche5.h $(SRCDIR)Position2D.h $(SRCDIR)Terrain.h
	g++ $(FLAGS) -c $(SRCDIR)BOSS.cpp -o $(OBJDIR)BOSS.o

$(OBJDIR)Manche.o: $(SRCDIR)Manche.cpp  
	g++ $(FLAGS) -c $(SRCDIR)Manche.cpp -o $(OBJDIR)Manche.o 

$(OBJDIR)Jeu.o: $(SRCDIR)Jeu.cpp $(SRCDIR)Personnage.h $(SRCDIR)Terrain.h $(SRCDIR)Position2D.h
	g++ $(FLAGS) -c $(SRCDIR)Jeu.cpp -o $(OBJDIR)Jeu.o 

$(OBJDIR)Personnage.o: $(SRCDIR)Personnage.cpp $(SRCDIR)Position2D.h $(SRCDIR)Terrain.h
	g++ $(FLAGS) -c $(SRCDIR)Personnage.cpp -o $(OBJDIR)Personnage.o 

$(OBJDIR)Position2D.o: $(SRCDIR)Position2D.cpp $(SRCDIR)Terrain.h
	g++ $(FLAGS) -c $(SRCDIR)Position2D.cpp -o $(OBJDIR)Position2D.o 

$(OBJDIR)Terrain.o: $(SRCDIR)Terrain.cpp 
	g++ $(FLAGS) -c $(SRCDIR)Terrain.cpp -o $(OBJDIR)Terrain.o 





test: $(EXECDIR)testReg $(EXECDIR)testManches

$(EXECDIR)testReg : $(OBJDIR)mainTestReg.o  $(OBJDIR)Jeu.o $(OBJDIR)Personnage.o $(OBJDIR)Terrain.o $(OBJDIR)Position2D.o  
	g++ $(FLAGS) $(OBJDIR)mainTestReg.o $(OBJDIR)Jeu.o $(OBJDIR)Personnage.o $(OBJDIR)Terrain.o $(OBJDIR)Position2D.o -o $(EXECDIR)testReg 

$(OBJDIR)mainTestReg.o: $(SRCDIR)mainTestReg.cpp $(SRCDIR)Jeu.h
	g++ $(FLAGS) -c $(SRCDIR)mainTestReg.cpp -o $(OBJDIR)mainTestReg.o

$(EXECDIR)testManches : $(OBJDIR)mainTestRegManches.o $(OBJDIR)Jeu.o $(OBJDIR)Personnage.o $(OBJDIR)Terrain.o $(OBJDIR)Position2D.o $(OBJDIR)Manche.o $(OBJDIR)Manche1.o $(OBJDIR)Manche2.o $(OBJDIR)Manche3.o $(OBJDIR)Manche4.o $(OBJDIR)Manche5.o $(OBJDIR)BOSS.o
	g++ $(FLAGS) $(OBJDIR)mainTestRegManches.o $(OBJDIR)Jeu.o $(OBJDIR)Personnage.o $(OBJDIR)Terrain.o $(OBJDIR)Position2D.o $(OBJDIR)Manche.o $(OBJDIR)Manche1.o $(OBJDIR)Manche2.o $(OBJDIR)Manche3.o $(OBJDIR)Manche4.o $(OBJDIR)Manche5.o $(OBJDIR)BOSS.o -o $(EXECDIR)testManches

$(OBJDIR)mainTestRegManches.o: $(SRCDIR)mainTestRegManches.cpp $(SRCDIR)Jeu.h $(MANCHES)
	g++ $(FLAGS) -c $(SRCDIR)mainTestRegManches.cpp -o $(OBJDIR)mainTestRegManches.o


clean:
	rm $(OBJDIR)*.o $(EXECDIR)*