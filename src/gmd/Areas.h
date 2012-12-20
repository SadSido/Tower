// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: minor sections of every level

#ifndef _Areas_h_
#define _Areas_h_

#include <ClanLib/core.h>
#include <map>

//************************************************************************************************************************

class Area
{
public:
	explicit Area();

private:
};

//************************************************************************************************************************

typedef std::map<CL_String, Area> Areas;

//************************************************************************************************************************

#endif
