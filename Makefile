#files to compile as part of the project
OBJS = game.cpp
PAC = Pac.cpp
Gho = Ghost.cpp
Tex = LTexture.cpp
OBJ_NAME = t
node = node.cpp

all: $(OBJS)
	g++ $(OBJS) $(PAC) $(Gho) $(node) $(Tex) -w -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -o $(OBJ_NAME)
