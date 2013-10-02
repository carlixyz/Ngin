//Plasma using y*xwidth +x addresing
//Relsoft 2004 - Sunhouse 2012

#include "tinyptc.h"
#include <stdlib.h>
#include <iostream>
#include <math.h>

 //Setup screen
#define SCR_WIDTH 640
#define SCR_HEIGHT 480
#define SCR_SIZE SCR_WIDTH*SCR_HEIGHT

#define PI 3.141593f
#define rnd(n) (rand()%(n+1))
   
static int buffer[SCR_SIZE];

int Lsin1[2048];
int *pSin1 = &Lsin1[1024];
int Lsin2[2048];
int *pSin2 = &Lsin2[1024];
int Lsin3[2048];
int *pSin3 = &Lsin3[1024];
int Lcols[3][256];

int				col		= 0;
int				i		= 0;
int				rot		= 0;
int				pixel	= 0;
long			offs	= 0;
long			frame	= 0;
long			counter = 0;

int main()
{
    if (!ptc_open("plasma test",SCR_WIDTH,SCR_HEIGHT)) return 1;




	for ( i = -1024; i <= 1024; i++ )
	{
		pSin1[i] = sin( i/128.f) * 256 ;     //Play with these values
        pSin2[i] = sin( i/64.f ) * 128 ;     //for different types of fx
        pSin3[i] = sin( i/32.f ) * 64  ;      //;*)

		//if ( i%256 == 0) std::cout << " Value: " << i << std::endl;
	}

	for ( i = 0; i <= 255; i++ )
	{
        Lcols[0][i]= abs( (int)(128 - 127 * sin(i * PI / 32)));
	    Lcols[1][i]= abs( (int)(128 - 127 * sin(i * PI / 64)));
	    Lcols[2][i]= abs( (int)(128 - 127 * sin(i * PI / 128)));
	}

    do
	{
      counter++;
      rot = 64 * ( ( ( counter & 1 ) == 1) | 1);
	  offs = 0;
      for (int y = 0; y < SCR_HEIGHT-1; y++)
	  {
		  for (int x = 0; x < SCR_WIDTH-1; x++)
		  {
              rot = -rot;
              col = ( pSin3[x + rot] + pSin1[x + rot + counter] + pSin2[y + rot]) & 255;
              pixel = ((Lcols[0][col] << 16) |  (Lcols[1][col] << 8) | Lcols[2][col]);
              buffer[ offs ] = pixel;
			  offs++;
		  }
		  offs++;

	  }
    	ptc_update( buffer );
	}
    while( offs < SCR_SIZE );


	ptc_close();
	
}
