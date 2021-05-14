#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <cmath>
#include <stdlib.h>
using namespace std;
#ifndef PAC_H
#define PAC_H

class Pac
{
    public:
		//The dimensions of the pac
		static const int PAC_WIDTH = 40;
		static const int PAC_HEIGHT = 40;
		vector<SDL_Rect> Red_blocks;
		//Maximum axis velocity of the pac
		static const int PAC_VEL = 5;

		//Initializes the variables
		Pac(vector<SDL_Rect> blocks,int ID);
		//detects collision with the blocks
		bool collision();

		//Takes key presses and adjusts the pac's velocity
		void handleEvent( SDL_Event& e );

		//Moves the pac
		void move();

		//Shows the pac on the screen relative to the camera
		void render( int camX, int camY );

		//Position accessors
		int getPosX();
		int getPosY();
        SDL_Rect PacBox;
    private:
		//The X and Y offsets of the pac
		int id;
		int mPosX, mPosY;
		int keyState;
        bool keychanged;
		//The velocity of the pac
		int mVelX, mVelY;
};

#endif