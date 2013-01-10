// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: user-controlled game object

#include "Player.h"
#include "LevelScene.h"

//************************************************************************************************************************
	
Player::Player(CL_Pointf pos, CL_Sizef size)
: Entity("player", "player"), m_ground(false), m_mount(false), m_stairs(false)
{
	setPos(pos);
	setSize(size);
}

bool Player::update(const UpdateCtx &ctx, float secs)
{
	// update input:
	m_acc.y = (m_ground || m_stairs) ? 0 : +10.0f;

	if (ctx.keys.get_keycode(CL_KEY_W) && m_ground)
	{ 
		m_vel.y = -8.0f;
		m_ground = false;
	}
	else if (ctx.keys.get_keycode(CL_KEY_S))
	{ 
		if (standOnStairs(ctx))
		{
			m_vel.y  = 6.0f;
			m_stairs = true;
		}
	}

	if (ctx.keys.get_keycode(CL_KEY_D))
	{ 
		m_acc.x = +6.0f;
	}
	else if (ctx.keys.get_keycode(CL_KEY_A))
	{ 
		m_acc.x = -6.0f;
	}
	else 
	{
		m_acc.x = 0.0f;
		if (m_ground || m_stairs)
		{
			m_vel.x = 0.0f;
		}
	}

	if (ctx.mouse.get_keycode(CL_MOUSE_LEFT) && !m_mount)
	{
		CL_Pointf ptr = ctx.tilemap->toTilespace(CL_Pointf(ctx.mouse.get_position()));
		CL_Pointf ropeDelta = ptr - m_rect.get_center();
		CL_Rectf ropeRect = CL_Rectf(m_rect.get_center(), CL_Sizef(0.01f, 0.01f));
		TileTest ropeTest = ctx.tilemap->checkMove(ropeRect, ropeDelta, anyBlocking);

		if (ropeTest.type)
		{
			m_mount = true;
			m_ropeLen = ropeTest.delta.length();
			m_rope = m_rect.get_center() + ropeTest.delta;
		}
	}

	if (m_mount && ctx.keys.get_keycode(CL_KEY_W))
	{
		m_ropeLen = max(m_ropeLen - 0.01f, 1.0f);
	}
	if (m_mount && ctx.keys.get_keycode(CL_KEY_S))
	{
		m_ropeLen = min(m_ropeLen + 0.01f, 10.0f);
	}

	if (ctx.keys.get_keycode(CL_KEY_SPACE) && m_mount)
	{
		m_mount = false;
	}
	
	if (ctx.keys.get_keycode(CL_KEY_ESCAPE))
	{
		ctx.globals.add(Globals::defeat());
	}

	// update objects:
	m_vel += m_acc * secs;

	// here: apply mount:
	if (m_mount)
	{
		CL_Pointf newPos  = m_rect.get_center() + m_vel * secs;
		CL_Pointf toMount = m_rope - newPos;
		
		if (toMount.length() > m_ropeLen)
		{
			CL_Pointf corPos = m_rope - toMount.normalize() * m_ropeLen;
			m_vel = (corPos - m_rect.get_center()) / secs;
		}
	}

	TileChecker check = (m_stairs) ? isBlocking : anyBlocking;
	TileTest moveTest = ctx.tilemap->checkMove(m_rect, m_vel * secs, check);

	if (moveTest.type == th_Horizontal) { m_vel.x = 0.0f; m_acc.x = 0.0f; }
	if (moveTest.type == th_Vertical)   { m_vel.y = 0.0f; }

	m_ground = standOnGround(ctx);
	if (!standOnStairs(ctx)) m_stairs = false;

	m_rect.translate(moveTest.delta);
	return true;
}

bool Player::render(const RenderCtx &ctx)
{
	CL_Draw::box(ctx.gc, ctx.tilemap->toScreen(m_rect), CL_Colorf(0,255,0));
	
	if (m_mount)
	{
		CL_Pointf a = ctx.tilemap->toScreen(m_rect.get_top_left());
		CL_Pointf b =  ctx.tilemap->toScreen(m_rope);

		CL_Draw::line(ctx.gc, a, b, CL_Colorf(255,0,0));
		CL_Draw::circle(ctx.gc, b, 5, CL_Colorf(255,0,0));  
	}

	return true;
}

// tilemap check helpers:

bool Player::standOnStairs(const UpdateCtx &ctx)
{
	bool one = anyStairs(ctx.tilemap->getTile(m_rect.get_bottom_left() + CL_Pointf(0, 0.5f)));
	bool two = anyStairs(ctx.tilemap->getTile(m_rect.get_bottom_right() + CL_Pointf(0, 0.5f)));

	return one && two;
}

bool Player::standOnGround(const UpdateCtx &ctx)
{
	bool one = anyBlocking(ctx.tilemap->getTile(m_rect.get_bottom_left() + CL_Pointf(0, 0.5f)));
	bool two = anyBlocking(ctx.tilemap->getTile(m_rect.get_bottom_right() + CL_Pointf(0, 0.5f)));

	return one || two;
}

//************************************************************************************************************************
