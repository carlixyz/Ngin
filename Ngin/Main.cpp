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

struct Image
{
  double x;
  double y;
  int texture;
};

#define numImages 19
Image images[numImages] =
{
  {20.5, 11.5, 10}, //green light in front of playerstart
  //green lights in every room
  {18.5,4.5, 10},
  {10.0,4.5, 10},
  {10.0,12.5,10},
  {3.5, 6.5, 10},
  {3.5, 20.5,10},
  {3.5, 14.5,10},
  {14.5,20.5,10},

  //row of pillars in front of wall: fisheye test
  {18.5, 10.5, 9},
  {18.5, 11.5, 9},
  {18.5, 12.5, 9},

  //some barrels around the map
  {21.5, 1.5, 8},
  {15.5, 1.5, 8},
  {16.0, 1.8, 8},
  {16.2, 1.2, 8},
  {3.5,  2.5, 8},
  {9.5, 15.5, 8},
  {10.0, 15.1,8},
  {10.5, 15.8,8},
};
//1D Zbuffer
double ZBuffer[WIDTH];

//arrays used to sort the sprites
int imgOrder[numImages];
double imgDistance[numImages];

//function used to sort the sprites
void combSort(int* order, double* dist, int amount);

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

//DWORD buffer[WIDTH][HEIGHT];
DWORD buffer[WIDTH*HEIGHT];
//Uint32 buffer[WIDTH][HEIGHT];


POINT MousePos ;
int ScreenW = 0;
int ScreenH = 0;

