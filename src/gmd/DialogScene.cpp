// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: the scene, showing the dialog between characters

#include "DialogScene.h"
#include "../sys/GameManager.h"
#include "../sys/Renderer.h"
#include "../util/BasePath.h"

/*
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
: GameScene(manager), m_topScene(manager->getTopScene()), m_script(script), m_iter(m_script->begin())
{
	auto renderer = m_manager->getRenderer();
	CL_Sizef window = renderer->getGCSize();

	m_rect = CL_Rectf(CL_Sizef(window.width, window.height/3.0f));
}

// scene lifecycle:

void DialogScene::update(float secs)
{
	CL_InputDevice  &keys   = m_manager->getRenderer()->getIC().get_keyboard();
	CL_InputDevice  &mouse  = m_manager->getRenderer()->getIC().get_mouse();

	if (m_iter == m_script->end())
	{ m_manager->popScene(); }

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
}

// helpers:

void DialogScene::updateLayout()
{

}

//************************************************************************************************************************
