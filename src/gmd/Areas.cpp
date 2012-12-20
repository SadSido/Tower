// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: minor sections of every level

#include "Areas.h"
#include "EntTest.h"

//************************************************************************************************************************

Area::Area()
{}

Area::Area(CL_Sizef window)
{
	// generate tilemap:
	m_tilemap = Tilemap::Ref(new Tilemap(20,20));
	m_tilemap->window(window);

	// generate entities:
	m_entities = Entities::Ref(new Entities());

	for (size_t no = 0; no < 10; ++ no)
	{ m_entities->push_back(Entity::Ref(new EntTest(CL_Pointf(5.0f,1.0f), CL_Sizef(1.0f, 1.0f)))); }
}

//************************************************************************************************************************
