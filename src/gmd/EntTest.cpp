// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: user-controlled game object

#include "EntTest.h"
#include "Player.h"

//************************************************************************************************************************
	
EntTest::EntTest(CL_String name, CL_DomElement node)
: Entity("EntTest", name), m_collapse(false)
{
	int vx = rand() % 100, vy = rand() % 100;
	m_vel = CL_Pointf(float(vx), float(vy)).normalize() * 5.0f;
}

bool EntTest::update(const UpdateCtx &ctx, float secs)
{
	if (!m_collapse)
	{
		TileTest moveTest = ctx.tilemap->checkMove(m_rect, m_vel * secs, anyBlocking);

		if (moveTest.type == th_Horizontal) { m_vel.x *= -1.0f; }
		if (moveTest.type == th_Vertical)   { m_vel.y *= -1.0f; }

		m_rect.translate(moveTest.delta);
		m_collapse = m_rect.is_overlapped(ctx.player.getRect());

		return true;
	}
	else
	{
		m_rect.shrink(secs * 2.0f);
		return m_rect.get_width() > 0.0f;
	}
}

bool EntTest::render(const RenderCtx &ctx)
{
	CL_Draw::box(ctx.gc, ctx.tilemap->toScreen(m_rect), CL_Colorf(255,255,0));
	return true;
}

//************************************************************************************************************************
