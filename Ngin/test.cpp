
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

//
// TinyPTC Starfield
// Joncom2000 of TTD
//
// A little example of plotting some stars
#include "tinyptc.h"
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include "gfx.h"


// Setuo screen
#define WIDTH 640
#define HEIGHT 480
#define SIZE WIDTH*HEIGHT





// Setup a buffer for our reading and writing   
static int buffer[SIZE];

// Function declares
void Cls(int rgb);
void WritePixel(int x,int y,int rgb);


int main()
{
	// Open our window/screen
    if (!ptc_open("Stars_TTD by Joncom2000",WIDTH,HEIGHT)) return 1;

	gfx::Get().Chachacha();
	int j = 0;	
	std::cout  <<  0xFFFFFF;

	// main loop exit
    while (1)
    {
		Cls(0xFFFFFF);
		int x;
		int y;

		//Do cls stuff
			for (y=0; y<HEIGHT; y++)
			{
				int factorY = y * .535f;			// screen range: 0xFF / HEIGHT ==  255/480 = 0.535
				for (x=0; x<WIDTH; x++)
				{
					int factorX = x * .398f;		// screen range: 0xFF / WIDTH ==  255/640 = 0.398
					buffer[(x+y*WIDTH)] = (factorY<<16) | (factorX<<8) | factorX;
				}
			}
					ptc_update(buffer);
    }

	ptc_close();
}





//-----------------------------------------------------------------
// Our Functions
//-----------------------------------------------------------------
void Cls(int rgb)
{
	int x;
	int y;
	//Do cls stuff
	for (y=0; y<HEIGHT; y++)
	{
		for (x=0; x<WIDTH; x++)
		{
            buffer[(y*WIDTH+x)] =rgb;
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
					buffer[(y*WIDTH+x)] =rgb;
				}
			}
		}
	}
}















/*
#include "tinyptc.h"
#include <png.h>

#define WIDTH 320
#define HEIGHT 240
#define SIZE WIDTH*HEIGHT

#define PI 3.14159265f
#define rnd(n) (rand()%(n+1))
   
static int noise;
static int carry;
static int index;
static int seed = 0x12345;
static int pixel[SIZE];


struct Sprite
{
	int width;
	int height;
	DWORD* data;
};


int main()
{


    if (!ptc_open("test",WIDTH,HEIGHT)) return 1;

    while (1)
    {
        for (index=0; index<SIZE; index++)
        {
            noise = seed;
            noise >>= 3;
            noise ^= seed;
            carry = noise & 1;
            noise >>= 1;
            seed >>= 1;
            seed |= (carry << 30);
            noise &= 0xFF;
            pixel[index] = (noise<<16) | (noise<<8) | noise;
        }
        ptc_update(pixel);
    }
}
*/