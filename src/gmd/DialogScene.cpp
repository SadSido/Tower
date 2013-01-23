// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: the scene, showing the dialog between characters

#include "DialogScene.h"
#include "../sys/GameManager.h"
#include "../sys/Renderer.h"
#include "../util/BasePath.h"

//************************************************************************************************************************

namespace
{

// color helpers:

CL_Colorf getPhraseColor(PhraseType type)
{
	switch (type)
	{
	case pht_None:   return CL_Colorf::azure;
	case pht_Player: return CL_Colorf::white;
	case pht_NPC:    return CL_Colorf::gray;
	}

	// assert (false);
	return CL_Colorf::black;
}

}

//************************************************************************************************************************

// c-tor and d-tor:

DialogScene::DialogScene(GameManager * manager, DialogScript::Ref script)
: GameScene(manager), m_topScene(manager->getTopScene()), m_script(script), m_iter(m_script->begin())
{
	auto renderer = m_manager->getRenderer();
	
	// create font data:
	m_font = CL_Font_System(renderer->getGC(), "Microsoft Sans Serif", 32);

	// provide initial layout:
	updateRects(renderer->getGCSize());
	updateLayout();

	// attach input handlers:
	m_slots.connect(renderer->getIC().get_keyboard().sig_key_down(), this, &DialogScene::onKeyDown);
	m_slots.connect(renderer->getIC().get_mouse().sig_key_down(), this, &DialogScene::onKeyDown);
}

// scene lifecycle:

void DialogScene::update(float secs)
{
	// finish the dialog:
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
	// CL_Draw::fill(renderer->getGC(), CL_Rectf(window), CL_Colorf(0.0f, 0.0f, 0.0f, 0.5f));
	CL_Draw::fill(renderer->getGC(), m_rcBub, getPhraseColor(m_type));

	m_layout.draw_layout(renderer->getGC());
}

// input handlers:

void DialogScene::onKeyDown(const CL_InputEvent &key, const CL_InputState &state)
{
	// proceed to the next replic:
	if (++ m_iter != m_script->end())
	{ updateLayout(); }	
}

// helpers:

void DialogScene::updateRects(CL_Size window)
{
	const int margins = 20;

	m_rcBub  = CL_Rect(window); m_rcBub.shrink(window.width/8, margins, window.width/8, 2 * window.height / 3);
	m_rcText = m_rcBub; m_rcText.shrink(margins);
}

void DialogScene::updateLayout()
{
	// update phrase type:
	m_type = m_iter->first;

	// update text layout:
	m_layout.clear();
	m_layout.set_position(m_rcText.get_top_left());
	m_layout.set_align(cl_left);

	m_layout.add_text(m_iter->second, m_font, CL_Colorf::black);
	m_layout.layout(m_manager->getRenderer()->getGC(), (int)m_rcText.get_width());
}

//************************************************************************************************************************
