// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: base scene for every game level

#include "LevelScene.h"
#include "Player.h"
#include "EntTest.h"
#include "../sys/GameManager.h"
#include "../sys/Renderer.h"
#include "../util/Parsing.h"
#include "../util/BasePath.h"

//************************************************************************************************************************

LevelScene::LevelScene(GameManager * manager, CL_String descFile)
: GameScene(manager), m_player(CL_Pointf(), CL_Sizef(0.8f, 1.6f))
{
	Configuration::Ref config = m_manager->getConfig();
	Renderer::Ref renderer = m_manager->getRenderer();

	// prepare desc file:
	CL_String descPath = makePath(descFile);
	CL_String descStr  = CL_File::read_text(descPath);
	
	// init stuff from desc file:
	loadDescFile(descStr.begin());


	
	
	// shitty debug stuff:
	m_brick = CL_Sprite(renderer->getGC(), "brick", &m_assets);
	CL_Sizef window = renderer->getGC().get_size();

	m_areas["main"]  = Area(window);
	// ...

	enterArea("main", "main");
}

// scene lifecycle:

void LevelScene::update(float secs)
{
	// create update context:
	CL_InputDevice  &keys   = m_manager->getRenderer()->getIC().get_keyboard();
	CL_InputDevice  &mouse  = m_manager->getRenderer()->getIC().get_mouse();

	UpdateCtx ctx = { keys, mouse, m_entities, m_tilemap, m_globals, m_player };

	// update player:
	m_player.update(ctx, secs);

	// update the entities:
	for (auto it = ctx.entities->begin(); it != ctx.entities->end(); /**/)
	{
		auto cur = it ++;
		bool res = (*cur)->update(ctx, secs);

		// drop the entity, which returned false:
		if (!res) { ctx.entities->erase(cur); }
	}

	// update camera:
	ctx.tilemap->offset(m_player.getRect().get_center());

	// check the globals:
	if (m_globals.check(Globals::victory()))
	{
		m_manager->popScene();
	}
	if (m_globals.check(Globals::defeat()))
	{
		m_manager->popScene();
	}
}

void LevelScene::render()
{
	// create render context:
	Renderer::Ref renderer = m_manager->getRenderer();
	RenderCtx ctx = { renderer->getGC(), m_entities, m_tilemap };
	
	// render map:
	ctx.tilemap->render(renderer->getGC(), m_brick);

	// render player:
	m_player.render(ctx);

	// render entities:
	for (auto it = ctx.entities->begin(); it != ctx.entities->end(); ++ it)
	{ (*it)->render(ctx); }
}

// areas management:

void LevelScene::enterArea(CL_String name, CL_String entry)
{
	auto it = m_areas.find(name);
	// assert(it != m_areas.end());

	// take entities & tilemap into processing:
	m_entities = it->second.getEntities();
	m_tilemap  = it->second.getTilemap();

	// update player position in the new area:
	m_player.setPos(it->second.getEntryPoint(entry));
	m_player.setVel(CL_Pointf());
}

// pasing and initialization stuff:

void LevelScene::loadDescFile(CL_String::const_iterator it)
{
	while (*it)
	{
		// load stuff depending on token:
		CL_String token = parseToken(it);

		if (token == "resource")
		{ loadResource(it); }

		if (token == "area")
		{ loadAreaFile(it); }

		else
		{ /* assert false; */ }
	}
}

void LevelScene::loadResource(CL_String::const_iterator &it)
{
	CL_String path = parseQuotes(it);
	m_assets.add_resources(CL_ResourceManager(makePath(path)));
}

void LevelScene::loadAreaFile(CL_String::const_iterator &it)
{
	// get window size:
	Renderer::Ref renderer = m_manager->getRenderer();
	CL_Sizef window = renderer->getGC().get_size();

	CL_String path = parseQuotes(it);
	Area area = Area(window, makePath(path));
	// m_areas[area->getName()] = area;
}

//************************************************************************************************************************
