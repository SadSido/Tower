// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: mischellaneous parsing utilities

#ifndef _Parsing_h_
#define _Parsing_h_

#include <ClanLib/core.h>

//************************************************************************************************************************

CL_String parseToken  (CL_String::iterator &it);
CL_String parseQuotes (CL_String::iterator &it);

// for checking the syntax of the script:

void parseAssert(CL_String::iterator &it, const CL_String &value);

//************************************************************************************************************************

#endif
