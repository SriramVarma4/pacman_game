#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <cmath>
#include <stdlib.h>
using namespace std;
#ifndef GHOST_H
#define GHOST_H

class Ghost
{
    public:
		//The dimensions of the pac
		static const int PAC_WIDTH = 40;
		static const int PAC_HEIGHT = 40;
		vector<SDL_Rect> Red_blocks;
		//Maximum axis velocity of the pac
		int PAC_VEL = 5;

		//Initializes the variables
		Ghost(vector<SDL_Rect> blocks);
		//detects collision with the blocks
		bool collision();

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
		int ran[6][2] = {{20, 1020}, {1020, 20}, {1860,20}, {1860, 1020}, {1020,1020}, {20, 20}};// corners
        	//state of the ghost -1 = stuck 
        	int state; //0- chase 1-random but active 0 = run away vulnerable
        	//count for changing the state
        	int count;
		//The velocity of the pac
		int mVelX, mVelY;
};

#endif
