// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: user-controlled game object

#ifndef _Player_h_
#define _Player_h_

#include "TileTest.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>

//************************************************************************************************************************

class Player
{
public:
	explicit Player();

	void update(const LevelScene::UpdateCtx &ctx, unsigned int msecs);
	void render(const LevelScene::RenderCtx &ctx);

	CL_Rectf getRect() const
	{ return m_obj; }

private:
	bool      m_ground;
	float     m_ropeLen;
	CL_Pointf m_rope;
	CL_Pointf m_acc;
	CL_Pointf m_vel;
	CL_Rectf  m_obj;
};

//************************************************************************************************************************

#endif
