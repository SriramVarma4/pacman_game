#include "Ghost.h"

using namespace std;

Ghost::Ghost(vector<SDL_Rect> blocks, vector<node> nodes, int ID){
    //Initialize the offsets
    id = ID;
    cross = nodes;
	Red_blocks = blocks;
    switch (id)
    {//{{20, 1020}, {1020, 20}, {1860,20}, {1860, 1020}, {1020,1020}, {20, 20}};
    case 0: mPosX = 1860; mPosY = 1020; break;
    case 1: mPosX = 20; mPosY = 1020; break;
    case 2: mPosX = 1020; mPosY = 1020; break;
    case 3: mPosX = 1020; mPosY = 20; break;
    case 4: mPosX = 1860; mPosY = 460; break;
    case 5: mPosX = 1860; mPosY = 20; break;
    
    default:
        mPosY = 1020; mPosX =20; break;
    }
	PacBox.x=mPosX;
	PacBox.y=mPosY;
	PacBox.w=40;
	PacBox.h=40;
    count = 0;
    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;
    state = 1;
}

bool Ghost::collision(void){
	int n = Red_blocks.size();
	for (int i=0; i<n; i++){
	    if(SDL_HasIntersection(&PacBox, &Red_blocks[i])){return true;}
	}
	//cout<<"nocollision\n";
	return false;
}

node Ghost::on_cross(){
    int n = cross.size();
    for (int i=0; i<n; i++){
        if(PacBox.x == cross[i].x, PacBox.y == cross[i].y){
            return cross[i];
        }
    }
    vector<bool> dummy;
    node temp(0,0, dummy,-1);
}

void Ghost::move(int pacx, int pacy)
{   
    //move towards the pacman
    //count++;
    /*if(count == 0){
        state = 1; //chasing
    }else if(count == 2000){
        state = -2;
        count = -100000;
    }*/
    int vdiff = mPosY - pacy;
    int hdiff = mPosX - pacx;
    //node posi = on_cross();
    //int ways = posi.ways;
    //if(ways == 1){ state = 2; mVelX = -mVelX; mVelY= -mVelY; }
    if(state == 1){//chasing
	    if(abs(vdiff)>abs(hdiff)){
            if(vdiff<0){mVelY = 5; goto action;}
            else{
                mVelY = -5;
                goto action;
            }
        }else{
            if(hdiff>0) {mVelX = -5; goto action;}
            else {mVelX = 5; goto action;}
        }
        if(count == 1000){ state = -1; count=0;}
    }else if(state == -1){//going to corner
        int ranx = ran[id][0];
        int rany = ran[id][1];
        int rvdif = mPosY - rany;
        int rhdif = mPosX - ranx;
        if(abs(vdiff)>abs(hdiff)){
            if(rvdif<0){mVelY = 5; goto action;}
            else{
                mVelY = -5;
                goto action;
            }
        }else{
            if(rhdif>0) {mVelX = -5; goto action;}
            else {mVelX = 5; goto action;}
        }
        if (count == 300){state = 1; count=0; id++;}
    }
    else if( state == -2){//runaway
        if(abs(vdiff) > abs(hdiff)){
            if(vdiff>0){mVelY = 5;}
            else{mVelY = -5;}
        }else{
            if(hdiff<0){mVelX = -5;}
            else{mVelX = 5;}
        }
    }
    //Move the PAC left or right
action:
    mPosX += mVelX;
	PacBox.x = mPosX;
    //top_blk = false; rit = false; bot_blk = false; lef_blk=false;
    //If the PAC cant move horizontally
    if( collision())
    {
        mPosX -= mVelX;
        mVelX=0;
        PacBox.x = mPosX;
    }
    
    //Move the PAC up or down
    mPosY += mVelY;
	PacBox.y =mPosY;
    //If the PAC cant move vertically
    if( collision() )
    {
        mPosY -= mVelY;
        mVelY=0;
		PacBox.y = mPosY;
    }
    if(mVelX==0 && mVelY ==0 && count%50 == 0){
        count = 0;
        state = -state; //going to corner
        id++;
        //printf("%i,%i, %i\n",id, ran[id][0],ran[id][1]);
    }
    if(id == 6){
        id =0;
    }
}

int Ghost::getPosX(){ return mPosX; }

int Ghost::getPosY(){ return mPosY; }