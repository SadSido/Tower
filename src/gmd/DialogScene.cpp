// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: the scene, showing the dialog between characters

#include "DialogScene.h"
#include "../sys/GameManager.h"
#include "../sys/Renderer.h"
#include "../util/BasePath.h"

/*
	CL_FontDescription font_desc;
	font_desc.set_typeface_name("Tahoma");
	font_desc.set_anti_alias(true);
	font_desc.set_height(32);
	CL_Font_System font_normal(gc, font_desc);

	font_desc.set_weight(800);
	font_desc.set_height(40);
	CL_Font_System font_bold(gc, font_desc);


	std::vector<CL_SpanLayout> layout;

	// Count number of lines
	int num_lines = 0;
	for (const char **text_ptr = TextToShow; *text_ptr; text_ptr++)
	{
		num_lines++;
	}

	// Extend layout vector
	layout.resize(num_lines);

	int ypos = 100;
	for (int line_count=0; line_count < num_lines; line_count++, ypos += 8)
	{
		layout[line_count] = CL_SpanLayout();

		layout[line_count].set_position(CL_Point(40, ypos));

		layout[line_count].set_align(cl_justify);

		const char *text_ptr = TextToShow[line_count];
		if (text_ptr[0] == '-')
		{
			layout[line_count].add_text(text_ptr, font_normal, CL_Colorf::red);
		}
		else if (strstr(text_ptr, "Clan"))
		{
			layout[line_count].add_text(text_ptr, font_bold, CL_Colorf::yellow);
		}
		else
		{
			layout[line_count].add_text(text_ptr, font_normal, CL_Colorf::yellow);
		}

		layout[line_count].layout(gc, texture_text.get_width() - 80);

		ypos += layout[line_count].get_size().height;

	}
*/

//************************************************************************************************************************

// c-tor and d-tor:

DialogScene::DialogScene(GameManager * manager, DialogScript::Ref script)
: GameScene(manager), m_topScene(manager->getTopScene()), m_script(script), m_iter(m_script->begin()), m_delay(0.0f)
{
	auto renderer = m_manager->getRenderer();
	CL_Sizef window = renderer->getGCSize();

	m_rect = CL_Rectf(CL_Sizef(window.width, window.height/3.0f));

	CL_FontDescription font_desc;
	font_desc.set_typeface_name("Tahoma");
	font_desc.set_anti_alias(true);
	font_desc.set_height(32);
	m_font = CL_Font_System(renderer->getGC(), font_desc);

	updateLayout();
}

// scene lifecycle:

void DialogScene::update(float secs)
{
	CL_InputDevice  &keys   = m_manager->getRenderer()->getIC().get_keyboard();
	CL_InputDevice  &mouse  = m_manager->getRenderer()->getIC().get_mouse();

	if (m_delay == 0.0f && mouse.get_keycode(CL_MOUSE_LEFT))
	{
		++ m_iter;
		if (m_iter != m_script->end())
		{ 
			m_delay = 0.5f;
			updateLayout();
		}
	}

	if (m_iter == m_script->end())
	{ m_manager->popScene(); }

	// update mouse clicking delay:
	m_delay = max(0.0f, m_delay - secs);

}

void DialogScene::render()
{
	Renderer::Ref renderer = m_manager->getRenderer();
	CL_Sizef window = renderer->getGCSize();

	// render underlying scene:
	m_topScene->render();

	// fade out a little:
	CL_Draw::fill(renderer->getGC(), CL_Rectf(window), CL_Colorf(0.0f, 0.0f, 0.0f, 0.5f));
	CL_Draw::fill(renderer->getGC(), m_rect, CL_Colorf(1.0f, 1.0f, 1.0f, 1.0f));

	m_layout.draw_layout(renderer->getGC());
}

// helpers:

void DialogScene::updateLayout()
{
	m_layout.clear();
	m_layout.set_position(CL_Point());
	m_layout.set_align(cl_left);

	const CL_String &text = m_iter->second;
	m_layout.add_text(text, m_font, CL_Colorf::red);
	m_layout.layout(m_manager->getRenderer()->getGC(), (int)m_rect.get_width());
}

//************************************************************************************************************************
