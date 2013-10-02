#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <time.h>


#include "tinyptc.h"

//#include "playmu/playmu.h"

#define PI 3.14159265f

#define WIDTH 640
#define HEIGHT 480
#define SIZE WIDTH*HEIGHT



#define HALF_W ((int)( WIDTH*.5))
#define HALF_H ((int)(HEIGHT*.5))

#define rnd(n) (rand()%(n+1))

#include "gfx.h"

const int maxplatforms= 1024;
Platform platforms[maxplatforms];

const int maxentoids=1024;
Entoid entoids[maxentoids];

const int numptypes= 20;
Sprite spr_platform[numptypes];

Sprite Test;
Sprite spr_room[10];
Sprite spr_gem[5];
Sprite spr_spark[3];
Sprite spr_calvin[3];
Sprite spr_drone[2];
Sprite spr_enemy[6];
Sprite spr_border;
Sprite spr_bg1, spr_bg2;
Sprite spr_win, spr_lose;


int main()
{
	ptc_open( "Jynx", WIDTH, HEIGHT);

	bool success = true;

	if ( gfx::LoadSprite(Test, "calvin1.png")) success = true;
			
	if(!success)
	{
		MessageBox(NULL, "Error loading graphics data", "Error", 0);
		//playmu_close();

		ptc_close();
		ExitProcess(0);
	}

	while( true)
	{
		//ptc_update(gfx::ptc_screen);
		gfx::DrawSprite(Test, 40, 40, false);
		gfx::UpdateScreen();	

		//gfx::Cls( (255 << 16 | 240 << 8 | 200)) ;

	}


	gfx::GCFree();

	ptc_close();

	ExitProcess(0);

}





//-----------------------------------------------------------------
// Our Functions
//-----------------------------------------------------------------
















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