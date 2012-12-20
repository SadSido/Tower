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
public:
	explicit Area();
	explicit Area(CL_Sizef window);

	// member accessors:

	Entities::Ref getEntities() const
	{ return m_entities; }

	Tilemap::Ref getTilemap() const
	{ return m_tilemap; }

private:
	Entities::Ref m_entities;
	Tilemap::Ref  m_tilemap;
};

//************************************************************************************************************************

typedef std::map<CL_String, Area> Areas;

//************************************************************************************************************************

#endif
