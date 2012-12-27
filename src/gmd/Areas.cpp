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

	// generate entry points:
	m_entries["main"] = CL_Pointf(9.0f, 14.0f);
}

Area::Area(CL_Sizef window, CL_String path)
{
	CL_DomDocument doc  = CL_DomDocument(CL_File(path));
	CL_DomElement  root = doc.get_document_element();

	const int width  = root.get_attribute_int("width");
	const int height = root.get_attribute_int("height");


}

//************************************************************************************************************************