void UpdateInput(Vector2D & pos, Vector2D & dir, Vector2D & plane, float lfTimestep)
{
		GetCursorPos(&MousePos);

		//speed modifiers
		double moveSpeed = lfTimestep * 5.0f; //the constant value is in squares/second
		double rotSpeed = lfTimestep * 3.0f; //the constant value is in radians/second

		//if(GetAsyncKeyState(VK_RIGHT)&KF_UP)
		    if (GetAsyncKeyState(VK_UP)&KF_UP || GetAsyncKeyState(0x57)&KF_UP)
			{
			  if(worldMap[int(pos.x + dir.x * moveSpeed)][int(pos.y)] == false) pos.x += dir.x * moveSpeed;
			  if(worldMap[int(pos.x)][int(pos.y + dir.y * moveSpeed)] == false) pos.y += dir.y * moveSpeed;
			}
			if (GetAsyncKeyState(VK_DOWN)&KF_UP || GetAsyncKeyState(0x53)&KF_UP)		//move backwards if no wall behind you
			{
			  if(worldMap[int(pos.x - dir.x * moveSpeed)][int(pos.y)] == false) pos.x -= dir.x * moveSpeed;
			  if(worldMap[int(pos.x)][int(pos.y - dir.y * moveSpeed)] == false) pos.y -= dir.y * moveSpeed;
			}
  
		    if (GetAsyncKeyState(VK_RSHIFT)&KF_UP || GetAsyncKeyState(0x44)&KF_UP)		//move rightside if no wall behind you
			{
			if(worldMap[int(pos.x + plane.x * moveSpeed)][int(pos.y)] == false) pos.x += plane.x * moveSpeed;
			  if(worldMap[int(pos.x)][int(pos.y + plane.y * moveSpeed)] == false) pos.y += plane.y * moveSpeed;
			}
			if (GetAsyncKeyState(VK_LSHIFT)&KF_UP || GetAsyncKeyState(0x41)&KF_UP)		//move leftside if no wall behind you
			{
			  if(worldMap[int(pos.x - plane.x * moveSpeed)][int(pos.y)] == false) pos.x -= plane.x * moveSpeed;
			  if(worldMap[int(pos.x)][int(pos.y - plane.y * moveSpeed)] == false) pos.y -= plane.y * moveSpeed;
			}

			if ( MousePos.x != ScreenW>>2 )
			{

				double oldDirX = dir.x;

				double tempRot = (double)((ScreenW>>2) - MousePos.x) * lfTimestep * .5;

			    dir.x = dir.x * cos(tempRot) - dir.y * sin( tempRot);
			    dir.y = oldDirX * sin(tempRot) + dir.y * cos(tempRot);
			    double oldPlaneX = plane.x;
			    plane.x = plane.x * cos(tempRot) - plane.y * sin(tempRot);
			   plane.y = oldPlaneX * sin(tempRot) + plane.y * cos(tempRot);

				SetCursorPos( ScreenW>>2,ScreenH>>2);
			}

			if ( (GetAsyncKeyState(VK_RIGHT)&KF_UP)  )		//rotate to the right 		
			{
			  //both camera direction and camera plane must be rotated
			  double oldDirX = dir.x;
			  dir.x = dir.x * cos(-rotSpeed) - dir.y * sin(-rotSpeed);
			  dir.y = oldDirX * sin(-rotSpeed) + dir.y * cos(-rotSpeed);
			  double oldPlaneX = plane.x;
			  plane.x = plane.x * cos(-rotSpeed) - plane.y * sin(-rotSpeed);
			  plane.y = oldPlaneX * sin(-rotSpeed) + plane.y * cos(-rotSpeed);
			}
			
			if (GetAsyncKeyState(VK_LEFT)&KF_UP  )		//rotate to the left	
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
#ifdef __PTC_WINDOWED__
	ScreenW = GetSystemMetrics(SM_CXSCREEN)*2;
	ScreenH = GetSystemMetrics(SM_CYSCREEN)*2;
#else
	ScreenW = GetSystemMetrics(SM_CXSCREEN);
	ScreenH = GetSystemMetrics(SM_CYSCREEN);
#endif

	GetCursorPos(&MousePos);
	SetCursorPos( ScreenW,ScreenH);

	Vector2D pos	= {22, 12};		//x and y start position
	Vector2D dir	= {-1, 0};		//initial direction vector
	Vector2D plane	= {0, 0.66};	//the 2d raycaster version of camera plane

	unsigned long luiLastTime = timeGetTime(); 

	//std::vector<Uint32> textures[8];
	//for(int i = 0; i < 8; i++) textures[i].resize(texWidth * texHeight);
	DWORD textures[11][texWidth*texHeight];

	bool success = (ptc_open( "Raycaster", WIDTH, HEIGHT) > 0);

	if(! gfx::loadImage(textures[0], "pics/eagle.png") ) success = false;
	if(! gfx::loadImage(textures[1], "pics/redbrick.png") ) success = false;
	if(! gfx::loadImage(textures[2], "pics/purplestone.png") ) success = false;
	if(! gfx::loadImage(textures[3], "pics/greystone.png") ) success = false;
	if(! gfx::loadImage(textures[4], "pics/bluestone.png") ) success = false;
	if(! gfx::loadImage(textures[5], "pics/mossy.png") ) success = false;
	if(! gfx::loadImage(textures[6], "pics/wood.png") ) success = false;
	if(! gfx::loadImage(textures[7], "pics/colorstone.png") ) success = false;
	
	if(! gfx::loadImage(textures[8], "pics/barrel.png") ) success = false;
	if(! gfx::loadImage(textures[9], "pics/pillar.png") ) success = false;
	if(! gfx::loadImage(textures[10], "pics/greenlight.png") ) success = false;

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
			Vector2D rayDir = { dir.x + plane.x * cameraX, dir.y + plane.y * cameraX };

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
			if (side == 1) 
				wallX = rayPos.x + ((mapY - rayPos.y + (1 - stepY) / 2) / rayDir.y) * rayDir.x;
			else           
				wallX = rayPos.y + ((mapX - rayPos.x + (1 - stepX) / 2) / rayDir.x) * rayDir.y;
			wallX -= floor((wallX));

			//x coordinate on the texture
			int texX = int(wallX * double(texWidth));
			if(side == 0 && rayDir.x > 0) texX = texWidth - texX - 1;
			if(side == 1 && rayDir.y < 0) texX = texWidth - texX - 1;

			for(int y = drawStart; y < drawEnd; y++)
			{
				int d = y * 256 - HEIGHT * 128 + lineHeight * 128;  //256 and 128 factors to avoid floats
				int texY = ((d * texHeight) / lineHeight) / 256;
				int color = textures[texNum][texHeight * texY + texX];
				//make color darker for y-sides: R, G and B byte each divided through two with a "shift" and an "and"
				if(side == 1) color = (color >> 1) & 8355711;
				buffer[y*WIDTH + x] = color;
				//buffer[x][y] = color;
			}

			      //SET THE ZBUFFER FOR THE SPRITE CASTING
			ZBuffer[x] = perpWallDist; //perpendicular distance is used

			// Floor casting
			Vector2D floorWall = { 0, 0};

			// 4 Wall projection posible 
			if ( side == 0 && rayDir.x > 0)
			{
				floorWall.x = mapX;
				floorWall.y = mapY + wallX ;
			}
			else if ( side == 0 && rayDir.x < 0)
			{
				floorWall.x = mapX + 1.0;
				floorWall.y = mapY + wallX ;
			}
			else if ( side == 1 && rayDir.y > 0)
			{
				floorWall.x = mapX + wallX;
				floorWall.y = mapY;
			}
			else
			{
				floorWall.x = mapX + wallX;
				floorWall.y = mapY + 1.0;
			}

			double distWall = perpWallDist, distPlayer = 0, currentDist = 0;

			if ( drawEnd < 0 ) drawEnd = HEIGHT; // becomes < 0 when the integer Overflows
			// Draw the floor from drawEnd to the bottom of the Screen
			for ( int y = drawEnd + 1; y < HEIGHT; y++ )
			{
				currentDist = HEIGHT / (2.0 * y - HEIGHT); //We could make a small lookup table for this instead

				double weight = (currentDist - distPlayer) / (distWall - distPlayer);

				Vector2D currentFloor = { weight * floorWall.x + (1.0 - weight) * pos.x, 
										  weight * floorWall.y + (1.0 - weight) * pos.y };

				int floorTexX = int(currentFloor.x * texWidth) % texWidth;
				int floorTexY = int(currentFloor.y * texHeight) % texHeight;

				//int checkerBoardPattern = (int(currentFloor.x) + int(currentFloor.y)) % 2;
				//int floorTexture = 0;
				//if(checkerBoardPattern == 0) floorTexture = 3;
				//else floorTexture = 4;

				//floor
				buffer[y*WIDTH + x] = (textures[3][texWidth * floorTexY + floorTexX] >> 1) & 8355711;
				//ceiling (symmetrical!)
				buffer[(HEIGHT - y) *WIDTH + x] = textures[6][texWidth * floorTexY + floorTexX];
			}
		}

		//SPRITE CASTING
		//sort sprites from far to close
		for(int i = 0; i < numImages; i++)
		{
		  imgOrder[i] = i;
		  imgDistance[i] = ((pos.x - images[i].x) * (pos.x - images[i].x) + (pos.y - images[i].y) * (pos.y - images[i].y)); //sqrt not taken, unneeded
		}
		combSort(imgOrder, imgDistance, numImages);

		   //after sorting the sprites, do the projection and draw them
		for( int i = 0; i < numImages; i++ )
		{
		  //translate sprite position to relative to camera
			Vector2D imgPos = { images[imgOrder[i]].x - pos.x, images[imgOrder[i]].y - pos.y};

		  //transform sprite with the inverse camera matrix
		  // [ planeX   dirX ] -1                                       [ dirY      -dirX ]
		  // [               ]       =  1/(planeX*dirY-dirX*planeY) *   [                 ]
		  // [ planeY   dirY ]                                          [ -planeY  planeX ]

		  double invDet = 1.0 / (plane.x * dir.y - dir.x * plane.y); //required for correct matrix multiplication

		  Vector2D transform = { invDet * (dir.y * imgPos.x - dir.x * imgPos.y), 
									invDet * (-plane.y * imgPos.x + plane.x * imgPos.y) };
		  //this is actually the depth inside the screen, that what Z is in 3D

		  int spriteScreenX = int((WIDTH / 2) * (1 + transform.x / transform.y));

		  //parameters for scaling and moving the sprites
		  #define uDiv 1
		  #define vDiv 1
		  #define vMove 0.0
		  int vMoveScreen = int(vMove / transform.y);

		  //calculate height of the sprite on screen
		  int spriteHeight = abs(int(HEIGHT / (transform.y))) / vDiv; //using "transformY" instead of the real distance prevents fisheye
		  //calculate lowest and highest pixel to fill in current stripe
		  int drawStartY = -spriteHeight / 2 + HEIGHT / 2 + vMoveScreen;
		  if(drawStartY < 0) drawStartY = 0;
		  int drawEndY = spriteHeight / 2 + HEIGHT / 2 + vMoveScreen;
		  if(drawEndY >= HEIGHT) drawEndY = HEIGHT - 1;

		  //calculate width of the sprite
		  int spriteWidth = abs( int (HEIGHT / (transform.y))) / uDiv;
		  int drawStartX = -spriteWidth / 2 + spriteScreenX;
		  if(drawStartX < 0) drawStartX = 0;
		  int drawEndX = spriteWidth / 2 + spriteScreenX;
		  if(drawEndX >= WIDTH) drawEndX = WIDTH - 1;

		  //loop through every vertical stripe of the sprite on screen
		  for(int stripe = drawStartX; stripe < drawEndX; stripe++)
		  {
			int texX = int(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * texWidth / spriteWidth) / 256;
			//the conditions in the if are:
			//1) it's in front of camera plane so you don't see things behind you
			//2) it's on the screen (left)
			//3) it's on the screen (right)
			//4) ZBuffer, with perpendicular distance
			if(transform.y > 0 && stripe > 0 && stripe < WIDTH && transform.y < ZBuffer[stripe])
			for(int y = drawStartY; y < drawEndY; y++) //for every pixel of the current stripe
			{
			  int d = (y-vMoveScreen) * 256 - HEIGHT * 128 + spriteHeight * 128; //256 and 128 factors to avoid floats
			  int texY = ((d * texHeight) / spriteHeight) / 256;
			  Uint32 color = textures[images[imgOrder[i]].texture][texWidth * texY + texX]; //get current color from the texture
			  //if((color & 0x00FFFFFF) != 0) buffer[y + stripe * HEIGHT] = color; //paint pixel if it isn't black, black is the invisible color
			  if((color & 0x00FFFFFF) != 0) buffer[y * WIDTH+ stripe] = color; //paint pixel if it isn't black, black is the invisible color
			}
		  }
		}



		//gfx::drawBuffer(buffer[0]);
		gfx::BufferDraw(buffer);
 

		for(int x = 0; x < WIDTH; x++) 
			for(int y = 0; y < HEIGHT; y++) 
				buffer[y*WIDTH + x] = 0;
				//buffer[x][y] = 0; //clear the buffer instead of cls()

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

// Get the horizontal and vertical screen sizes in pixel

//sort algorithm
void combSort(int* order, double* dist, int amount)
{
  int gap = amount;
  bool swapped = false;
  while(gap > 1 || swapped)
  {
    //shrink factor 1.3
    gap = (gap * 10) / 13;
    if(gap == 9 || gap == 10) gap = 11;
    if (gap < 1) gap = 1;
    swapped = false;
    for (int i = 0; i < amount - gap; i++)
    {
      int j = i + gap;
      if (dist[i] < dist[j])
      {
        std::swap(dist[i], dist[j]);
        std::swap(order[i], order[j]);
        swapped = true;
      }
    }
  }
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