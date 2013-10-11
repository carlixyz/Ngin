#ifndef _GFX_H_
#define _GFX_H_

//#define _CRT_SECURE_NO_WARNINGS

#include "tinyptc.h"
#include <string>
#include <iostream>
#include <vector>


// and link the libs!
#pragma comment(lib, "winmm.lib")
#pragma comment( lib, "corona.lib" )

#include "corona.h"

struct Vector2D 
{
	double x, y;
};
struct Vector3D 
{
	double x, y, z;
};
struct SectorLine 
{
	Vector2D coord_1;
	Vector2D coord_2;
	bool solid;
};
struct Rect 
{
        double x_min, y_min, x_max, y_max;
};

//struct ColorRGB
//{
//  int r;
//  int g;
//  int b;
//  
//  ColorRGB(Uint8 r, Uint8 g, Uint8 b);
//  ColorRGB(const ColorRGB8bit& color);
//  ColorRGB();
//};

struct Sprite
{
	int width;
	int height;
	DWORD* data;
};

struct Box
{
	int x;
	int y;
	int w;
	int h;
};

struct Entoid
{
	bool exists;
	int type;
	int timer;
	int x;
	int y;
	Sprite* sprite;
	bool flipped;
	int layer;
	// local box
	Box box;
	// global (collision) box
	Box cbox;
	// current messages
	int msg[20];
	int msgcount;
	// new messages
	int msgn[20];
	int msgncount;
	// local data (type specific)
	void* data;
	// update function
	bool (*TickFunc)(Entoid&);
};

struct Platform
{
	bool exists;
	int x;
	int y;
	int w;
	int h;
	Sprite* sprite;
};


