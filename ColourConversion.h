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
/**
 * Copyright (C) 1998  David Harvey
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
