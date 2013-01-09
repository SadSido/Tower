// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: minor sections of every level

#include "Areas.h"
#include "../util/Parsing.h"

//************************************************************************************************************************

// c-tors and d-tors:

Area::Area()
{}

Area::Area(CL_Sizef window, CL_String path, CL_String name)
: m_name(name)
{
	CL_DomDocument doc  = CL_DomDocument(CL_File(path));
	CL_DomElement  root = doc.get_document_element();

	// load the tilemap:
	m_tilemap = loadTilemap(root);
	m_tilemap->window(window);

	// load the entities:
	m_entities = loadEntities(root);

	// generate entry points:
	m_entryMap = loadEntryMap(root);
}

// loading tmx:

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

		TileDesc desc = { flags, backData[no], foreData[no] };
		result->pushDesc(desc);
	}

	return result;
}

Entities::Ref Area::loadEntities(CL_DomElement &root)
{
	const float tilesz = root.get_attribute_float("tilewidth");
	Entities::Ref result = Entities::Ref(new Entities());

	auto groups = root.get_elements_by_tag_name("objectgroup");
	for (int no = 0; no < groups.get_length(); ++ no)
	{
		CL_DomElement group  = groups.item(no).to_element(); 
		if (group.get_attribute("name") != "entities") continue;

		// resolve all object entries:
		auto objects = group.get_elements_by_tag_name("object");

		for (int obNo = 0; obNo < objects.get_length(); ++ obNo)
		{
			CL_DomElement object = objects.item(obNo).to_element();
			result->push_back(createEntity(object, tilesz));
		}
	}

	return result;
}

Area::EntryMap::Ref Area::loadEntryMap(CL_DomElement &root)
{
	const float tilesz = root.get_attribute_float("tilewidth");
	EntryMap::Ref result = EntryMap::Ref(new EntryMap());

	auto groups = root.get_elements_by_tag_name("objectgroup");
	for (int no = 0; no < groups.get_length(); ++ no)
	{
		CL_DomElement group  = groups.item(no).to_element(); 
		if (group.get_attribute("name") != "entries") continue;

		// resolve all object entries:
		auto objects = group.get_elements_by_tag_name("object");
		for (int obNo = 0; obNo < objects.get_length(); ++ obNo)
		{
			CL_DomElement object = objects.item(obNo).to_element();

			const CL_String name = object.get_attribute("name");
			const CL_Pointf exit = CL_Pointf(object.get_attribute_float("x"), object.get_attribute_float("y")) / tilesz;

			result->operator[](name) = exit;
		}
	}

	return result;
}

//************************************************************************************************************************
