// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: minor sections of every level

#include "Areas.h"
#include "EntTest.h"
#include "../util/Parsing.h"

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

Area::Area(CL_Sizef window, CL_String path, CL_String name)
: m_name(name)
{
	CL_DomDocument doc  = CL_DomDocument(CL_File(path));
	CL_DomElement  root = doc.get_document_element();

	// load the tilemap:
	m_tilemap = loadTilemap(root);
	m_tilemap->window(window);

	// load the entities:
	m_entities = Entities::Ref(new Entities());

	// generate entry points:
	m_entries["main"] = CL_Pointf(9.0f, 14.0f);
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
		result->pushProxy(name, count);
	}

	// then, we fetch layer data:

	std::vector<int> backData;
	std::vector<int> foreData;
	std::vector<int> blockFlags;

	auto layers = root.get_elements_by_tag_name("layer");
	for (int no = 0; no < layers.get_length(); ++ no)
	{
		CL_DomElement layer  = layers.item(no).to_element();
		const CL_String name = layer.get_attribute("name");
		const CL_String data = layer.get_child_string("data");
		
		auto it = data.begin();
		if (name == "background")
		{ parseCSV(it, backData); }

		else if (name == "foreground")
		{ parseCSV(it, foreData); }

		else if (name == "blockFlags")
		{ parseCSV(it, blockFlags); }
	}

	// finally, we assemble layer data into tile descriptors:

	for (size_t no = 0; no < blockFlags.size(); ++ no)
	{
		int flags = 0;

		if (blockFlags[no]) flags |= tf_Blocking;
		// if (ladderFlags[no]) flags |= tf_Ladder;

		TileDesc desc = { flags, backData[no] - 1, 0 };
		result->pushDesc(desc);
	}

	return result;
}

//************************************************************************************************************************
