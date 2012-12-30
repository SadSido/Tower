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
	m_tilemap = Tilemap::Ref(new Tilemap(20, 20, 64));
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

	// load the stuff:
	Tilemap::Ref tilemap = loadTilemap(root);

	// commit result:
	m_tilemap = tilemap;
}

Tilemap::Ref Area::loadTilemap(CL_DomElement &root)
{
	const int width  = root.get_attribute_int("width");
	const int height = root.get_attribute_int("height");
	const int tilesz = root.get_attribute_int("tilewidth");

	Tilemap::Ref result = Tilemap::Ref(new Tilemap(width, height, tilesz));

	// first, we process tilesets to create proxies:

	auto tilesets = root.get_elements_by_tag_name("tileset");
	for (int no = 0; no < tilesets.get_length(); ++ no)
	{
		CL_DomElement tileset = tilesets.item(no).to_element();
		
		const int firstGid = tileset.get_attribute_int("firstgid");
		const CL_String name = tileset.get_attribute("name");

		// how much tiles is in the set?
		auto image = tileset.get_first_child_element();

		const int width  = image.get_attribute_int("width");
		const int height = image.get_attribute_int("height");

		const int count  = width / tilesz * height / tilesz;
		// result->addProxy(name, count);
	}

	// then, we process layers to handle geometry:

	auto layers = root.get_elements_by_tag_name("layer");
	for (int no = 0; no < layers.get_length(); ++ no)
	{
		CL_DomElement layer = layers.item(no).to_element();
		const CL_String name = layer.get_attribute("name");
	}

	return result;
}

//************************************************************************************************************************
