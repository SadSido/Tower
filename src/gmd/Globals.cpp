// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: class for passing data through the objects in level scene

#include "Globals.h"

//************************************************************************************************************************

bool Globals::check(CL_String var) const
{
	return m_vars.find(var) != m_vars.end();
}

void Globals::add(CL_String var)
{
	m_vars.insert(var);
}

void Globals::del(CL_String var)
{
	m_vars.erase(var);
}

//************************************************************************************************************************