namespace gfx{

static DWORD ptc_screen[SIZE];
DWORD screen[HALF_W*HALF_H];

// garbage collection (well,acoordly to Dr. Petters kindaof)
#define GCNEW 0
#define GCNEWARRAY 1
#define GCMALLOC 2

void* gcptr[200];
int gcmethod[200];
int numgc=0;

void GCAdd(void* ptr, int method)
{
	gcptr[numgc]=ptr;
	gcmethod[numgc]=method;
	numgc++;
}

void GCFree()
{
	for(int i=0;i<numgc;i++)
	{
		switch(gcmethod[i])
		{
		case GCNEW:
			delete gcptr[i];
			break;
		case GCNEWARRAY:
			delete[] gcptr[i];
			break;
		case GCMALLOC:
			free(gcptr[i]);
			break;
		}
	}
}




void UpdateScreen()
{
	DWORD * dp = ptc_screen;
	DWORD * sp = screen;
	for( int y= 0; y < HALF_H ; y++)
	{
		for( int x=0; x < HALF_W; x++)
		{
			*dp++ = *sp;
			*dp++ = *sp++;
		}
		sp -= HALF_W;
		for( int x= 0; x < HALF_W; x++)
		{
			*dp++ = *sp;
			*dp++ = *sp++;
		}
	}	
	ptc_update( ptc_screen );
}

void DrawBar(int sx, int sy, int w, int h, DWORD color)
{
	for (int y=sy ; y < sy+h ; y++)
	{
		if ( y < 0 ) continue;
		if ( y >= HALF_H) break;
		int i=y*HALF_W+sx;

		for (int x=sx ; x < sx+w ; x++ )
			if ( x >= 0 && x <= HALF_W )
				screen[i++] = color;
		
	}
}

void DrawSprite(Sprite &sprite, int sx, int sy, bool flipped)
{
	DWORD* dps= screen + sy * HALF_W;
	for ( int y = 0; y < sprite.height; y++)
	{
		if (sy+y<0)
		{
			y=-sy-1;
			dps=screen;
			continue;
		}
		if (sy+y>=HALF_H)
			break;
		DWORD* dp=dps+sx;
		int fx = 0;
		int w = sprite.width;
		if(sx<0)
		{
			dp-=sx;
			fx-=sx;
			//sp-=sx;
			w+=sx;
		}
		if(sx+sprite.width>HALF_W)
			w-=sx+sprite.width-HALF_W;
		int ds=1;
		DWORD* sp;
		if ( flipped )
		{
			//sp+=sprite.width;
			sp=sprite.data+y*sprite.width+sprite.width-fx-1;
			ds=-1;
		}
		else
			sp=sprite.data+y*sprite.width+fx;
		for( int x = 0; x<w ; x++ )
		{
			DWORD c= *sp;
			sp += ds;
			if ( c&0xFF000000 )
				*dp=c;
			dp++;
		}
		dps+= HALF_W;
	}
}

void DrawSprite(Sprite &sprite, int sx, int sy)
{
	DrawSprite( sprite, sx, sy, false);
}





//#define GetByte(X,Y) (*(((unsigned char *)&X)+Y)) //Dirty hack to get, Counts as 0, 1, 2 or 3

bool LoadSprite(Sprite &sprite, char* filename)
{
	corona::Image* img;

	img = corona::OpenImage(filename);
	if(img==NULL) return false;

// get width, height and channels
	int width	= img->getWidth();
	int height	= img->getHeight();
	int channels = 3;

	if ( img->getFormat() == corona::PF_R8G8B8 /*|| img->getFormat() == corona::PF_B8G8R8*/)// Iphone inverted
		channels = 3;

	sprite.width=width;
	sprite.height=height;

	//// Prepare sprite data memory allocation
	//sprite.data=(DWORD*)malloc(width*height*sizeof(DWORD));
	sprite.data= new DWORD[width*height] ;

	BYTE* Pix = (BYTE*)img->getPixels();
	//

	for (int i = 0; i < width * height; ++i) 
	{

		DWORD pixel=0xFF000000;

		pixel|= *Pix++ <<16;
		pixel|= *Pix++ <<8;
		pixel|= *Pix++;

		//if((int)GetByte(pixel, 2)==255 && (int)GetByte(pixel, 1)==240 && (int)GetByte(pixel, 0)==200)
			
		unsigned char *bytes = (unsigned char*)&pixel; // Let's try to explain this:
		// -- get pixel first byte pointer and set in bytes.. then travel it around the next R G B colors

		if	(  255 == (int)bytes[2] 
			&& 240 == (int)bytes[1] 
			&& 200 == (int)bytes[0] )
			pixel=0x00FFF0C8;

		sprite.data[i]=pixel;
	}

	GCAdd(sprite.data, GCNEWARRAY);
	return true;

}



void Cls(int rgb)
{
	int x;
	int y;
	//Do cls stuff
	for (y=0; y<HEIGHT; y++)
	{
		for (x=0; x<WIDTH; x++)
		{
            ptc_screen[(y*WIDTH+x)] =rgb;
		}
	}
}

void WritePixel(int x,int y,int rgb)
{
	//Do write pixel
	if(x>-1)
	{
		if(x<WIDTH)
		{
			if(y>-1)
			{
				if(y<HEIGHT)
				{
					ptc_screen[(y*WIDTH+x)] =rgb;
				}
			}
		}
	}
}

//Fast vertical line from (x,y1) to (x,y2), with rgb color
bool verLine(int sx, int sy1, int sy2, const DWORD& color)
{
  if(sy2 < sy1)
	{sy1 += sy2; sy2 = sy1 - sy2; sy1 -= sy2;} //swap y1 and y2
  if(sy2 < 0 || sy1 >= HEIGHT  || sx < 0 || sx >= WIDTH) 
	  return 0; //no single point of the line is on screen

  //if(sy1 < 0) 
	 // sy1 = 0;			//clip top
  //if(sy2 >= WIDTH) 
	 // sy2 = HEIGHT - 1;	//clip bottom

  	for (int y=sy1 ; y < sy2 ; y++)
	{
		if ( y < 0 ) continue;
		if ( y >= HEIGHT) break;
		//int i=y*WIDTH+sx;

		//for (int x=sx ; x < sx+w ; x++ )
		//	if ( x >= 0 && x <= HALF_W )
		//		screen[i++] = color;
		if ( sx >= 0 && sx <= WIDTH )
			ptc_screen[y*WIDTH+sx] = color;
	}
   
  return 1;
}
void BufferDraw(DWORD buffer[])
{
	for(int x = 0; x < WIDTH; x++) 
		for(int y = 0; y < HEIGHT; y++) 
			ptc_screen[y*WIDTH+x] = buffer[y*WIDTH+x]; //clear the buffer instead of cls()
}
void DrawBuffer(DWORD buffer[WIDTH][HEIGHT])
{
	for(int x = 0; x < WIDTH; x++) 
		for(int y = 0; y < HEIGHT; y++) 
			ptc_screen[y*WIDTH+x] = buffer[x][y]; //clear the buffer instead of cls()
}
//Draws a buffer of pixels to the screen
void drawBuffer(DWORD* buffer )
{
	for(int x = 0; x < WIDTH; x++) 
		for(int y = 0; y < HEIGHT; y++) 
			ptc_screen[y*WIDTH+x] = buffer[HEIGHT * x + y]; //clear the buffer instead of cls()
}


bool loadImage(DWORD* buffer, char* filename)
{
	corona::Image* img;

	img = corona::OpenImage(filename);
	if(img==NULL) return false;

// get width, height and channels
	int width	= img->getWidth();
	int height	= img->getHeight();
	int channels = 3;

	if ( img->getFormat() == corona::PF_R8G8B8 /*|| img->getFormat() == corona::PF_B8G8R8*/)// Iphone inverted
		channels = 3;

	//buffer = new DWORD[width*height] ;

	BYTE* Pix = (BYTE*)img->getPixels();
	//

	for (int i = 0; i < width * height; ++i) 
	{
		DWORD pixel=0xFF000000;

		pixel|= *Pix++ <<16;
		pixel|= *Pix++ <<8;
		pixel|= *Pix++;

		buffer[i]=pixel;
	}

	//GCAdd(buffer, GCNEWARRAY);
	return true;
}



bool loadImage(Uint32* buffer, char* filename)
{
	corona::Image* img;

	img = corona::OpenImage(filename);
	if(img==NULL) return false;

// get width, height and channels
	int width	= img->getWidth();
	int height	= img->getHeight();
	int channels = 3;

	if ( img->getFormat() == corona::PF_R8G8B8 /*|| img->getFormat() == corona::PF_B8G8R8*/)// Iphone inverted
		channels = 3;

	//buffer = new DWORD[width*height] ;

	Uint32* Pix = (Uint32*)img->getPixels();
	//

	for (int i = 0; i < width * height; ++i) 
	{
		Uint32 pixel=0xFF000000;

		pixel|= *Pix++ <<16;
		pixel|= *Pix++ <<8;
		pixel|= *Pix++;

		buffer[i]=pixel;
	}

	//GCAdd(buffer, GCNEWARRAY);
	return true;
}
}; // end GFx namespace


#endif

