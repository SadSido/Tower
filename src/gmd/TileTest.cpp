// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: the very first scene in the game

#include "TileTest.h"
#include "Player.h"
#include "EntTest.h"
#include "../sys/GameManager.h"
#include "../sys/Renderer.h"

//************************************************************************************************************************

TileTestScene::TileTestScene(GameManager * manager)
: GameScene(manager), m_player(CL_Pointf(8.0f, 10.0f), CL_Sizef(0.8f, 1.6f))
{
	Configuration::Ref config = m_manager->getConfig();
	Renderer::Ref renderer = m_manager->getRenderer();

	// load up necessary resources:
	CL_String resPath = config->getPath("intro.xml");
	CL_ResourceManager resMan = CL_ResourceManager(resPath);

	m_brick = CL_Sprite(renderer->getGC(), "brick", &resMan);
	m_brickbg = CL_Sprite(renderer->getGC(), "brickbg", &resMan);

	// init scene areas:
	CL_Sizef window = renderer->getGC().get_size();

	m_areas["main"]  = Area(window);
	// ...

	enterArea("main");
}

// scene lifecycle:

void TileTestScene::update(unsigned int msecs)
{
	if (!msecs) return;

	// create update context:
	CL_InputDevice  &keys   = m_manager->getRenderer()->getIC().get_keyboard();
	CL_InputDevice  &mouse  = m_manager->getRenderer()->getIC().get_mouse();

	UpdateCtx ctx = { keys, mouse, m_entities, m_tilemap, m_globals, m_player };

	// update player:
	m_player.update(ctx, msecs);

	// update the entities:
	for (auto it = ctx.entities->begin(); it != ctx.entities->end(); /**/)
	{
		auto cur = it ++;
		bool res = (*cur)->update(ctx, msecs);

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

void TileTestScene::render()
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

void TileTestScene::enterArea(CL_String name)
{
	auto it = m_areas.find(name);
	// assert(it != m_areas.end());

	m_entities = it->second.getEntities();
	m_tilemap  = it->second.getTilemap();
}

//************************************************************************************************************************
