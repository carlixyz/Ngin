#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <time.h>


//#include "tinyptc.h"

//#include "playmu/playmu.h"

#define PI 3.14159265f

#define WIDTH 640
#define HEIGHT 480
#define SIZE WIDTH*HEIGHT


#define HALF_W ((int)( WIDTH*.5))
#define HALF_H ((int)(HEIGHT*.5))

#define rnd(n) (rand()%(n+1))

#include "gfx.h"

Sprite Test;

#define mapWidth 24
#define mapHeight 24

int worldMap[mapWidth][mapHeight]=
{
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};



int main()
{
	Vector2D pos	= {22, 12};		//x and y start position
	Vector2D dir	= {-1, 0};		//initial direction vector
	Vector2D plane	= {0, 0.66};	//the 2d raycaster version of camera plane

	double time		= 0;			//time of current frame
	double oldTime	= 0;			//time of previous frame


	ptc_open( "Raycaster", WIDTH, HEIGHT);

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
		//gfx::Cls( (255 << 16 | 240 << 8 | 200)) ;

		for(int x = 0; x < WIDTH; x++)
		{
			//calculate ray position and direction 
			double cameraX = 2 * x / double(WIDTH) - 1; //x-coordinate in camera space
			Vector2D rayPos = {pos.x, pos.y};
			Vector2D rayDir = {dir.x + plane.x * cameraX, dir.y + plane.y * cameraX};

			// which box in the map are we
			int mapX = (int)rayPos.x;
			int mapY = (int)rayPos.y;

			//length of ray from current position to next x or y-side
			Vector2D sideDist = {0,0};

			//length of ray from one x or y-side to next x or y-side
			double deltaDistX = sqrt(1 + (rayDir.y * rayDir.y) / (rayDir.x * rayDir.x));
			double deltaDistY = sqrt(1 + (rayDir.x * rayDir.x) / (rayDir.y * rayDir.y));
			double perpWallDist = 0;

			//what direction to step in x or y-direction (either +1 or -1)
			int stepX = 0;
			int stepY = 0;

			int hit  = 0; //was there a wall hit?
			int side = 0; //was a NS or a EW wall hit?

			//calculate step and initial sideDist
			if (rayDir.x < 0)
			{
				stepX = -1;
				sideDist.x = (rayPos.x - mapX) * deltaDistX;
			}
			else
			{
				stepX = 1;
				sideDist.x = (mapX + 1.0 - rayPos.x) * deltaDistX;
			}
			if (rayDir.y < 0)
			{
				stepY = -1;
				sideDist.y = (rayPos.y - mapY) * deltaDistY;
			}
			else
			{
				stepY = 1;
				sideDist.y = (mapY + 1.0 - rayPos.y) * deltaDistY;
			}

			//perform DDA
			while (hit == 0)
			{
				//jump to next map square, OR in x-direction, OR in y-direction
				if (sideDist.x < sideDist.y)
				{
					sideDist.x += deltaDistX;
					mapX += stepX;
					side = 0;
				}
				else
				{
					sideDist.y += deltaDistY;
					mapY += stepY;
					side = 1;
				}
				//Check if ray has hit a wall
				if (worldMap[mapX][mapY] > 0) hit = 1;
			} 
			 //Calculate distance projected on camera direction (oblique distance will give fisheye effect!)
			if (side == 0)
				perpWallDist = fabs((mapX - rayPos.x + (1 - stepX) / 2) / rayDir.x);
			else
				perpWallDist = fabs((mapY - rayPos.y + (1 - stepY) / 2) / rayDir.y);

			//Calculate height of line to draw on screen
			int lineHeight = abs(int(HEIGHT / perpWallDist));

			//calculate lowest and highest pixel to fill in current stripe
			int drawStart = -lineHeight / 2 + HEIGHT / 2;
			if(drawStart < 0)
				drawStart = 0;
			int drawEnd = lineHeight / 2 + HEIGHT / 2;
			if(drawEnd >= HEIGHT)
				drawEnd = HEIGHT - 1;

			//choose wall color
			DWORD color;
			switch(worldMap[mapX][mapY])
			{
				case 1:  color = 0xFF0000;  break; //red
				case 2:  color = 0x00FF00;  break; //green
				case 3:  color = 0x0000FF;   break; //blue
				case 4:  color = 0xFFFFFF;  break; //white
				default: color = 0x00FFFF; break; //yellow
			}

			//give x and y sides different brightness
			if (side == 1) 
				color = color >> 2;

			//draw the pixels of the stripe as a vertical line
			gfx::verLine(x, drawStart, drawEnd, color);

		}


		// This is hexadecimal notation goes from 0-9 to A-E (0-15)
		//gfx::DrawBar(160-100, 10, 202, 6, 0x000000);

		gfx::DrawSprite(Test, 40, 40, false);
		//gfx::UpdateScreen();	
		ptc_update( gfx::ptc_screen );

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