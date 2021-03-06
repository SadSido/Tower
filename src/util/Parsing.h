// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: mischellaneous parsing utilities

#ifndef _Parsing_h_
#define _Parsing_h_

#include <ClanLib/core.h>

//************************************************************************************************************************

CL_String parseToken  (CL_String::const_iterator &it);
CL_String parseQuotes (CL_String::const_iterator &it);
CL_String parseLine   (CL_String::const_iterator &it);

// block data processing:

int  parseInt(CL_String::const_iterator &it);
void parseCSV(CL_String::const_iterator &it, std::vector<int> &res);

// for checking the syntax of the script:

void parseAssert(CL_String::const_iterator &it, const CL_String &value);

//************************************************************************************************************************

#endif
