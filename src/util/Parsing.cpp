// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: mischellaneous parsing utilities

#include "Parsing.h"

//************************************************************************************************************************

namespace 
{
	const CL_String spaces = " \n\r\t";
	const CL_String digits = "0123456789";
	const CL_String quotes = "'\"";
	const CL_String newln  = "\r\n";

	void skipWhile(CL_String::const_iterator &it, const CL_String &pattern)
	{ 
		while (*it && pattern.find(*it) != (-1)) { ++ it; } 
	}

	void skipUntil(CL_String::const_iterator &it, const CL_String &pattern)
	{ 
		while (*it && pattern.find(*it) == (-1)) { ++ it; } 
	}

	void skipAfter(CL_String::const_iterator &it, const CL_String &pattern)
	{
		skipUntil(it, pattern);
		skipWhile(it, pattern);
	}
}

//************************************************************************************************************************

CL_String parseToken(CL_String::const_iterator &it)
{
	skipWhile(it, spaces);
	auto beg = it;
	skipUntil(it, spaces);
	return CL_String(beg, it - beg);
}

CL_String parseQuotes (CL_String::const_iterator &it)
{
	skipUntil(it, quotes);
	auto beg = it;
	skipAfter(it, quotes);
	return CL_String(beg, it - beg);
}

CL_String parseLine(CL_String::const_iterator &it)
{
	auto beg = it;
	skipUntil(it, newln);
	auto end = it;
	skipWhile(it, newln);
	return CL_String(beg, end - beg);
}

void parseAssert(CL_String::const_iterator &it, const CL_String &value)
{
	CL_String token = parseToken(it);
	// assert(token == value);
}

//************************************************************************************************************************
