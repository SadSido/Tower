// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: the scene, showing the dialog between characters

#include "DialogScene.h"
#include "../sys/GameManager.h"
#include "../sys/Renderer.h"
#include "../util/BasePath.h"
#include <assert.h>

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
	case pht_NPC:    return CL_Colorf::gray80;
	}

	assert(false);
	return CL_Colorf::black;
}

CL_Colorf blendColors(CL_Colorf one, CL_Colorf two, float percent)
{
	return CL_Colorf
	(
		one.r * percent + two.r * (1.0f - percent),
		one.g * percent + two.g * (1.0f - percent),
		one.b * percent + two.b * (1.0f - percent),
		one.a * percent + two.a * (1.0f - percent)
	);
}

}

//************************************************************************************************************************

// c-tor and d-tor:

DialogScene::DialogScene(GameManager * manager, DialogScript::Ref script, Globals & globals)
: GameScene(manager), m_topScene(manager->getTopScene()), m_script(script), m_iter(m_script->begin()), m_globals(globals)
{
	auto renderer = m_manager->getRenderer();
	
	// init phrase types animation:
	m_percent = 1.0f;
	m_oldtype = m_iter->first;
	m_newtype = m_iter->first;

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

void DialogScene::update(float secs, int msecs)
{
	// phrase type animation:
	if (m_oldtype != m_newtype)
	{
		m_percent = min(1.0f, m_percent + secs * 3.0f);
		if (m_percent == 1.0f) { m_oldtype = m_newtype; }
	}

	// finish the dialog:
	if (m_iter == m_script->end())
	{
		m_script->applyPosts(m_globals);
		m_manager->popScene(); 
	}
}

void DialogScene::render()
{
	Renderer::Ref renderer = m_manager->getRenderer();
	CL_Sizef window = renderer->getGCSize();

	// render underlying scene:
	m_topScene->render();

	// fade out a little:
	CL_Rectf rcShadow = m_rcBub; rcShadow.translate(CL_Pointf(5.0f, 5.0f));
	CL_Colorf bubColor = blendColors(getPhraseColor(m_newtype), getPhraseColor(m_oldtype), m_percent);

	CL_Draw::fill(renderer->getGC(), rcShadow, CL_Colorf::black);
	CL_Draw::fill(renderer->getGC(), m_rcBub, bubColor);

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
	m_newtype = m_iter->first;
	m_percent = 0.0f;

	// update text layout:
	m_layout.clear();
	m_layout.set_position(m_rcText.get_top_left());
	m_layout.set_align(cl_justify);

	m_layout.add_text(m_iter->second, m_font, CL_Colorf(0,0,0,100));
	m_layout.layout(m_manager->getRenderer()->getGC(), (int)m_rcText.get_width());
}

//************************************************************************************************************************
