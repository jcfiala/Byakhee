#ifndef __COLOURCONVERSION_H__
#define __COLOURCONVERSION_H__

#pragma once

inline COLORREF HEXTOCOLORREF( unsigned int hex )
{
	return ( (hex&0xFF0000) >> 0x10 ) |
		     (hex&0x00FF00) |
		   ( (hex&0x0000FF) << 0x10 );

}

inline unsigned int COLORREFTOHEX( COLORREF cr )
{
	return ( (cr&0xFF0000) >> 0x10 ) |
		     (cr&0x00FF00) |
		   ( (cr&0x0000FF) << 0x10 );
}


#endif //__COLOURCONVERSION_H__
