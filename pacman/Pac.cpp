#include "Pac.h"
using namespace std;

Pac::Pac(vector<SDL_Rect> blocks,int ID){
    //Initialize the offsets
    id = ID; 
	Red_blocks = blocks;
    mPosX = 20;
    mPosY = 20;
	PacBox.x=20;
	PacBox.y=20;
	PacBox.w=40;
	PacBox.h=40;
	keyState = 0;
    keychanged = false;
    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;
}

bool Pac::collision(void){
	int n = Red_blocks.size();
	for (int i=0; i<n; i++){
	    if(SDL_HasIntersection(&PacBox, &Red_blocks[i])){return true;}
	}
	//cout<<"nocollision\n";
	return false;
}

void Pac::handleEvent( SDL_Event& e )
{
    //If a key was pressed
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        //keychanged = true;
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: keyState = 1; break;
            case SDLK_DOWN: keyState = 2; break;
            case SDLK_LEFT: keyState = 3; break;
            case SDLK_RIGHT: keyState = 4; break;
        }
    }
    
}

void Pac::move()
{   
    
    if(keyState == 1){mVelY = -PAC_VEL;}
    else if(keyState == 2){mVelY = PAC_VEL;}
	else if(keyState == 3){ mVelX = -PAC_VEL;}
    else if(keyState == 4){mVelX = PAC_VEL;}
    //Move the PAC left or right
    mPosX += mVelX;
	PacBox.x += mVelX;
	//cout<<keyState<<'\n';
    //If the PAC cant move horizontally
    if( collision())
    {
        //Move back
        mPosX -= mVelX;
		PacBox.x -=mVelX;
		mVelX = 0;
    }

    //Move the PAC up or down
    mPosY += mVelY;
	PacBox.y +=mVelY;
    //If the PAC cant move vertically
    if( collision() )
    {
        //Move back
        mPosY -= mVelY;
		PacBox.y -= mVelY;
		mVelY=0;
    }
}

int Pac::getPosX(){ return mPosX; }

int Pac::getPosY(){ return mPosY; }