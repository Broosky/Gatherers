/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program Name: Gatherers (C)                                                                                             //
// Author: Jeffrey Bednar                                                                                                  //
// Copyright (c) Illusion Interactive, 2011 - 2025.                                                                        //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _COMMON_H_
#define _COMMON_H_
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// For MSVC compiler specific annotations (/analyze). Introduced with Visual Studio having much more extensive code analysis
// versus the much older original tooling.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS // stdio.h: Disable warnings for unsafe CSL runtime functions.
#endif
#define SAL_NO_ATTRIBUTE 1
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// App will use floating point where applicable, with precision truncated when calling library functions which is intentional.
//     'function': conversion from 'float' to 'int', possible loss of data
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma warning(disable : 4244)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Keep everything ANSI as it was back in 2011.
//     'function': incompatible types - from 'char [X]' to 'LPCWSTR'
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#undef UNICODE
#undef _UNICODE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
