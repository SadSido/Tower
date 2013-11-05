// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: map for variant types, useful for quick in-game parameters editing

#ifndef _Databags_h_
#define _Databags_h_

#include <ClanLib/core.h>
#include <map>

//************************************************************************************************************************

class Databag
{
public:
	typedef std::shared_ptr<Databag> Ref;
};

//************************************************************************************************************************

struct Databags : public std::map<CL_String, Databag::Ref>
{
};

//************************************************************************************************************************

#endif
