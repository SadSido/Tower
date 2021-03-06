// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: minor sections of every level

#ifndef _Areas_h_
#define _Areas_h_

#include "Tilemap.h"
#include "EntityFactory.h"
#include <ClanLib/core.h>
#include <map>

//************************************************************************************************************************

class Area
{
	// map of entrance points to the area:
	struct EntryMap : public std::map<CL_String, CL_Pointf>
	{ typedef std::shared_ptr<EntryMap> Ref; };

public:
	explicit Area();
	explicit Area(const CL_String &path, const Databags &data, const CL_Sizef &window);

	// member accessors:

	//CL_String getName() const
	//{ return m_name; }

	Entities::Ref getEntities() const
	{ return m_entities; }

	Tilemap::Ref getTilemap() const
	{ return m_tilemap; }

	CL_Pointf getEntryPoint(const CL_String &name) const
	{ return m_entryMap->find(name)->second; }

private:
	Entities::Ref m_entities;
	Tilemap::Ref  m_tilemap;
	EntryMap::Ref m_entryMap;

	// loading stuff from xml:

	static Tilemap::Ref  loadTilemap  (CL_DomElement &tmxRoot);
	static Entities::Ref loadEntities (CL_DomElement &tmxRoot, const Databags &data);
	static EntryMap::Ref loadEntryMap (CL_DomElement &tmxRoot);
};

//************************************************************************************************************************

struct Areas : public std::map<CL_String, Area>
{};

//************************************************************************************************************************

#endif
