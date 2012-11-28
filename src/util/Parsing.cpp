// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: mischellaneous parsing utilities

#include "Parsing.h"

//************************************************************************************************************************

namespace 
{
	CL_String spaces()
	{ return " \n\r\t";	}

	CL_String digits()
	{ return "0123456789";	}

	void skipWhile(CL_String::iterator &it, CL_String pattern)
	{ 
		while (*it && pattern.find(*it) != (-1)) { ++ it; } 
	}

	void skipUntil(CL_String::iterator &it, CL_String pattern)
	{ 
		while (*it && pattern.find(*it) == (-1)) { ++ it; } 
	}
}

//************************************************************************************************************************

CL_String parseToken(CL_String::iterator &it)
{
	skipWhile(it, spaces());
	auto beg = it;
	skipUntil(it, spaces());
	return CL_String(beg, it - beg);
}

//************************************************************************************************************************
