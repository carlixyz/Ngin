#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <vector>

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
#define texWidth 64
#define texHeight 64
int worldMap[mapWidth][mapHeight]=
{
  {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,7,7,7,7,7,7,7,7},
  {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
  {4,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
  {4,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
  {4,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
  {4,0,4,0,0,0,0,5,5,5,5,5,5,5,5,5,7,7,0,7,7,7,7,7},
  {4,0,5,0,0,0,0,5,0,5,0,5,0,5,0,5,7,0,0,0,7,7,7,1},
  {4,0,6,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
  {4,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,7,1},
  {4,0,8,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
  {4,0,0,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,7,7,7,1},
  {4,0,0,0,0,0,0,5,5,5,5,0,5,5,5,5,7,7,7,7,7,7,7,1},
  {6,6,6,6,6,6,6,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
  {8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
  {6,6,6,6,6,6,0,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
  {4,4,4,4,4,4,0,4,4,4,6,0,6,2,2,2,2,2,2,2,3,3,3,3},
  {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
  {4,0,0,0,0,0,0,0,0,0,0,0,6,2,0,0,5,0,0,2,0,0,0,2},
  {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
  {4,0,6,0,6,0,0,0,0,4,6,0,0,0,0,0,5,0,0,0,0,0,0,2},
  {4,0,0,5,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
  {4,0,6,0,6,0,0,0,0,4,6,0,6,2,0,0,5,0,0,2,0,0,0,2},
  {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
  {4,4,4,4,4,4,4,4,4,4,1,1,1,2,2,2,2,2,2,3,3,3,3,3}
};

DWORD buffer[WIDTH][HEIGHT];
//DWORD buffer[WIDTH*HEIGHT];
//Uint32 buffer[WIDTH][HEIGHT];



void UpdateInput(Vector2D & pos, Vector2D & dir, Vector2D & plane, float lfTimestep)
{

		//speed modifiers
		double moveSpeed = lfTimestep * 5.0f; //the constant value is in squares/second
		double rotSpeed = lfTimestep * 3.0f; //the constant value is in radians/second

		//if(GetAsyncKeyState(VK_RIGHT)&KF_UP)
		    if (GetAsyncKeyState(VK_UP)&KF_UP)
			{
			  if(worldMap[int(pos.x + dir.x * moveSpeed)][int(pos.y)] == false) pos.x += dir.x * moveSpeed;
			  if(worldMap[int(pos.x)][int(pos.y + dir.y * moveSpeed)] == false) pos.y += dir.y * moveSpeed;
			}
			if (GetAsyncKeyState(VK_DOWN)&KF_UP)		//move backwards if no wall behind you
			{
			  if(worldMap[int(pos.x - dir.x * moveSpeed)][int(pos.y)] == false) pos.x -= dir.x * moveSpeed;
			  if(worldMap[int(pos.x)][int(pos.y - dir.y * moveSpeed)] == false) pos.y -= dir.y * moveSpeed;
			}
  
		    if (GetAsyncKeyState(VK_RSHIFT)&KF_UP)		//move rightside if no wall behind you
			{
			if(worldMap[int(pos.x + plane.x * moveSpeed)][int(pos.y)] == false) pos.x += plane.x * moveSpeed;
			  if(worldMap[int(pos.x)][int(pos.y + plane.y * moveSpeed)] == false) pos.y += plane.y * moveSpeed;
			}
			if (GetAsyncKeyState(VK_LSHIFT)&KF_UP)		//move leftside if no wall behind you
			{
			  if(worldMap[int(pos.x - plane.x * moveSpeed)][int(pos.y)] == false) pos.x -= plane.x * moveSpeed;
			  if(worldMap[int(pos.x)][int(pos.y - plane.y * moveSpeed)] == false) pos.y -= plane.y * moveSpeed;
			}

			if (GetAsyncKeyState(VK_RIGHT)&KF_UP)		//rotate to the right 		
			{
			  //both camera direction and camera plane must be rotated
			  double oldDirX = dir.x;
			  dir.x = dir.x * cos(-rotSpeed) - dir.y * sin(-rotSpeed);
			  dir.y = oldDirX * sin(-rotSpeed) + dir.y * cos(-rotSpeed);
			  double oldPlaneX = plane.x;
			  plane.x = plane.x * cos(-rotSpeed) - plane.y * sin(-rotSpeed);
			  plane.y = oldPlaneX * sin(-rotSpeed) + plane.y * cos(-rotSpeed);
			}
			
			if (GetAsyncKeyState(VK_LEFT)&KF_UP)		//rotate to the left	
			{
			  //both camera direction and camera plane must be rotated
			  double oldDirX = dir.x;
			  dir.x = dir.x * cos(rotSpeed) - dir.y * sin(rotSpeed);
			  dir.y = oldDirX * sin(rotSpeed) + dir.y * cos(rotSpeed);
			  double oldPlaneX = plane.x;
			  plane.x = plane.x * cos(rotSpeed) - plane.y * sin(rotSpeed);
			  plane.y = oldPlaneX * sin(rotSpeed) + plane.y * cos(rotSpeed);
			}
}

int main()
{

	Vector2D pos	= {22, 12};		//x and y start position
	Vector2D dir	= {-1, 0};		//initial direction vector
	Vector2D plane	= {0, 0.66};	//the 2d raycaster version of camera plane

	unsigned long luiLastTime = timeGetTime(); 

	DWORD textures[8][texWidth * texHeight];
	//std::vector<DWORD> textures[8];

	//for(int i = 0; i < 8; i++) 
	//	textures[i].resize(texWidth * texHeight);

	ptc_open( "Raycaster", WIDTH, HEIGHT);
	bool success = true;

	  //generate some textures
	if(! gfx::loadImage(textures[0], "pics/eagle.png") ) success = false;
	if(! gfx::loadImage(textures[1], "pics/redbrick.png") ) success = false;
	if(! gfx::loadImage(textures[2], "pics/purplestone.png") ) success = false;
	if(! gfx::loadImage(textures[3], "pics/greystone.png") ) success = false;
	if(! gfx::loadImage(textures[4], "pics/bluestone.png") ) success = false;
	if(! gfx::loadImage(textures[5], "pics/mossy.png") ) success = false;
	if(! gfx::loadImage(textures[6], "pics/wood.png") ) success = false;
	if(! gfx::loadImage(textures[7], "pics/colorstone.png") ) success = false;
	 // for(int x = 0; x < texWidth; x++)
	 // for(int y = 0; y < texHeight; y++)
	 // {
		//int xorcolor = (x * 256 / texWidth) ^ (y * 256 / texHeight);
		////int xcolor = x * 256 / texWidth;
		//int ycolor = y * 256 / texHeight;
		//int xycolor = y * 128 / texHeight + x * 128 / texWidth;
		//textures[0][texWidth * y + x] = 65536 * 254 * (x == y && x != texWidth - y); //flat red texture with black cross
		//textures[1][texWidth * y + x] = xycolor + 256 * xycolor + 65536 * xycolor; //sloped greyscale
		//textures[2][texWidth * y + x] = 256 * xycolor + 65536 * xycolor; //sloped yellow gradient
		//textures[3][texWidth * y + x] = xorcolor + 256 * xorcolor + 65536 * xorcolor; //xor greyscale
		//textures[4][texWidth * y + x] = 256 * xorcolor; //xor green
		//textures[5][texWidth * y + x] = 65536 * 192 * (x % 16 && y % 16); //red bricks
		//textures[6][texWidth * y + x] = 65536 * ycolor; //red gradient
		//textures[7][texWidth * y + x] = 128 + 256 * 128 + 65536 * 128; //flat grey texture
	 // }

	if (! gfx::LoadSprite(Test, "calvin1.png")) success = false;
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
		//gfx::Cls( 0) ;

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

			//perform DDA	(Checks if every deltaDist increm hit a wall block)
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

			 //texturing calculations
			int texNum = worldMap[mapX][mapY] - 1; //1 subtracted from it so that texture 0 can be used!

			//calculate value of wallX
			double wallX = 0; //where exactly the wall was hit
			  //Calculate distance of perpendicular ray (oblique distance will give fisheye effect!)
			if (side == 1) 
				perpWallDist = fabs((mapX - rayPos.x+ (1 - stepX) / 2) / rayDir.x);
			else           
				perpWallDist = fabs((mapY - rayPos.y + (1 - stepY) / 2) / rayDir.y);
			wallX -= floor((wallX));

			//x coordinate on the texture
			int texX = int(wallX * double(texWidth));
			if(side == 0 && rayDir.x > 0) texX = texWidth - texX - 1;
			if(side == 1 && rayDir.y < 0) texX = texWidth - texX - 1;

			for(int y = drawStart; y < drawEnd; y++)
			{
				int d = y * 256 - HEIGHT * 128 + lineHeight * 128;  //256 and 128 factors to avoid floats
				int texY = ((d * texHeight) / lineHeight) / 256;
				DWORD color = textures[texNum][texHeight * texY + texX];
				//make color darker for y-sides: R, G and B byte each divided through two with a "shift" and an "and"
				if(side == 1) color = (color >> 1) & 8355711;
				buffer[x][y] = color;
				//buffer[y*WIDTH + x] = color;
			}

		}

		gfx::drawBuffer(buffer[0]);
		//gfx::BufferDraw(buffer);
 

		for(int x = 0; x < WIDTH; x++) 
			for(int y = 0; y < HEIGHT; y++) 
				//buffer[y*WIDTH + x] = 0;
				buffer[x][y] = 0; //clear the buffer instead of cls()

		// Calculando el tiempo en segundos
        unsigned long luiActualTime = timeGetTime(); 
        float lfTimestep = ((float)(luiActualTime - luiLastTime)/ 1000.0f);
        luiLastTime     = luiActualTime;


		UpdateInput( pos, dir, plane, lfTimestep);
		// This is hexadecimal notation goes from 0-9 to A-E (0-15)
		//gfx::DrawBar(160-100, 10, 202, 6, 0x000000);

		//gfx::DrawSprite(Test, 40, 40, false);
		//gfx::UpdateScreen();	
		//gfx::DrawBuffer(buffer);
		//gfx::BufferDraw(buffer);

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