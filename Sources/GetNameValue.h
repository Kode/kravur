// GetNameValue.h
//
// Copyright (C) 2001 Hans Dietrich
//
// This software is released into the public domain.  
// You are free to use it in any way you like.
//
// This software is provided "as is" with no expressed 
// or implied warranty.  I accept no liability for any 
// damage or loss of business that this software may cause. 
//
///////////////////////////////////////////////////////////////////////////////

#ifndef GETNAMEVALUE_H
#define GETNAMEVALUE_H

LONG GetNameValue(HKEY key, LPSTR subkey, LPSTR valuename, LPSTR retdata);
LONG GetNextNameValue(HKEY key, LPSTR subkey, LPSTR szName, LPSTR szData);

#endif //GETNAMEVALUE_H
