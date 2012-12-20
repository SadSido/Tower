// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: user-controlled game object

#include "EntTest.h"
#include "Player.h"

//************************************************************************************************************************
	
EntTest::EntTest(CL_Pointf pos, CL_Sizef size)
: Entity(pos, size), m_collapse(false)
{
	m_vel = CL_Pointf(rand() % 100, rand() % 100).normalize() * 0.005f;
}

bool EntTest::update(const UpdateCtx &ctx, unsigned int msecs)
{
	if (!m_collapse)
	{
		TileTest moveTest = ctx.tilemap->checkMove(m_rect, m_vel * (float)msecs);

		if (moveTest.type == th_Horizontal) { m_vel.x *= -1.0f; }
		if (moveTest.type == th_Vertical)   { m_vel.y *= -1.0f; }

		m_rect.translate(moveTest.delta);
		m_collapse = m_rect.is_overlapped(ctx.player.getRect());

		return true;
	}
	else
	{
		m_rect.shrink(msecs * 0.001f);
		return m_rect.get_width() > 0.0f;
	}
}

bool EntTest::render(const RenderCtx &ctx)
{
	CL_Draw::box(ctx.gc, ctx.tilemap->toScreen(m_rect), CL_Colorf(255,255,0));
	return true;
}

//************************************************************************************************************************
