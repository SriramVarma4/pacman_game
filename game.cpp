//COP TASK2

#include "LTexture.h"
#include "Pac.h"
#include "Ghost.h"
#include "node.h"
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <time.h>
#include <iostream>
using namespace std;

//screen dimension constants
const int SCREEN_WIDTH = 1900;

const int SCREEN_HEIGHT = 900;

//level dimension constants
const int LEVEL_WIDTH = 1920;
const int LEVEL_HEIGHT = 1080;
vector<SDL_Rect> red_blocks;

class rect_para{
public:
	int x, y, w, h;
	rect_para(int a, int b, int c, int d){
		x = a; y = b; w = c; h = d;
	}
};

int Rand_0_5(void){
	double i = ((double) rand() / RAND_MAX);
	if( i<0.2){
		return 2;
	}else if(i<0.4){
		return 2;
	}else if(i<0.6){
		return 3;
	}else if(i<0.8){
		return 4;
	}else{return 5;}
}

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image as texture
SDL_Texture* loadTexture( std::string path );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;
//sound effects
Mix_Chunk *gMunch_a = NULL;
Mix_Chunk *gMunch_b = NULL;
Mix_Chunk *gDead = NULL;
Mix_Chunk *gMenu = NULL;
Mix_Chunk *gKill = NULL;
//Scene textures Texture wrapper class
LTexture gPac_ritTexture;
LTexture gPac_leftTexture;
LTexture gPac_upTexture;
LTexture gPac_downTexture;
LTexture ggreenPac_ritTexture;
LTexture ggreenPac_leftTexture;
LTexture ggreenPac_upTexture;
LTexture ggreenPac_downTexture;
LTexture gbottom_strip;
LTexture gBGTexture;	//background
LTexture gGameover;
LTexture ggreenplayer;
LTexture gyellowplayer;
LTexture gScoreboard;
LTexture gInstructions;
LTexture gGh1Texture;
LTexture gspoint;//	for rendering points
LTexture gTextTexture;//for rendering score text
LTexture gbpoint;
LTexture gstart;


TTF_Font *gFont = NULL; //FONT 


bool LTexture::loadFromFile( std::string path ){
	//Get rid of preexisting texture
	free();
	//The final texture
	SDL_Texture* newTexture = NULL;
	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Convert surface to display format
		SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat( loadedSurface, SDL_GetWindowPixelFormat( gWindow ), 0 );
		if( formattedSurface == NULL )
		{
			printf( "Unable to convert loaded surface to display format! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Create blank streamable texture
			newTexture = SDL_CreateTexture( gRenderer, SDL_GetWindowPixelFormat( gWindow ), SDL_TEXTUREACCESS_STREAMING, formattedSurface->w, formattedSurface->h );
			if( newTexture == NULL )
			{
				printf( "Unable to create blank texture! SDL Error: %s\n", SDL_GetError() );
			}
			else
			{
				//Lock texture for manipulation
				SDL_LockTexture( newTexture, NULL, &mPixels, &mPitch );

				//Copy loaded/formatted surface pixels
				memcpy( mPixels, formattedSurface->pixels, formattedSurface->pitch * formattedSurface->h );

				//Unlock texture to update
				SDL_UnlockTexture( newTexture );
				mPixels = NULL;

				//Get image dimensions
				mWidth = formattedSurface->w;
				mHeight = formattedSurface->h;
			}
			//Get rid of old formatted surface
			SDL_FreeSurface( formattedSurface );
		}	
		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}
	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor ){
	//Get rid of preexisting texture
	free();
	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface == NULL )
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else{
			//Get image dimensions
			mWidth = 300;
			mHeight = 30;
		}
		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	//Return success
	return mTexture != NULL;
}

//renders player1
void Pac::render( int camX, int camY ){
    //Show the pac relative to the camera
	if(mVelX>0){gPac_ritTexture.render( mPosX - camX, mPosY - camY );}
	else if(mVelX<0){gPac_leftTexture.render( mPosX - camX, mPosY - camY );}
	else if(mVelY<0){gPac_upTexture.render( mPosX - camX, mPosY - camY );}
	else if(mVelY>0){gPac_downTexture.render( mPosX - camX, mPosY - camY );}
	else {gPac_ritTexture.render( mPosX - camX, mPosY - camY );}
}

