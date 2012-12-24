// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: base scene for every game level

#ifndef _LevelScene_h_
#define _LevelScene_h_

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

class LevelScene : public GameScene
{
public:
	explicit LevelScene(GameManager * manager, CL_String descName);

	// scene lifecycle:	
	virtual void update(unsigned int msecs);
	virtual void render();

	// area management:
	void enterArea(CL_String name, CL_String entry);

private:
	CL_Sprite m_brick;
	CL_Sprite m_brickbg;

	Entities::Ref m_entities;
	Tilemap::Ref  m_tilemap;
	
	Globals  m_globals;
	Player   m_player;
	Areas    m_areas;
};

//************************************************************************************************************************

#endif
