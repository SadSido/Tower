// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: the very first scene in the game

#ifndef _TileTest_h_
#define _TileTest_h_

#include "../sys/GameScene.h"
#include "Tilemap.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>

//************************************************************************************************************************

class Player;

//************************************************************************************************************************

class TileTestScene : public GameScene
{
public:
	// level objects get this when to calculate update:
	struct UpdateCtx
	{
		CL_InputDevice &keys;
		CL_InputDevice &mouse;
		Tilemap &map;
	};

	// level objects get this when they have to render:
	struct RenderCtx
	{
		CL_GraphicContext &gc;
		Tilemap &map;
	};

public:
	explicit TileTestScene(GameManager * manager);

	virtual void update(unsigned int msecs);
	virtual void render();

private:
	CL_Sprite m_brick;
	CL_Sprite m_brickbg;

	Tilemap  m_map;
	Player * m_player;
};

typedef TileTestScene LevelScene;

//************************************************************************************************************************

#endif
