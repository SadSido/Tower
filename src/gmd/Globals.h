// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: class for passing data through the objects in level scene

#ifndef _Globals_h_
#define _Globals_h_

#include <set>
#include <ClanLib/core.h>

//************************************************************************************************************************

class Globals
{
public:
	Globals(CL_Pointf pos, CL_Sizef size);
	~Globals();

	// variable query:
	bool check(CL_String var) const;

	// modifications:
	void add(CL_String var);
	void del(CL_String var);

private:
	std::set<CL_String> m_vars;
};

//************************************************************************************************************************

#endif
