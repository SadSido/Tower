// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: classes for handling combat damage

#include "Hitmap.h"
#include <assert.h>

//************************************************************************************************************************

CL_Rectf Hitmap::getRect(int index) const 
{
	const size_t pos = (size_t)index;
	return (pos < m_rects.size()) ? m_rects.at(pos) : CL_Rectf();
}

//************************************************************************************************************************
