// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: class for passing data through the objects in level scene

#include "Globals.h"

//************************************************************************************************************************

Globals::Globals()
: m_gen(1)
{}

// variables query:

bool Globals::check(const CL_String &var) const
{
	return m_vars.find(var) != m_vars.end();
}

long Globals::getGen() const
{
	return m_gen;
}

// modifications:

void Globals::add(const CL_String &var)
{
	++ m_gen;
	m_vars.insert(var);
}

void Globals::del(const CL_String &var)
{
	++ m_gen;
	m_vars.erase(var);
}

// list of some popular globals:

CL_String Globals::victory()
{ return "general:victory"; }

CL_String Globals::defeat()
{ return "general:defeat"; }

//************************************************************************************************************************
