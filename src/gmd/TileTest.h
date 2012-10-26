// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: the very first scene in the game

#ifndef _TileTest_h_
#define _TileTest_h_

#include "../sys/GameScene.h"
#include "Tilemap.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>

//************************************************************************************************************************

class TileTest : public GameScene
{
public:
	explicit TileTest(GameManager * manager);

	virtual void update(unsigned int msecs);
	virtual void render();

private:
	CL_Sprite m_brick;
	CL_Sprite m_brickbg;

	Tilemap m_map;

	float m_offX;
	float m_offY;

	bool m_ground;
	bool m_mount;
	bool m_damaged;

	float     m_ropeLen;
	CL_Pointf m_rope;
	CL_Pointf m_acc;
	CL_Pointf m_vel;
	CL_Rectf  m_obj;
};

//************************************************************************************************************************

#endif