//renders player2
void Pac::render1( int camX, int camY ){
    //Show the pac relative to the camera
	if(mVelX>0){ggreenPac_ritTexture.render( mPosX - camX, mPosY - camY );}
	else if(mVelX<0){ggreenPac_leftTexture.render( mPosX - camX, mPosY - camY );}
	else if(mVelY<0){ggreenPac_upTexture.render( mPosX - camX, mPosY - camY );}
	else if(mVelY>0){ggreenPac_downTexture.render( mPosX - camX, mPosY - camY );}
	else {ggreenPac_ritTexture.render( mPosX - camX, mPosY - camY );}
}

//renders ghosts
void Ghost::render( int camX, int camY ){
	gGh1Texture.render(mPosX - camX, mPosY - camY);
}

//renders died pac
void Pac::pactoghostrender( int camX, int camY ){
    //Show the pac relative to the camera
	if(mVelX>0){gGh1Texture.render( mPosX - camX, mPosY - camY );}
	else if(mVelX<0){gGh1Texture.render( mPosX - camX, mPosY - camY );}
	else if(mVelY<0){gGh1Texture.render( mPosX - camX, mPosY - camY );}
	else if(mVelY>0){gGh1Texture.render( mPosX - camX, mPosY - camY );}
	else {gGh1Texture.render( mPosX - camX, mPosY - camY );}
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}
		//Create window
		gWindow = SDL_CreateWindow( "PACMAN", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT+50, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
				//Initialize SDL_mixer
				if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
				{
					printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
					success = false;
				}
				//Initialize SDL_ttf
				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load pac texture
	if( !gPac_ritTexture.loadFromFile( "images/pac_rit.png" ) ){
		printf( "Failed to load pacman texture!\n" );
		success = false;
	}
	if( !gPac_leftTexture.loadFromFile( "images/pac_left.png" ) ){
		printf( "Failed to load pacman texture!\n" );
		success = false;
	}
	if( !gPac_upTexture.loadFromFile( "images/pac_up.png" ) ){
		printf( "Failed to load pacman texture!\n" );
		success = false;
	}
	if( !gPac_downTexture.loadFromFile( "images/pac_down.png" ) ){
		printf( "Failed to load pacman texture!\n" );
		success = false;
	}
	if( !ggreenPac_ritTexture.loadFromFile( "images/greenpac_rit.png" ) ){
		printf( "Failed to load pacman texture!\n" );
		success = false;
	}
	if( !ggreenPac_leftTexture.loadFromFile( "images/greenpac_left.png" ) ){
		printf( "Failed to load pacman texture!\n" );
		success = false;
	}
	if( !ggreenPac_upTexture.loadFromFile( "images/greenpac_up.png" ) ){
		printf( "Failed to load pacman texture!\n" );
		success = false;
	}
	if( !ggreenPac_downTexture.loadFromFile( "images/greenpac_down.png" ) ){
		printf( "Failed to load pacman texture!\n" );
		success = false;
	}
	if( !gbottom_strip.loadFromFile( "images/bottom_strip.png" ) ){
		printf( "Failed to load gameover texture!\n" );
		success = false;
	}
	//load ghost
	if( !gGh1Texture.loadFromFile( "images/ghost.png" ) )
	{
		printf( "Failed to load gh1 texture!\n" );
		success = false;
	}
	if (!gspoint.loadFromFile("images/smallpoint.png"))
	{
		printf("Failed to load small points\n");
		success = false;
	}
	if (!gbpoint.loadFromFile("images/bigpoint.png")){
		printf("Failed to load big point\n");
		success = false;
	}
	if (!gstart.loadFromFile("images/start.jpg")){
		printf("Failed to load start\n");
		success = false;
	}
	//Load background texture

	if( !gBGTexture.loadFromFile( "images/black.jpg" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	if( !ggreenplayer.loadFromFile( "images/greenplayer.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	if( !gyellowplayer.loadFromFile( "images/yellowplayer.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	if( !gScoreboard.loadFromFile( "images/scoreboard.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	if( !gInstructions.loadFromFile( "images/Instructions.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}
	else{
		//Lock texture
		if( !gBGTexture.lockTexture() ){ printf( "Unable to lock background' texture!\n" ); }
		//Manual color key
		else
		{
			//Allocate format from window
			Uint32 format = SDL_GetWindowPixelFormat( gWindow );
			SDL_PixelFormat* mappingFormat = SDL_AllocFormat( format );

			//make rectangles which have to be colored
			srand(time(0));
				vector<rect_para> rect_queue;
				//vector<SDL_Rect> red_blocks;
				int X= 60;int Y= 60; int H=0; int W=0;
				while(Y + 40<= 1060){		//creating the random rect on the first row
					H = Rand_0_5()*40;
					W = Rand_0_5()*40;
						
					if (Y + H + 40 >= 1060){//bottom edge
						H = 1060 - 40 - Y;
						if (Y == 1020){//bottom edge - pacsize
							Y = Y - 40;
							H = 40;
						}
					}
					SDL_Rect block = {X, Y, W, H};
					rect_para r(X,Y,W,H);
					rect_queue.push_back(r);
					red_blocks.push_back(block);

					Y = Y + H + 40;
				}
				int no_vertical_blocks = rect_queue.size();

				for (int i =0 ; i<no_vertical_blocks ; i++){	//creating random blocks on each column
					Y = rect_queue[i].y;
					H = rect_queue[i].h;
					X = rect_queue[i].x + rect_queue[i].w + 40;
					while (X + 40 <= 1900){
						W = Rand_0_5()*40;
						if (X + W + 40 >= 1900){
							W = 1900 - 40 - X;
							if (X == 1860){
								X = X-40;
								W = 40;
							}
						}
						SDL_Rect block = {X, Y, W, H};
						red_blocks.push_back(block);
						X = X + W + 40;
					}
				}
				for (int i =0 ; i<no_vertical_blocks+1; i++){
					for (int j =0; j<2; j++){
						if (j==0){
							X = (int)(((double)rand()/RAND_MAX)*12.5)*40 + 220;
						}
						else { X = (int)(((double)rand()/RAND_MAX)*17.5)*40 + 1020;}
						Y = 20;
						if(i>0) {
							rect_para r = rect_queue[i-1];
							Y = r.y + r.h;
						}
						SDL_Rect block = {X, Y, 40, 40};

						red_blocks.push_back(block);
					}
				}
				SDL_Rect left_border = {0, 0, 20, 1080}; 
				SDL_Rect right_border = {1900, 0, 20, 1080};
				SDL_Rect top_border = {10, 0, 1900, 20 };
				SDL_Rect bottom_border = {10, 1060, 1900, 20};
				
				//SDL_Rect ghost_home = {810, 0, 60, 60};

				red_blocks.push_back(left_border);
				red_blocks.push_back(right_border);
				red_blocks.push_back(top_border);
				red_blocks.push_back(bottom_border);
				//red_blocks.push_back(ghost_home);
			
			//Get pixel data
			Uint32* pixels = (Uint32*)gBGTexture.getPixels();
			int pixelCount = ( gBGTexture.getPitch() / 4 ) * gBGTexture.getHeight();
			int width = gBGTexture.getPitch()/4;

			//Map colors
			Uint32 colorKey = SDL_MapRGB( mappingFormat, 0xCC, 0x00, 0x00 );
			Uint32 transparent = SDL_MapRGBA( mappingFormat, 0xFF, 0xFF, 0x00, 0xFF );

			//Color key pixels
			int n = red_blocks.size();
			for (int k=0;k<n; k++){
				int x = red_blocks[k].x;
				int y = red_blocks[k].y;
				int w = red_blocks[k].w;
				int h = red_blocks[k].h;
				for (int i=0; i<h; i++){
					for (int j=0; j<w; j++){
						pixels[1920*(i+y) + (j+x)] = colorKey;
					}
				}
			}

			//Unlock texture
			gBGTexture.unlockTexture();

			//Free format
			SDL_FreeFormat( mappingFormat );
		}
	}
	//Load sound effects
	gMunch_a = Mix_LoadWAV( "sound/munch_a.wav" );
	if( gMunch_a == NULL )
	{
		printf( "Failed to load pellet munch_a sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
	gMunch_b = Mix_LoadWAV( "sound/munch_b.wav" );
	if( gMunch_b == NULL )
	{
		printf( "Failed to load pellet munch_b sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
	gMenu = Mix_LoadWAV( "sound/Among Us Theme (mp3cut.net).wav" );
	if( gMenu == NULL )
	{
		printf( "Failed to load pellet munch_b sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
	gDead = Mix_LoadWAV( "sound/death.wav" );
	if( gDead == NULL )
	{
		printf( "Failed to load dead chunk! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
	gKill= Mix_LoadWAV( "sound/Impostor Kill (online-audio-converter.com).wav" );
	if( gKill == NULL )
	{
		printf( "Failed to load dead chunk! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
	//Open the font
	gFont = TTF_OpenFont( "CONSOLA.TTF", 28 );
	if( gFont == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	return success;
}

void close(){
//Free loaded images
	gPac_leftTexture.free();
	gPac_ritTexture.free();
	gPac_upTexture.free();
	gPac_downTexture.free();
	gbottom_strip.free();
	gBGTexture.free();
	gGh1Texture.free();
	gstart.free();
	gbpoint.free();
	gspoint.free();
	gGameover.free();
	ggreenplayer.free();
	gyellowplayer.free();
	gScoreboard.free();
	gInstructions.free();
	//free sound effect
	Mix_FreeChunk( gMunch_a );
	Mix_FreeChunk(gMunch_b);
	Mix_FreeChunk(gMenu);
	Mix_FreeChunk(gKill);
	gMunch_a = NULL;
	gMunch_b = NULL;
	gMenu = NULL;
	gKill = NULL;
	Mix_FreeChunk( gDead );
	gDead = NULL;
	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;
	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
	Mix_Quit();
	//Free global font
	TTF_CloseFont( gFont ); gFont = NULL; TTF_Quit();
}


bool gameover(){
	bool success =true;
	Mix_PlayChannel(-1,gKill,0);
	gPac_leftTexture.free();
	gPac_ritTexture.free();
	gPac_upTexture.free();
	gPac_downTexture.free();
	//gBGTexture.free();
	//gGh1Texture.free();
	red_blocks.clear();

	return success;
	//animation;
}
int main( int argc, char* args[] )
{
	string game_state = "start";
	int pac1score = 0;
	int pac2score = 0;
	clock_t startimepac,endtimepac1,endtimepac2;
	bool player1 = true;
	bool player2 = true;
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
again:		//Load media
		
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			Mix_VolumeChunk(gMenu,2);
			Mix_VolumeChunk(gDead, 2);
			Mix_VolumeChunk(gMunch_a, 2);
			Mix_VolumeChunk(gKill, 2);
			string difficulty = "easy";
			vector<node> nodes;
			vector<SDL_Rect> small_points;
			vector<SDL_Rect> big_points;
			int no_redblks = red_blocks.size();
			bool intersects =false;
			int no_paths = 4;
			SDL_Rect check_blocks[4];
			

			for(int x = 20; x < 1900; x+=40){
				for(int y = 20; y < 1060; y+=40){
					intersects = false;
					SDL_Rect block = {x,y,40,40};
					for (int i=0; i<no_redblks; i++){
						if(SDL_HasIntersection(&block, &red_blocks[i])){
							intersects = true;
						}
					}
					if(!intersects){
						bool push = false;
						small_points.push_back(block);
						no_paths = 4;
						bool nobloc[4] ={true,true,true,true};
						check_blocks[0] = {x-40,y,40,40};	//left
						check_blocks[1] = {x, y-40, 40, 40};//top
						check_blocks[2] = {x+40, y, 40, 40};//right
						check_blocks[3] = {x, y+40, 40, 40};//bottom
						for(int i=0; i<4; i++){ nobloc[i] = true;}

						for(int j=0; j<no_redblks; j++){
							
							if(nobloc[0] && SDL_HasIntersection(&check_blocks[0],&red_blocks[j]) ){
								no_paths--; nobloc[0]=false;
							}
							if(nobloc[1] && SDL_HasIntersection(&check_blocks[1],&red_blocks[j]) ){
								no_paths--; nobloc[1]=false;
							}
							if(nobloc[2] && SDL_HasIntersection(&check_blocks[2],&red_blocks[j]) ){
								no_paths--; nobloc[2]=false;
							}
							if(nobloc[3] && SDL_HasIntersection(&check_blocks[3],&red_blocks[j]) ){
								no_paths--; nobloc[3]=false;
							}
							if(no_paths == 1){break;}
						}
						if(no_paths == 1){
							push = true;
							big_points.push_back(block);
						}
						else if(no_paths>2){
							push = true;
						}else if(no_paths ==2 && nobloc[0]!=nobloc[2]){
							push = true;
						}
						if(push){
							string temp = "";
							if(nobloc[0]){ temp += "1";  }//left
							else{ temp += "0"; }

							if(nobloc[1]){ temp += "1"; }//top
							else{ temp += "0"; }
							
							if(nobloc[2]){ temp += "1"; }//right
							else{ temp += "0"; }
							
							if(nobloc[3]){ temp += "1"; }//bottom
							else{ temp += "0"; }
							//printf("%i,%i,%i\n",x,y,4-no_paths);
							node newnode(x,y,temp, no_paths);
							nodes.push_back(newnode);
						}
					}
				}
			}
			
			//Main loop flag
			bool quit = false;
			SDL_Color textColor = { 255, 255, 0 };// yellow
			//Event handler
			SDL_Event e;

			//The Pac that will be moving around on the screen
			Pac pac(red_blocks,0);
			Pac pac1(red_blocks,1);

			Ghost gh1(red_blocks, nodes, 0);
			Ghost gh2(red_blocks, nodes, 1);
			Ghost gh3(red_blocks, nodes, 2);
			Ghost gh4(red_blocks, nodes, 3);
 
			//The camera area
			SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT ){
						quit = true;
					}
					//Handle input for the Pac
					pac.handleEvent(e);
					pac1.handleEvent1(e);
					if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    				{
        				if( e.key.keysym.sym == SDLK_y)
        				{
							pac1score=0;
							pac2score=0;
							player1= true;
							player2=true;
							game_state = "start";
							goto again;
        				}
						if( e.key.keysym.sym == SDLK_i)
        				{
							//gBGTexture.render(0,0);
							game_state = "instructions";
							goto again;
        				}
						if( e.key.keysym.sym == SDLK_t)
        				{
							//gBGTexture.render(0,0);
							game_state = "summary";
							goto again;
        				}
						if(game_state == "start"){
							if( e.key.keysym.sym == SDLK_h){
								game_state = "play";
								difficulty = "hard";
        					}else if ( e.key.keysym.sym == SDLK_e){
								game_state = "play";
							}
						}
    				}

				}
				if(game_state == "over"){ //Clear screen
					textColor = {255,0,0};
					SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
					SDL_RenderClear(gRenderer);
					Mix_PlayChannelTimed( -1,gMenu,0,6000);
					if (pac1score>pac2score){
						gyellowplayer.render(0,0);
					}
					if (pac1score<pac2score){
						ggreenplayer.render(0,0);
					}
					SDL_RenderPresent( gRenderer );
					
				}
				else if(game_state == "summary" ){
					SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
					SDL_RenderClear( gRenderer );
					Mix_PlayChannelTimed( -1,gMenu,0,6000);
					gScoreboard.render(0,0);
					textColor = {0,255,0};
					gTextTexture.loadFromRenderedText( "SCORE = "+to_string(pac2score), textColor );
					gTextTexture.render(280,500);
					gTextTexture.loadFromRenderedText( "TIME TAKEN = "+to_string(endtimepac2-startimepac), textColor );
					gTextTexture.render(280,550);
					textColor = {255,255,0};
					gTextTexture.loadFromRenderedText( "SCORE = "+to_string(pac1score), textColor );
					gTextTexture.render(1350,500);
					gTextTexture.loadFromRenderedText( "TIME TAKEN = "+to_string(endtimepac1-startimepac), textColor );
					gTextTexture.render(1350,550);
					red_blocks.clear();
					SDL_RenderPresent( gRenderer );
				}
				else if(game_state == "instructions" ){
					SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
					SDL_RenderClear( gRenderer );
					gInstructions.render(0,0);
					red_blocks.clear();
					SDL_RenderPresent( gRenderer );
				}

				else if(game_state == "play"){
				//Move the Pac
				pac.move();
				pac1.move();
				gh1.move(pac.getPosX(), pac.getPosY());
				gh2.move(pac1.getPosX(), pac1.getPosY());
				if (difficulty == "hard"){
					gh3.move(pac.getPosX(), pac.getPosY());
					gh4.move(pac1.getPosX(), pac1.getPosY());
				}
				if (player1==false && player2==false){
		
					game_state = "over"; gameover();
				}
				if(player1==true && SDL_HasIntersection(&pac.PacBox, &gh1.PacBox )){
					
					endtimepac1 = time(NULL);
					cout<<endtimepac1-startimepac<<endl;
					player1 = false;
					Mix_PlayChannel(-1,gKill,0);
					pac.pactoghostrender(camera.x,camera.y);
					
				}else if(player1 && SDL_HasIntersection(&pac.PacBox, &gh2.PacBox)){
				
					endtimepac1 = time(NULL);
					cout<<endtimepac1-startimepac<<endl;
					player1 = false;
					Mix_PlayChannel(-1,gKill,0);
					pac.pactoghostrender(camera.x,camera.y);
					
				}else if(player2 && SDL_HasIntersection(&pac1.PacBox, &gh1.PacBox)){
					
					endtimepac2 = time(NULL);
					player2 = false;
					cout<<endtimepac2-startimepac<<endl;
					Mix_PlayChannel(-1,gKill,0);
					pac1.pactoghostrender(camera.x,camera.y);
					
				}else if(player2 && SDL_HasIntersection(&pac1.PacBox, &gh2.PacBox)){
				
					endtimepac2 = time(NULL);
					player2 = false;
					cout<<endtimepac2-startimepac<<endl;
					Mix_PlayChannel(-1,gKill,0);
					pac1.pactoghostrender(camera.x,camera.y);
					
				}
				if (difficulty == "hard"){
					if(player1==true){
						if(SDL_HasIntersection(&pac.PacBox, &gh3.PacBox)){
							
							endtimepac1 = time(NULL);
							cout<<endtimepac1-startimepac<<endl;
							player1 = false;
							Mix_PlayChannel(-1,gKill,0);
							pac.pactoghostrender(camera.x,camera.y);
							
						}else if(SDL_HasIntersection(&pac.PacBox, &gh4.PacBox)){
						
							endtimepac1 = time(NULL);
							cout<<endtimepac1-startimepac<<endl;
							player1 = false;
							Mix_PlayChannel(-1,gKill,0);
							pac.pactoghostrender(camera.x,camera.y);
				
						}
					}
					if (player2 ==true){
						if(SDL_HasIntersection(&pac1.PacBox, &gh3.PacBox)){
				
							endtimepac2 = time(NULL);
							cout<<endtimepac2-startimepac<<endl;
							player2 = false;
							Mix_PlayChannel(-1,gKill,0);
							pac1.pactoghostrender(camera.x,camera.y);
					
						}else if(SDL_HasIntersection(&pac1.PacBox, &gh4.PacBox)){
					
							endtimepac2 = time(NULL);
							cout<<endtimepac2-startimepac<<endl;
							player2 = false;
							Mix_PlayChannel(-1,gKill,0);
							pac1.pactoghostrender(camera.x,camera.y);
						}
					}
				}
				
				
				if (player1==false && player2){
					if(SDL_HasIntersection(&pac1.PacBox,&pac.PacBox)){
						endtimepac2 = time(NULL);
						cout<<endtimepac2-startimepac<<endl;
						Mix_PlayChannel(-1,gKill,0);
						player2=false;
						
					}
				}
				if (player2==false && player1){
					if(SDL_HasIntersection(&pac.PacBox,&pac1.PacBox)){
					
						endtimepac1 = time(NULL);
						cout<<endtimepac1-startimepac<<endl;
						player1=false;		
						Mix_PlayChannel(-1,gKill,0);
					}
				}
				//Center the camera over the Pac
				camera.x = ( pac.getPosX() + Pac::PAC_WIDTH / 2 ) - SCREEN_WIDTH / 2;
				camera.y = ( pac.getPosY() + Pac::PAC_HEIGHT / 2 ) - SCREEN_HEIGHT / 2;

				//Keep the camera in bounds
				if( camera.x < 0 ){ camera.x = 0; }
				
				if( camera.y < 0 ){	camera.y = 0;}
				
				if( camera.x > LEVEL_WIDTH - camera.w ){ camera.x = LEVEL_WIDTH - camera.w; }
				
				if( camera.y > LEVEL_HEIGHT - camera.h ){ camera.y = LEVEL_HEIGHT - camera.h; }

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
				SDL_RenderClear( gRenderer );
				//Render background
				gBGTexture.render( 0, 0, &camera );
				//render points
				if (player1==true){
					for(int i=0; i<small_points.size(); i++){
						SDL_Rect block = small_points[i];
						if(SDL_HasIntersection(&pac.PacBox, &block) ){
							Mix_PlayChannel( -1, gMunch_a, 0 );
							//Mix_PlayChannel( -1, gMunch_b, 0 );
							small_points.erase(small_points.begin()+i);
							pac1score += 10;
							continue;
						}
						gspoint.render(block.x - camera.x+10, block.y - camera.y+10);
					}
				}

				if (player1==true){
					for(int i=0; i<big_points.size(); i++){
						SDL_Rect block = big_points[i];
						if(SDL_HasIntersection(&pac.PacBox, &block)){
							big_points.erase(big_points.begin()+i);
							pac1score+=50;
							continue;
						}
						gbpoint.render(block.x - camera.x, block.y-camera.y);
					}
				}
				if(player2==true){
					for(int i=0; i<small_points.size(); i++){
						SDL_Rect block = small_points[i];
						if(SDL_HasIntersection(&pac1.PacBox, &block) ){
							Mix_PlayChannel( -1, gMunch_a, 0 );
							//Mix_PlayChannel( -1, gMunch_b, 0 );
							small_points.erase(small_points.begin()+i);
							pac2score += 10;
							continue;
						}
						gspoint.render(block.x - camera.x+10, block.y - camera.y+10);
					}
				}
				if(player2 ==true){
					for(int i=0; i<big_points.size(); i++){
						SDL_Rect block = big_points[i];
						if(SDL_HasIntersection(&pac1.PacBox, &block)){
							big_points.erase(big_points.begin()+i);
							pac2score+=50;
							continue;
						}
						gbpoint.render(block.x - camera.x, block.y-camera.y);
					}
				}

				//Render objects
				gh1.render(camera.x, camera.y);
				gh2.render(camera.x, camera.y);
				if(difficulty == "hard"){
					gh3.render(camera.x, camera.y);
					gh4.render(camera.x, camera.y);
				}
				if (player1 ==true){
					pac.render( camera.x, camera.y);
				}
				if (player2 ==true){
					pac1.render1( camera.x, camera.y);
				}
				if (player1 ==false){
					pac.pactoghostrender( camera.x, camera.y);
				}
				if (player2 ==false){
					pac1.pactoghostrender( camera.x, camera.y);
				}
				
				//Update screen
				SDL_RenderPresent( gRenderer );
				}
				else if(game_state == "start"){
					startimepac = time(NULL);
					SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
					SDL_RenderClear( gRenderer );
					gstart.render(0,0);
					Mix_PlayChannelTimed( -1,gMenu,0,6000);
					SDL_RenderPresent( gRenderer );
				}
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
