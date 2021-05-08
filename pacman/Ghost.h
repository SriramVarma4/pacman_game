#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <cmath>
#include <stdlib.h>
#include "node.h"
using namespace std;
#ifndef GHOST_H
#define GHOST_H

class Ghost
{
    public:
		//The dimensions of the pac
		static const int PAC_WIDTH = 40;
		static const int PAC_HEIGHT = 40;
		vector<node> cross;
		vector<SDL_Rect> Red_blocks;
		//Maximum axis velocity of the pac
		int PAC_VEL = 5;

		//Initializes the variables
		Ghost(vector<SDL_Rect> red_blocks, vector<node> cros, int ID);
		
		bool collision();	//collision to prohibited area

		node on_cross();	//detects whether the ghost is on the instersection
		
		//Moves the ghost
		void move(int pacx, int pacy);

		//Shows the pac on the screen relative to the camera
		void render(  int camX, int camY );

		//Position accessors
		int getPosX();
		int getPosY();
        SDL_Rect PacBox;
    private:
		//The X and Y offsets of the pac
		int mPosX, mPosY;
		int id;
		int ran[6][2] = {{20, 1020}, {1020, 20}, {1860,20}, {1860, 1020}, {1020,1020}, {20, 20}};//four corners
        //state of the ghost 2 = stuck 
        int state; //0- chase 1-random but active -1 = run away vulnerable
        //count for changing the state
        int count;
		//The velocity of the pac
		int mVelX, mVelY;
};

#endif