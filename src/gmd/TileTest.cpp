// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: the very first scene in the game

#include "TileTest.h"
#include "Player.h"
#include "../sys/GameManager.h"
#include "../sys/Renderer.h"

//************************************************************************************************************************

TileTestScene::TileTestScene(GameManager * manager)
: GameScene(manager), m_map(500,500), m_offX(0), m_offY(0), m_player(new Player(CL_Pointf(8.0f, 10.0f), CL_Sizef(0.8f, 1.6f)))
{
	Configuration::Ref config = m_manager->getConfig();
	Renderer::Ref renderer = m_manager->getRenderer();

	// load up necessary resources:
	CL_String resPath = config->getPath("intro.xml");
	CL_ResourceManager resMan = CL_ResourceManager(resPath);

	m_brick = CL_Sprite(renderer->getGC(), "brick", &resMan);
	m_brickbg = CL_Sprite(renderer->getGC(), "brickbg", &resMan);
}

void TileTestScene::update(unsigned int msecs)
{
	if (!msecs) return;

	CL_Size tileSize = m_brick.get_size();
	CL_Size wndSize  = m_manager->getRenderer()->getGC().get_size();

	CL_InputDevice  &keys   = m_manager->getRenderer()->getIC().get_keyboard();
	CL_InputDevice  &mouse  = m_manager->getRenderer()->getIC().get_mouse();

	// update player:
	UpdateCtx ctx = { keys, mouse, m_map };
	m_player->update(ctx, msecs);

	// update camera:
	CL_Rectf rect = m_player->getRect();
	m_offX = rect.get_center().x * tileSize.width - wndSize.width/2;
	m_offY = rect.get_center().y * tileSize.width - wndSize.height/2;
}

void TileTestScene::render()
{
	Renderer::Ref renderer = m_manager->getRenderer();

	// resolve 
	CL_Size wndSize  = renderer->getGC().get_size();
	CL_Size tileSize = m_brick.get_size();

	// render tilemap:
	const int tilesInX = 3 + wndSize.width  / tileSize.width;
	const int tilesInY = 3 + wndSize.height / tileSize.height;

	const int startX = m_offX / tileSize.width - 1;
	const int startY = m_offY / tileSize.height - 1;

	for (int tileX = startX; tileX < startX + tilesInX; ++ tileX)
	for (int tileY = startY; tileY < startY + tilesInY; ++ tileY)
	{
		if (m_map.getTile(tileX, tileY).flags & tf_Blocking)
		{
			// draw shade and original sprite:
			m_brick.draw(renderer->getGC(), tileX * tileSize.width - m_offX, tileY * tileSize.height - m_offY);
		}
	}

	// render player:
	RenderCtx ctx = { renderer->getGC(), tileSize, CL_Point(m_offX, m_offY) };
	m_player->render(ctx);
}

//************************************************************************************************************************
