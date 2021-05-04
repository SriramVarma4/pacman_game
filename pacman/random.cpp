
/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, math, and strings
#include "Pac.h"
#include "Ghost.h"
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <time.h>
#include <iostream>
using namespace std;

//screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

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

//Texture wrapper class
class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path );
		
		#if defined(SDL_TTF_MAJOR_VERSION)
		//Creates image from font string
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
		#endif

		//Deallocates texture
		void free();

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );
		
		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

		//Pixel manipulators
		bool lockTexture();
		bool unlockTexture();
		void* getPixels();
		int getPitch();

		//Gets image dimensions
		int getWidth();
		int getHeight();

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;
		void* mPixels;				//@@new
		int mPitch;					//@@new
		//Image dimensions
		int mWidth;
		int mHeight;
};

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

//Scene textures
LTexture gPacTexture;
LTexture gBGTexture;	//this is the texture we got to manipulate
LTexture gGh1Texture;

LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
	mPixels = NULL;
	mPitch = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile( std::string path )
{
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

#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface != NULL )
	{
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	else
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}

	
	//Return success
	return mTexture != NULL;
}
#endif

void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
		mPixels = NULL;
		mPitch = 0;
	}
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( mTexture, blending );
}
		
void LTexture::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
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

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

bool LTexture::lockTexture()
{
	bool success = true;
	//Texture is already locked
	if( mPixels != NULL )
	{
		printf( "Texture is already locked!\n" );
		success = false;
	}
	//Lock texture
	else
	{
		if( SDL_LockTexture( mTexture, NULL, &mPixels, &mPitch ) != 0 )
		{
			printf( "Unable to lock texture! %s\n", SDL_GetError() );
			success = false;
		}
	}
	return success;
}

bool LTexture::unlockTexture()
{
	bool success = true;
	//Texture is not locked
	if( mPixels == NULL ){
		printf( "Texture is not locked!\n" );
		success = false;
	}
	//Unlock texture
	else{
		SDL_UnlockTexture( mTexture );
		mPixels = NULL;
		mPitch = 0;
	}
	return success;
}
void* LTexture::getPixels()
{
	return mPixels;
}
int LTexture::getPitch()
{
	return mPitch;
}

void Pac::render( int camX, int camY )
{
    //Show the pac relative to the camera
	gPacTexture.render( mPosX - camX, mPosY - camY );
}

void Ghost::render( int camX, int camY ){
	gGh1Texture.render(mPosX - camX, mPosY - camY);
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
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
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
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
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
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
	if( !gPacTexture.loadFromFile( "player.png" ) )
	{
		printf( "Failed to load pacman texture!\n" );
		success = false;
	}else{
		if( !gPacTexture.lockTexture() ){ printf( "Unable to lock Pacman' texture!\n" ); }
		else{
			Uint32 format = SDL_GetWindowPixelFormat( gWindow );
			SDL_PixelFormat* mappingFormat = SDL_AllocFormat( format );
			Uint32 transparent = SDL_MapRGBA( mappingFormat, 0x00, 0x00, 0x00, 0x00 );
			Uint32* pixels = (Uint32*)gPacTexture.getPixels();
			int pixelCount = ( gPacTexture.getPitch() / 4 ) * gPacTexture.getHeight();
			int width = gPacTexture.getPitch()/4;
			for(int i=0; i<pixelCount; i++){
				if(pixels[i]==16711935){
					pixels[i] = transparent;
				}
			}
			//Unlock texture
			gPacTexture.unlockTexture();
			//Free format
			SDL_FreeFormat( mappingFormat );
		}
	}
	//load ghost
	if( !gGh1Texture.loadFromFile( "icon.png" ) )
	{
		printf( "Failed to load gh1 texture!\n" );
		success = false;
	}
	//Load background texture

	if( !gBGTexture.loadFromFile( "black.jpg" ) )
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

	//Nothing to load
	return success;
}

void close()
{
//Free loaded images
	gPacTexture.free();
	gBGTexture.free();

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

/*SDL_Texture* loadTexture( std::string path )
{
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
		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}
	return newTexture;
}*/

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//The Pac that will be moving around on the screen
			Pac pac(red_blocks);

			Ghost gh1(red_blocks);

			//The camera area
			SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}

					//Handle input for the Pac
					pac.handleEvent( e );
				}

				//Move the Pac
				pac.move();
				gh1.move(pac.getPosX(), pac.getPosY());

				//Center the camera over the Pac
				camera.x = ( pac.getPosX() + Pac::PAC_WIDTH / 2 ) - SCREEN_WIDTH / 2;
				camera.y = ( pac.getPosY() + Pac::PAC_HEIGHT / 2 ) - SCREEN_HEIGHT / 2;

				//Keep the camera in bounds
				if( camera.x < 0 )
				{ 
					camera.x = 0;
				}
				if( camera.y < 0 )
				{
					camera.y = 0;
				}
				if( camera.x > LEVEL_WIDTH - camera.w )
				{
					camera.x = LEVEL_WIDTH - camera.w;
				}
				if( camera.y > LEVEL_HEIGHT - camera.h )
				{
					camera.y = LEVEL_HEIGHT - camera.h;
				}

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				//Render background
				gBGTexture.render( 0, 0, &camera );

				//Render objects
				
				pac.render( camera.x, camera.y);
				gh1.render(camera.x, camera.y);

				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}