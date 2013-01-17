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
	explicit Globals();

	// variable query:
	bool check(CL_String var) const;
	long getGen() const;

	// modifications:
	void add(CL_String var);
	void del(CL_String var);

public:
	// some popular globals:
	static CL_String victory();
	static CL_String defeat();

private:
	long m_gen;
	std::set<CL_String> m_vars;
};

//************************************************************************************************************************

#endif
