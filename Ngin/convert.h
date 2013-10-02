//
// TinyPTC by Gaffer
// www.gaffer.org/tinyptc
//

#ifndef __TINYPTC_CONVERT_H
#define __TINYPTC_CONVERT_H


// include files
#include "tinyptc.h"

// converter function type
typedef void (*PTC_CONVERTER) (void *src,void *dst,int pixels);

// converter request
PTC_CONVERTER ptc_request_converter(int bits,Uint32 r,Uint32 g,Uint32 b);


#endif
