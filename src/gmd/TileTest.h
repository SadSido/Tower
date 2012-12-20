// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: the very first scene in the game

#ifndef _TileTest_h_
#define _TileTest_h_

#include "../sys/GameScene.h"
#include "Areas.h"
#include "Tilemap.h"
#include "Globals.h"
#include "Dialogs.h"
#include "Player.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>

//************************************************************************************************************************

// level objects get this when they calculate update:
struct UpdateCtx
{
	CL_InputDevice &keys;
	CL_InputDevice &mouse;

	Entities::Ref entities;
	Tilemap::Ref  tilemap;

	Globals  &globals;
	Player   &player;
};

// level objects get this when they have to render:
struct RenderCtx
{
	CL_GraphicContext &gc;
	
	Entities::Ref entities;
	Tilemap::Ref  tilemap;
};

//************************************************************************************************************************

class TileTestScene : public GameScene
{
public:
	explicit TileTestScene(GameManager * manager);

	// scene lifecycle:	
	virtual void update(unsigned int msecs);
	virtual void render();

	// area management:
	void enterArea(CL_String name);

private:
	CL_Sprite m_brick;
	CL_Sprite m_brickbg;

	Entities::Ref m_entities;
	Tilemap::Ref  m_tilemap;
	
	Globals  m_globals;
	Player   m_player;
	Areas    m_areas;
};

typedef TileTestScene LevelScene;

//************************************************************************************************************************

#endif
