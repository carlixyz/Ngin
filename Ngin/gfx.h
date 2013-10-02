#ifndef _GFX_H_
#define _GFX_H_

//#define _CRT_SECURE_NO_WARNINGS
//#include "Mad.cpp"
#include <string>
#include <iostream>
#include <vector>


// and link the libs!
#pragma comment( lib, "corona.lib" )

#include "corona.h"

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
DWORD screen[320*240];

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
	sprite.data=(DWORD*)malloc(width*height*sizeof(DWORD));

	BYTE* Pix = (BYTE*)img->getPixels();
	//

	for (int i = 0; i < width * height; ++i) 
	{

		DWORD pixel=0xFF000000;

		pixel|= *Pix++ <<16;
		pixel|= *Pix++ <<8;
		pixel|= *Pix++;

		//byte red	= *Pix++;
		//byte green= *Pix++;
		//byte blue	= *Pix++;

		//pixel|= red <<16;
		//pixel|= green <<8;
		//pixel|= blue;

		//if( red ==255 && green ==240 &&  blue ==200)
		//	pixel=0x00FFF0C8;
		//  sprite.data[i]=pixel;


		//if((int)GetByte(pixel, 2)==255 && (int)GetByte(pixel, 1)==240 && (int)GetByte(pixel, 0)==200)
			
		unsigned char *bytes = (unsigned char*)&pixel; // Let's try to explain this:
		// -- get pixel first byte pointer and set in bytes.. then travel it around the next R G B colors

		if	(  255 == (int)bytes[2] 
			&& 240 == (int)bytes[1] 
			&& 200 == (int)bytes[0] )
			pixel=0x00FFF0C8;

		sprite.data[i]=pixel;
	}

	GCAdd(sprite.data, GCMALLOC);
	return true;

#pragma region oldCode For LibPNG That Never Worked  
	//// Open a file
	//FILE *fp = fopen(filename, "rb");
	//if(!fp )
	//	return false;

	//png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	//if(!png_ptr)
	//	return false;

	//png_infop info_ptr = png_create_info_struct(png_ptr);
	//if(!info_ptr)
	//{
	//	png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
	//	return false;
	//}
	//png_init_io(png_ptr, fp);


	//png_read_info(png_ptr, info_ptr);
	//png_read_update_info(png_ptr, info_ptr);
	//// Read Info and update info

	//// get width, height and channels
	//int width=info_ptr->width;
	//int height=info_ptr->height;
	//int channels=info_ptr->channels;

	//sprite.width=width;
	//sprite.height=height;
	//BYTE *rgb_image=(BYTE*)malloc(width*height*channels);

	//// Prepare sprite data memory allocation
	//sprite.data=(DWORD*)malloc(width*height*sizeof(DWORD));

	////and this.. I am not sure, It takes a row pointer & use it to fill a row memory area
	//png_byte **row_pointers=(png_byte**)malloc(height*sizeof(png_byte*));

	//// then it travels trough height and use the rgb_image sized plus each row product width and channels (¡?)
	//for(int i=0;i<height;i++)
	//	row_pointers[i]=(png_byte*)(rgb_image+i*width*channels);

	//// Duh, it seems we do that in order to fullfill this functions
	//png_set_rows(png_ptr, info_ptr, (png_bytepp)(&row_pointers));
	//png_read_image(png_ptr, row_pointers);
	//png_read_end(png_ptr, NULL);
	//png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
	//fclose(fp);
	//free(row_pointers);

	//// so, then we keep rgb_image, width, height & channels in order to fullfill the pixel data info inside data
	//for(int y=0;y<height;y++)
	//{
	//	int si=y*width*channels;
	//	int di=y*width;
	//	for(int x=0;x<width;x++)
	//	{
	//		DWORD pixel=0xFF000000;
	//		pixel|=rgb_image[si+0]<<16;
	//		pixel|=rgb_image[si+1]<<8;
	//		pixel|=rgb_image[si+2];
	//		if(rgb_image[si+0]==255 &&
	//		   rgb_image[si+1]==240 &&
	//		   rgb_image[si+2]==200)
	//			pixel=0x00FFF0C8;
	//		sprite.data[di++]=pixel;
	//		si+=channels;
	//	}
	//}
	//free(rgb_image);
	//GCAdd(sprite.data, GCMALLOC);
	//return true;

#pragma endregion

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

}; // end GFx namespace


#endif

