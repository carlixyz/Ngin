

//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

//
// TinyPTC PLASMA
// Joncom2000 of TTD
//
// A little example of plotting some stars

#include "tinyptc.h"
#include <stdlib.h>
#include <math.h>
#include <iostream>


// Setuo screen
#define SCR_WIDTH 640
#define SCR_HEIGHT 480
#define SCR_SIZE SCR_WIDTH*SCR_HEIGHT
#define PI 3.141593f


struct RGB{
	BYTE r ;
	BYTE g;
	BYTE b;
	BYTE a;
}; 

void pCopy( int dest[], int source[] );

// Setup a buffer for our reading and writing   
static int Buffer[SCR_SIZE];
static int Buffer2[SCR_SIZE];

int Lsin1[2048];
//int *pSin1 = &Lsin1[1024];
int Lsin2[2048];
//int *pSin2 = &Lsin2[1024];
int Lsin3[2048];
//int *pSin3 = &Lsin3[1024];

RGB Lcols[256];

int main()
{
	int		*p		= 0;
	int		*p2		= 0;

	register int		pixel	= 0;
	register int		col		= 0;
	register int		cdir	= 0;
	register int		i		= 0;
	register int		rot		= 0;
	register int		counter = 0;

	register long	ofs		= 0;
	register long	frame	= 0;

	// Open our window/screen
    if (!ptc_open("Plasma2000",SCR_WIDTH,SCR_HEIGHT)) return 1;

	for ( i = 0; i < 2048; i++ )
	{
		Lsin1[i] = sin( (float)i/64 ) * 56 ;     //Play with these values
        Lsin2[i] = sin( (float)i/164 ) * 128 ;     //for different types of fx
        Lsin3[i] = sin( (float)i/32 ) * 64  ;      //;*)
	}

	for ( i = 0; i < 256; i++ )					// MAKE FLOW PLASMA THING
	{
        Lcols[i].r = abs( (int)(128 - 127 * sin(i * PI / 32)));
	    Lcols[i].g = abs( (int)(128 - 127 * sin(i * PI / 64)));
	    Lcols[i].b = abs( (int)(128 - 127 * sin(i * PI / 128)));
	}

	p = &Buffer2[0];
      for (int By = 0; By < SCR_HEIGHT; By++)		// MAKE BLUE CHECKER BOXES BACKGROUND
	  {
		  for (int Bx = 0; Bx < SCR_WIDTH; Bx++)
		  {
			*p =  (Bx ^ By) ;
			p++;
		  }
	  }

	counter = 0;
    cdir	= 1;
	int sp = 0;
	int dp = 0;

	while(true)
	{
		counter += cdir;

		if( (counter < 1) || (counter > 2048) ) 
         cdir = -cdir;

		p = Buffer;
		p2 = Buffer2;

		pCopy( Buffer, Buffer2);

      for (int y = 0; y < SCR_HEIGHT; y++)
	  {
		  for (int x = 0; x < SCR_WIDTH; x++)
		  {
			col = (Lsin3[x]  + Lsin1[x+counter] + Lsin2[y]) & 255;
            sp = (Lcols[col].r << 16) | (Lcols[col].g << 8) | Lcols[col].b;
            dp = *p2;
			DWORD r = dp >> 16;
			DWORD g = (dp >> 8) & 255;
			DWORD b = dp & 255;
			r = (Lcols[col].r + r) >> 1;
			g = (Lcols[col].g + g) >> 1;
			b = (Lcols[col].b + b) >> 1;
			*p = (r << 16) | (g << 8) | b;
			p++;
			p2++;

		  }
	  }
		ptc_update( Buffer );
	}
	ptc_close();
}


void pCopy (  int  dest[], int source[] ){    
    for( long offset = 0; offset < SCR_SIZE; offset++)
	{
       Buffer[ offset ] = Buffer2[ offset ];

	}
}




//-----------------------------------------------------------------
// Our Functions
//-----------------------------------------------------------------
//void Cls(int rgb)
//{
//	int x;
//	int y;
//	//Do cls stuff
//	for (y=0; y<SCR_HEIGHT; y++)
//	{
//		for (x=0; x<SCR_WIDTH; x++)
//		{
//           Buffer[(y*SCR_WIDTH+x)] =rgb;
//		}
//	}
//}
//
//void WritePixel(int x,int y,int rgb)
//{
//	//Do write pixel
//	if(x>-1)
//	{
//		if(x<SCR_WIDTH)
//		{
//			if(y>-1)
//			{
//				if(y<SCR_HEIGHT)
//				{
//					Buffer[y*SCR_WIDTH+x] =rgb;
//				}
//			}
//		}
//	}
//}
















