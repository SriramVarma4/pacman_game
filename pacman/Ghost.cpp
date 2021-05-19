#include "Ghost.h"
#include <time.h>
#include <string>
using namespace std;

Ghost::Ghost(vector<SDL_Rect> blocks, vector<node> Nodes, int ID){
    //Initialize the offsets
    id = ID;
    nodes= Nodes;
	nodes_copy = Nodes;
	cameback = false;
	Red_blocks = blocks;
    switch (id)
    {//{{20, 1020}, {1020, 20}, {1860,20}, {1860, 1020}, {1020,1020}, {20, 20}};
    case 0: mPosX = 1860; mPosY = 1020; count = 0; break;
    case 1: mPosX = 1020; mPosY = 20; count = 300; break;
    case 2: mPosX = 20; mPosY = 1020; count = 600; break;
    case 4: mPosX = 1860; mPosY = 20; count = 900; break;
    default:
        mPosY = 1860; mPosX =1020; count = 0; break;
    }
	PacBox.x=mPosX;
	PacBox.y=mPosY;
	PacBox.w=40;
	PacBox.h=40;
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

int Ghost::on_cross(){
    int n = nodes.size();
    for (int i=0; i<n; i++){
        if(mPosX == nodes[i].x && mPosY == nodes[i].y){
            return i;
        }
    }
    return -1;
}

void Ghost::move(int pacx, int pacy)
{   
    int vdiff;
    int hdiff;
    count++;
    if(count == 1000 && state == 1) { count=0; state = -1; }
    else if(count == freetime && state == -1){count=0; state = 1;}//alternates between chase, pseudorandom
    switch (id){
    case 0:
        if(state == -1){pacx = 20; pacy=20;}
        vdiff = mPosY - pacy;
        hdiff = mPosX - pacx;
        break;
    case 1:
        if(state == -1){pacx = 1860; pacy=20;}
        vdiff = mPosY - pacy;
        hdiff = mPosX - pacx - 80;
        break;
    case 2:
        if(state == -1){pacx = 1020; pacy=20;}
        vdiff = mPosY - pacy-80;
        hdiff = mPosX - pacx;
        break;
    case 3:
        if(state == -1){pacx = 20; pacy=1020;}
        vdiff = mPosY - pacy;
        hdiff = mPosX - pacx+80;
        break;
    default:
        if(state == -1){pacx = 20; pacy=20;}
        vdiff = mPosY - pacy;
        hdiff = mPosX - pacx;
    }
	
	
    int posi = on_cross(); 
    node current_node(0,0,"",-1);
    if(posi>=0){
        current_node = nodes[posi];
    }
    int ways = current_node.ways;
    string path = current_node.paths;
    if(ways == 1){ 
        mVelX =0; mVelY=0;
        if(path[0] == '1'){mVelX = -5;}
        else if(path[1] == '1'){mVelY = -5;}
        else if(path[2] == '1'){mVelX = 5;}
        else if(path[3] == '1'){mVelY = 5;}
	    
        prevx = mPosX; prevy = mPosY;
        if(state == 1){
            cameback = true;
        }
    }
    else if(ways!=-1){//chasing
        if(cameback){//block the single path
            string temp_path = nodes[posi].paths;
            if(mVelX>0){temp_path = "0" + temp_path.substr(1,3);}//block left
            else if(mVelX<0){temp_path = temp_path.substr(0,2) + "0" + temp_path.substr(3,1);}//block rit
            else if(mVelY>0){temp_path = temp_path.substr(0,1) + "0" + temp_path.substr(2,2);}//block top
            else if(mVelY<0){temp_path = temp_path.substr(0,3) + "0";}//block bottom
            nodes[posi].paths = temp_path;
            nodes[posi].ways = ways - 1;
            cameback = false;
        }
        mVelX =0; mVelY=0;
	    if(abs(vdiff)>abs(hdiff)){
            if(vdiff>0){
                if(path[1] == '1' && prevy>=mPosY){mVelY = -5;}
                else if(path[2] == '1' && prevx<=mPosX){ mVelX = 5;}//move right
                else if(path[0] == '1' && prevx>=mPosX){ mVelX = -5;}
                else if(path[3] == '1' && prevy<=mPosY){mVelY= 5;}//move down
            }else{
                if(path[3] == '1' && prevy<=mPosY){mVelY = 5; }
                else if(path[0] == '1' && prevx>=mPosX){ mVelX = -5; }//move left
                else if(path[2] == '1' && prevx<=mPosX){ mVelX = 5; }
                else if(path[1] == '1' && prevy>=mPosY){mVelY= -5; }//move up
            }
        }else{
            if(hdiff>0) {
                //cout<<( path)<<'\n';
                if(path[0]== '1' && prevx>=mPosX){ mVelX=-5; }
                else if(path[1] == '1' && prevy>=mPosY){ mVelY=-5;  }
                else if(path[3] == '1' && prevy<=mPosY){ mVelY=5; }
                else if(path[2] == '1' && prevx<=mPosX){ mVelX=5; }
            }
            else {
                if(path[2]=='1' && prevx<=mPosX){mVelX=5; }
                else if(path[3] == '1' && prevy<=mPosY){ mVelY=5; }
                else if(path[1] == '1' && prevy>=mPosY){ mVelY=-5; }
                else if(path[0] == '1' && prevx>=mPosX){ mVelX=-5; }
            }
        }
        //if(count == 1000){ state = -1; count=0;}
        prevx = mPosX; prevy = mPosY;
    }else if(state == 2 && ways!=-1){//runaway
        mVelX =0; mVelY=0;
	    if(abs(vdiff)>abs(hdiff)){
            if(vdiff>0){
                if(path[3] == '1' && prevy <= mPosY){mVelY = 5;}
                else if(path[2] == '1' && prevx <= mPosX){ mVelX = 5;}//move right
                else if(path[0] == '1' && prevx >= mPosX){ mVelX = -5;}
                else if(path[1] == '1' && prevy>= mPosY){mVelY= -5;}//move down
            }else{
                if(path[1] == '1' && prevy >= mPosY){mVelY = -5; }
                else if(path[0] == '1' && prevx >= mPosX){ mVelX = -5; }
                else if(path[2] == '1' && prevx <= mPosX){ mVelX = 5; }
                else if(path[3] == '1' && prevy <= mPosY){ mVelY= 5; }
            }
        }else{
            if(hdiff>0) {
                //cout<<( path)<<'\n';
                if(path[2]== '1' && prevx<=mPosX){ mVelX=5; }
                else if(path[1] == '1' && prevy>=mPosY){ mVelY=-5;  }
                else if(path[3] == '1' && prevy<=mPosY){ mVelY=5; }
                else if(path[0] == '1' && prevx>=mPosX){ mVelX=-5; }
            }
            else {
                if(path[0]=='1' && prevx>=mPosX){mVelX=-5; }
                else if(path[1] == '1' && prevy>=mPosY){ mVelY=-5; }
                else if(path[3] == '1' && prevy<=mPosY){ mVelY=5; }
                else if(path[2] == '1' && prevx<=mPosX){ mVelX=5; }
            }
        }
        prevx = mPosX; prevy = mPosY;
    }else if(ways == -1 && mVelX==0 && mVelY==0){
	    mVelX=-5; mVelY=0;
    }
    //Move the ghost left or right
action:

    mPosX += mVelX; mPosY += mVelY; 
    
    PacBox.x = mPosX;
    
    PacBox.y =mPosY;
}

int Ghost::getPosX(){ return mPosX; }

int Ghost::getPosY(){ return mPosY; }
