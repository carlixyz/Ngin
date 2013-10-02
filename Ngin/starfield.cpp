

//
// TinyPTC Starfield
// Joncom2000 of TTD
//
// A little example of plotting some stars
// (and TV synth demo at bottom commented)


#include "tinyptc.h"
#include <stdlib.h>
#include <iostream>
#include <math.h>
// Setuo screen
#define WIDTH 640
#define HEIGHT 480
#define SIZE WIDTH*HEIGHT
// Setup a buffer for our reading and writing   
static int buffer[SIZE];
// Function declares
void Cls(int rgb);
void WritePixel(int x,int y,int rgb);
// Starfield variables
int starx[600];
int stary[600];

struct Sprite
{
	int width;
	int height;
	DWORD* data;
};


int main()
{
	int i; //For loops
	int speed; //For speed calcs
	// Open our window/screen
    if (!ptc_open("Stars_TTD by Joncom2000",WIDTH,HEIGHT)) return 1;
	// setup stars
	for (i=0; i<600; i++)
	{
		starx[i]=rand() % WIDTH;
		stary[i]=rand() % HEIGHT;
		//starx[i]= cos((float)i )* WIDTH *.5f;
		//stary[i]= sin((float)i )* HEIGHT *.5f;
	}
	// main loop exit
    while (1)
    {
		Cls(0);
		for (i=0; i<600; i++)
		{
			speed=1+(i % 3);
			starx[i]=starx[i]+speed;
			if (starx[i]>=WIDTH)
			{
				starx[i]=0;
			}
			WritePixel(starx[i],stary[i],((85*speed<<16)|(85*speed<<8)|85*speed));
			//WritePixel(starx[i],stary[i],0xffff00);
			//if(i>500) std::cout << "hello World: " << aux << std::endl;
		}
        ptc_update(buffer);
    }
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

//
// TinyPTC by Gaffer
// www.gaffer.org/tinyptc
//

//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
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