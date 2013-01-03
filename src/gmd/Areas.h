// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: minor sections of every level

#ifndef _Areas_h_
#define _Areas_h_

#include "Tilemap.h"
#include "Entities.h"
#include <ClanLib/core.h>
#include <map>

//************************************************************************************************************************

class Area
{
	typedef std::map<CL_String, CL_Pointf> EntryPoints;

public:
	explicit Area();
	explicit Area(CL_Sizef window, CL_String path, CL_String name);

	// member accessors:

	CL_String getName() const
	{ return m_name; }

	Entities::Ref getEntities() const
	{ return m_entities; }

	Tilemap::Ref getTilemap() const
	{ return m_tilemap; }

	CL_Pointf getEntryPoint(CL_String name) const
	{ return m_entries.find(name)->second; }

private:
	CL_String     m_name;
	Entities::Ref m_entities;
	Tilemap::Ref  m_tilemap;
	EntryPoints   m_entries;

	// loading stuff from xml:

	static Tilemap::Ref  loadTilemap  (CL_DomElement &tmxRoot);
	static Entities::Ref loadEntities (CL_DomElement &tmxRoot);
};

//************************************************************************************************************************

struct Areas : public std::map<CL_String, Area>
{};

//************************************************************************************************************************

#endif
