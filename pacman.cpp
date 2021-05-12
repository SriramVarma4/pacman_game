#include "Pac1.h"
using namespace std;

Pac::Pac(vector<SDL_Rect> blocks){
    //Initialize the offsets
	Red_blocks = blocks;
    mPosX = 20;
    mPosY = 20;
	/*PacBox.x=20;
	PacBox.y=20;
	PacBox.w=35;
	PacBox.h=35;
    */
	keyState = 0;
    keychanged = false;
    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;
}


void Pac::resolve_player_key_down(int key, struct Player* player) {
    if (key == player->left_key) {
        player->left = true;
        keyState = 3;
    }
    if (key == player->right_key) {
        player->right = true;
        keyState = 4;
    }
    if (key == player->up_key) {
        player->up = true;
        keytState = 1;
    }
    if (key == player->down_key) {
        player->down = true;
        keyState = 2;
    }
}

void Pac::resolve_player_key_up(int key, struct Player* player) {
    if (key == player->left_key) {
        player->left = false;
        keyState = 3;
    }
    if (key == player->right_key) {
        player->right = false;
        keyState = 4;
    }
    if (key == player->up_key) {
        player->up = false;
        keytState = 1;
    }
    if (key == player->down_key) {
        player->down = false;
        keyState = 2;
    }
}

bool Pac::collision(void){
	int n = Red_blocks.size();
	for (int i=0; i<n; i++){
	    if(SDL_HasIntersection(&PacBox, &Red_blocks[i])){return true;}
	}
	//cout<<"nocollision\n";
	return false;
}

void Pac::handleEvent(SDL_Event e, struct Player* player) {
    if (e.type == SDL_KEYDOWN) {
        resolve_player_key_down(e.key.keysym.sym, player);
    } 
    if (e.type == SDL_KEYUP) {
        resolve_player_key_up(e.key.keysym.sym, player);
    }
}

void Pac::set_player_pos(struct Player* player, float x, float y) {
    player->PacBox.x = x;
    player->PacBox.y = y;
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