////#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
//
////
//// TinyPTC PLASMA
//// Joncom2000 of TTD
////
//// A little example of plotting some stars
//
//#include "tinyptc.h"
//#include <stdlib.h>
//#include <math.h>
//#include <iostream>
//
//
//// Setuo screen
//#define SCR_WIDTH 640
//#define SCR_HEIGHT 480
//#define SCR_SIZE SCR_WIDTH*SCR_HEIGHT
//#define PI 3.141593f
//
//
//struct RGB{
//	BYTE r ;
//	BYTE g;
//	BYTE b;
//	BYTE a;
//}; 
//
//void pCopy( int dest[], int source[] );
//
//// Setup a buffer for our reading and writing   
//static int Buffer[SCR_SIZE];
//static int Buffer2[SCR_SIZE];
//
//int Lsin1[2048];
//int *pSin1 = &Lsin1[1024];
//int Lsin2[2048];
//int *pSin2 = &Lsin2[1024];
//int Lsin3[2048];
//int *pSin3 = &Lsin3[1024];
//
//RGB Lcols[256];
//
//int main()
//{
//	int		*p		= 0;
//	int		*p2		= 0;
//
//	register int		pixel	= 0;
//	register int		col		= 0;
//	register int		cdir	= 0;
//	register int		i		= 0;
//	register int		rot		= 0;
//	register int		counter = 0;
//
//	register long	ofs		= 0;
//	register long	frame	= 0;
//
//	// Open our window/screen
//    if (!ptc_open("Plasma2000",SCR_WIDTH,SCR_HEIGHT)) return 1;
//
//	for ( i = -1024; i < 1024; i++ )
//	{
//		pSin1[i] = sin( i/64.0f) * 56 ;     //Play with these values
//        pSin2[i] = sin( i/164.0f ) * 128 ;     //for different types of fx
//        pSin3[i] = sin( i/32.0f ) * 64  ;      //;*)
//	}
//
//	for ( i = 0; i < 256; i++ )					// MAKE FLOW PLASMA THING
//	{
//        Lcols[i].r = (int)abs( (128 - 127 * sin(i * PI / 32)));
//	    Lcols[i].g = (int)abs( (128 - 127 * sin(i * PI / 64)));
//	    Lcols[i].b = (int)abs( (128 - 127 * sin(i * PI / 128)));
//	}
//
//	p = &Buffer2[0];
//      for (int y = 0; y < SCR_HEIGHT; y++)		// MAKE BLUE CHECKER BOXES BACKGROUND
//	  {
//		  for (int x = 0; x < SCR_WIDTH; x++)
//		  {
//			*p =  (x ^ y) ;
//			p++;
//		  }
//	  }
//
//	counter = 0;
//    cdir	= 1;
//	int sp = 0;
//	int dp = 0;
//
//	while(true)
//	{
//		counter += cdir;
//		if( (counter < 1) || (counter > 4096) ) 
//         cdir = -cdir;
//
//		p = Buffer;
//		p2 = Buffer2;
//
//		pCopy( Buffer, Buffer2);
//
//      for (int y = 0; y <= SCR_HEIGHT-1; y++)
//	  {
//		  for (int x = 0; x <= SCR_WIDTH-1; x++)
//		  {
//			col = (pSin3[x]  + pSin1[x + counter] + pSin2[y]) & 255;
//            sp = (Lcols[col].r << 16) | (Lcols[col].g << 8) | Lcols[col].b;
//            dp = *p2;
//			int r = dp >> 16;
//			int g = (dp >> 8) & 255;
//			int b = dp & 255;
//			r = (Lcols[col].r + r) >> 1;
//			g = (Lcols[col].g + g) >> 1;
//			b = (Lcols[col].b + b) >> 1;
//			*p = (r << 16) | (g << 8) | b;
//			p++;
//			p2++;
//		  }
//	  }
//		ptc_update( Buffer );
//	}
//	ptc_close();
//}
//
//
//void pCopy (  int  dest[], int source[] ){    
//    for( long offset = 0; offset <= SCR_SIZE -1; offset++)
//	{
//       Buffer[ offset ] = Buffer2[ offset ];
//	}
//}
//
//
//
//
////-----------------------------------------------------------------
//// Our Functions
////-----------------------------------------------------------------
////void Cls(int rgb)
////{
////	int x;
////	int y;
////	//Do cls stuff
////	for (y=0; y<SCR_HEIGHT; y++)
////	{
////		for (x=0; x<SCR_WIDTH; x++)
////		{
////           Buffer[(y*SCR_WIDTH+x)] =rgb;
////		}
////	}
////}
////
////void WritePixel(int x,int y,int rgb)
////{
////	//Do write pixel
////	if(x>-1)
////	{
////		if(x<SCR_WIDTH)
////		{
////			if(y>-1)
////			{
////				if(y<SCR_HEIGHT)
////				{
////					Buffer[y*SCR_WIDTH+x] =rgb;
////				}
////			}
////		}
////	}
////}
//
//
//
//
//
//
//
//
//
//
//
//
//
//
