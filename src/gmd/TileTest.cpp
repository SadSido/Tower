// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: the very first scene in the game

#include "TileTest.h"
#include "Player.h"
#include "../sys/GameManager.h"
#include "../sys/Renderer.h"

//************************************************************************************************************************

TileTestScene::TileTestScene(GameManager * manager)
: GameScene(manager), m_map(20,20), m_player(new Player(CL_Pointf(8.0f, 10.0f), CL_Sizef(0.8f, 1.6f)))
{
	Configuration::Ref config = m_manager->getConfig();
	Renderer::Ref renderer = m_manager->getRenderer();

	// load up necessary resources:
	CL_String resPath = config->getPath("intro.xml");
	CL_ResourceManager resMan = CL_ResourceManager(resPath);

	m_brick = CL_Sprite(renderer->getGC(), "brick", &resMan);
	m_brickbg = CL_Sprite(renderer->getGC(), "brickbg", &resMan);

	// init some shit for the map:
	m_map.window(m_manager->getRenderer()->getGC().get_size());
}

void TileTestScene::update(unsigned int msecs)
{
	if (!msecs) return;

	CL_Size tileSize = m_brick.get_size();
	CL_Size wndSize  = m_manager->getRenderer()->getGC().get_size();

	CL_InputDevice  &keys   = m_manager->getRenderer()->getIC().get_keyboard();
	CL_InputDevice  &mouse  = m_manager->getRenderer()->getIC().get_mouse();

	// update player:
	UpdateCtx ctx = { keys, mouse, m_map, m_globals, m_entities };
	m_player->update(ctx, msecs);

	// update the entities:
	for (auto it = m_entities.begin(); it != m_entities.end(); /**/)
	{
		auto cur = it ++;
		bool res = (*cur)->update(ctx, msecs);

		// drop the entity, which returned false:
		if (!res) { m_entities.erase(cur); }
	}

	// update camera:
	m_map.offset(m_player->getRect().get_center());

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
	Renderer::Ref renderer = m_manager->getRenderer();
	
	// render map:
	m_map.render(renderer->getGC(), m_brick);

	// render player:
	RenderCtx ctx = { renderer->getGC(), m_map };
	m_player->render(ctx);

	// render entities:
	for (auto it = m_entities.begin(); it != m_entities.end(); ++ it)
	{ (*it)->render(ctx); }
}

//************************************************************************************************************************
