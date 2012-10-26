// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: the very first scene in the game

#include "TileTest.h"
#include "../sys/GameManager.h"
#include "../sys/Renderer.h"

//************************************************************************************************************************

TileTest::TileTest(GameManager * manager)
: GameScene(manager), m_map(500,500), m_offX(0), m_offY(0), m_vel(0, 0), m_acc(0, 0), m_mount(false), m_ground(false), m_damaged(false)
, m_obj(CL_Pointf(8.0f, 10.0f), CL_Sizef(0.8f, 1.6f))
{
	Configuration::Ref config = m_manager->getConfig();
	Renderer::Ref renderer = m_manager->getRenderer();

	// load up necessary resources:
	CL_String resPath = config->getPath("intro.xml");
	CL_ResourceManager resMan = CL_ResourceManager(resPath);

	m_brick = CL_Sprite(renderer->getGC(), "brick", &resMan);
	m_brickbg = CL_Sprite(renderer->getGC(), "brickbg", &resMan);
}

void TileTest::update(unsigned int msecs)
{
	if (!msecs)
	{
		int a = 0;
	}

	CL_Size tileSize = m_brick.get_size();
	CL_Size wndSize  = m_manager->getRenderer()->getGC().get_size();

	CL_InputContext &input  = m_manager->getRenderer()->getIC();
	CL_InputDevice  &keybrd = input.get_keyboard();
	CL_InputDevice  &mouse  = input.get_mouse();

	// update input:
	m_acc.y = (m_ground) ? 0 : +0.00002f;

	if (keybrd.get_keycode(CL_KEY_S))
	{ 
		// m_acc.y = +0.00001f;
	}
	else if (keybrd.get_keycode(CL_KEY_W) && m_ground)
	{ 
		//m_acc.y = -0.00001f;
		m_vel.y = -0.012f;
		m_ground = false;
	}
	else
	{
		// m_acc.y = 0.0f;
	}

	if (keybrd.get_keycode(CL_KEY_D))
	{ 
		m_acc.x = +0.00001f;
	}
	else if (keybrd.get_keycode(CL_KEY_A))
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

	if (mouse.get_keycode(CL_MOUSE_LEFT) && !m_mount)
	{
		CL_Pointf ptr = (CL_Pointf(mouse.get_x(), mouse.get_y()) + CL_Pointf(m_offX,m_offY)) / (float)tileSize.width;
		CL_Pointf ropeTest = ptr - m_obj.get_center();
		CL_Rectf ropeRect = CL_Rectf(m_obj.get_center(), CL_Sizef(0.01f, 0.01f));
		int dummy = 0; ropeTest = m_map.checkMove(ropeRect, ropeTest, dummy);

		if (dummy)
		{
			m_mount = true;
			m_ropeLen = ropeTest.length();
			m_rope = m_obj.get_center() + ropeTest;
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

	if (keybrd.get_keycode(CL_KEY_ESCAPE))
	{ 
		m_manager->popScene(); 
	}

	// update objects:
	m_vel += m_acc * msecs;

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

	int hit = 0, dummy = 0;
	//m_map.resetColor();

	CL_Pointf delta  = m_map.checkMove(m_obj, m_vel * msecs, hit);
	CL_Pointf grTest = m_map.checkMove(m_obj, CL_Pointf(0, 0.1f), dummy);

	if (hit && !m_damaged)
	{
		m_damaged = m_vel.length() > 0.015f; 
	}

	if (hit == 1) { m_vel.x = 0.0f; m_acc.x = 0.0f; }
	if (hit == 2) { m_vel.y = 0.0f; }
	m_ground = (grTest.y < 0.1f);


	m_obj.translate(delta);

	m_offX = m_obj.get_center().x * tileSize.width - wndSize.width/2;
	m_offY = m_obj.get_center().y * tileSize.width - wndSize.height/2;
}

void TileTest::render()
{
	Renderer::Ref renderer = m_manager->getRenderer();

	// resolve 
	CL_Size wndSize  = renderer->getGC().get_size();
	CL_Size tileSize = m_brick.get_size();

	const int tilesInX = 3 + wndSize.width  / tileSize.width;
	const int tilesInY = 3 + wndSize.height / tileSize.height;

	const int startX = m_offX / tileSize.width - 1;
	const int startY = m_offY / tileSize.height - 1;

	/*
	for (int tileX = startX; tileX < startX + tilesInX; ++ tileX)
	for (int tileY = startY; tileY < startY + tilesInY; ++ tileY)
	{
		if (m_map.getTile(tileX, tileY))
		{
			// get shade offset from the center of the screen:
			float shadeX = ((tilesInX / 2.0f) - (tileX - startX));
			float shadeY = ((tilesInY / 2.0f) - (tileY - startY));

			// draw shade and original sprite:
			m_brickbg.draw(renderer->getGC(), tileX * tileSize.width - m_offX + 2 * shadeX, tileY * tileSize.height - m_offY + 2 * shadeY);
	//		m_brick.draw(renderer->getGC(), tileX * tileSize.width - m_offX, tileY * tileSize.height - m_offY);
		}
	}
	*/

	for (int tileX = startX; tileX < startX + tilesInX; ++ tileX)
	for (int tileY = startY; tileY < startY + tilesInY; ++ tileY)
	{
		if (m_map.getTile(tileX, tileY).flags & tf_Blocking)
		{
			// draw shade and original sprite:
			m_brick.draw(renderer->getGC(), tileX * tileSize.width - m_offX, tileY * tileSize.height - m_offY);
		}
	}

	// render lines
	CL_Rectf draw = CL_Rectf((m_obj.get_top_left()) * tileSize.width - CL_Pointf(m_offX, m_offY), m_obj.get_size() * tileSize.width);
	CL_Pointf cent = draw.get_center();

	CL_Colorf color = (m_damaged) ? CL_Colorf(255,0,0) : CL_Colorf(0,255,0);
	CL_Draw::box(renderer->getGC(), draw, color);

	if (m_mount)
	{
		CL_Pointf pt = m_rope * tileSize.width - CL_Point(m_offX, m_offY);
		CL_Draw::line(renderer->getGC(), cent, pt, CL_Colorf(255,0,0));

		CL_Pointf centr = CL_Pointf(m_rope * tileSize.width - CL_Pointf(m_offX, m_offY));
		CL_Draw::circle(renderer->getGC(), centr, 5, CL_Colorf(255,0,0));  
	}
}

//************************************************************************************************************************
