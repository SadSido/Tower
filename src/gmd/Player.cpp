// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: the very first scene in the game

#include "Player.h"

//************************************************************************************************************************
	
Player::Player()
: m_ground(false)
{
}

void Player::update(const LevelScene::UpdateCtx &ctx, unsigned int msecs)
{
	/*
	CL_Size tileSize = m_brick.get_size();
	CL_Size wndSize  = m_manager->getRenderer()->getGC().get_size();
	*/
	/*
	CL_InputContext &input  = m_manager->getRenderer()->getIC();
	CL_InputDevice  &keybrd = input.get_keyboard();
	CL_InputDevice  &mouse  = input.get_mouse();
	*/
	// update input:
	m_acc.y = (m_ground) ? 0 : +0.00002f;

	if (ctx.keys.get_keycode(CL_KEY_W) && m_ground)
	{ 
		m_vel.y = -0.012f;
		m_ground = false;
	}
	else
	{
		// m_acc.y = 0.0f;
	}

	if (ctx.keys.get_keycode(CL_KEY_D))
	{ 
		m_acc.x = +0.00001f;
	}
	else if (ctx.keys.get_keycode(CL_KEY_A))
	{ 
		m_acc.x = -0.00001f;
	}
	else 
	{
		m_acc.x = 0.0f;
		if (m_ground)
		{
			m_vel.x = 0.0f;
		}
	}

	/*
	if (mouse.get_keycode(CL_MOUSE_LEFT) && !m_mount)
	{
		CL_Pointf ptr = (CL_Pointf(mouse.get_x(), mouse.get_y()) + CL_Pointf(m_offX,m_offY)) / (float)tileSize.width;
		CL_Pointf ropeDelta = ptr - m_obj.get_center();
		CL_Rectf ropeRect = CL_Rectf(m_obj.get_center(), CL_Sizef(0.01f, 0.01f));
		TileTest ropeTest = m_map.checkMove(ropeRect, ropeDelta);

		if (ropeTest.type)
		{
			m_mount = true;
			m_ropeLen = ropeTest.delta.length();
			m_rope = m_obj.get_center() + ropeTest.delta;
		}
	}

	if (m_mount && keybrd.get_keycode(CL_KEY_W))
	{
		m_ropeLen = max(m_ropeLen - 0.01f, 1.0f);
	}
	if (m_mount && keybrd.get_keycode(CL_KEY_S))
	{
		m_ropeLen = min(m_ropeLen + 0.01f, 10.0f);
	}

	if (keybrd.get_keycode(CL_KEY_SPACE) && m_mount)
	{
		m_mount = false;
	}
	*/
	
	if (ctx.keys.get_keycode(CL_KEY_ESCAPE))
	{ 
		// m_manager->popScene(); 
		std::abort();
	}

	// update objects:
	m_vel += m_acc * (float)msecs;

	/*
	// here: apply mount:
	if (m_mount)
	{
		CL_Pointf newPos  = m_obj.get_center() + m_vel * msecs;
		CL_Pointf toMount = m_rope - newPos;
		
		if (toMount.length() > m_ropeLen)
		{
			CL_Pointf corPos = m_rope - toMount.normalize() * m_ropeLen;
			m_vel = (corPos - m_obj.get_center()) / msecs;
		}
	}
	*/

	TileTest moveTest = ctx.map.checkMove(m_obj, m_vel * (float)msecs);
	TileTest gravTest = ctx.map.checkMove(m_obj, CL_Pointf(0, 0.1f));

	if (moveTest.type == th_Horizontal) { m_vel.x = 0.0f; m_acc.x = 0.0f; }
	if (moveTest.type == th_Vertical)   { m_vel.y = 0.0f; }

	m_ground = (gravTest.delta.y < 0.1f);


	m_obj.translate(moveTest.delta);

	/*
	m_offX = m_obj.get_center().x * tileSize.width - wndSize.width/2;
	m_offY = m_obj.get_center().y * tileSize.width - wndSize.height/2;
	*/
}

void Player::render(const LevelScene::RenderCtx &ctx)
{

}

//************************************************************************************************************************
