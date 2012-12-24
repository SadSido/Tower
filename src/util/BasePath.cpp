// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: proxy for fast switching between the resource folders

#include "BasePath.h"

//************************************************************************************************************************

CL_String makePath(const CL_String &path)
{
	static CL_String basePath = CL_File::read_text("basepath");
	return basePath + path;
}

//************************************************************************************************************************
