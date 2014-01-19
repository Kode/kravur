#ifdef SYS_WINDOWS
// GetFontFile.cpp
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

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include "GetFontFile.h"
#include "GetNameValue.h"

///////////////////////////////////////////////////////////////////////////////
// GetFontFile
//
// Note:  This is *not* a foolproof method for finding the name of a font file.
//        If a font has been installed in a normal manner, and if it is in
//        the Windows "Font" directory, then this method will probably work.
//        It will probably work for most screen fonts and TrueType fonts.
//        However, this method might not work for fonts that are created 
//        or installed dynamically, or that are specific to a particular
//        device, or that are not installed into the font directory.

bool GetFontFile(char* lpszFontName, char** strDisplayName, char** strFontFile)
{
	ASSERT(lpszFontName && lpszFontName[0] != 0);

	char szName[2 * MAX_PATH];
	char szData[2 * MAX_PATH];

	char* strFont = "Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts";

	*strFontFile = ""; //.Empty();

	bool bResult = false;

	while (GetNextNameValue(HKEY_LOCAL_MACHINE, strFont, szName, szData) == ERROR_SUCCESS)
	{
		if (_strnicmp(lpszFontName, szName, strlen(lpszFontName)) == 0)
		{
			TRACE(_T("found font\n"));
			*strDisplayName = szName;
			*strFontFile = szData;
			bResult = true;
			break;
		}

		strFont = ""; //.Empty(); // this will get next value, same key
	}

	GetNextNameValue(HKEY_LOCAL_MACHINE, NULL, NULL, NULL);	// close the registry key

	return bResult;
}
#endif
