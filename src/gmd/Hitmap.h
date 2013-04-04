// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: classes for handling combat damage

#ifndef _Hitmap_h_
#define _Hitmap_h_

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <vector>

//************************************************************************************************************************

class Hitmap
{
public:
	explicit Hitmap(const CL_String &id, CL_ResourceManager * resources);
	CL_Rectf getRect(int index) const;

private:
	std::vector<CL_Rectf> m_rects;
};
	
//************************************************************************************************************************

#endif
