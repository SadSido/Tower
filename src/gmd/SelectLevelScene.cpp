// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: the scene for choosing starting level

#include "SelectLevelScene.h"
#include "LevelScene.h"
#include "../sys/GameManager.h"
#include "../sys/Renderer.h"
#include "../util/Generic.h"

//************************************************************************************************************************

// c-tor and d-tor:

SelectLevelScene::SelectLevelScene(GameManager * manager)
: GameScene(manager), m_selected(0)
{
	auto renderer = m_manager->getRenderer();

	// create font:
	m_font = CL_Font_System(renderer->getGC(), "Microsoft Sans Serif", 32);

	// attach input handlers:
	m_slots.connect(renderer->getIC().get_keyboard().sig_key_down(), this, &SelectLevelScene::onKeyDown);
	m_slots.connect(renderer->getIC().get_mouse().sig_key_down(), this, &SelectLevelScene::onKeyDown);
}

// scene lifecycle:

void SelectLevelScene::update(float secs)
{
}

void SelectLevelScene::render()
{
	Renderer::Ref renderer = m_manager->getRenderer();

	// render options in a column:
	const CL_Pointf margins(40.0f, 40.0f);
	for (int itemNo = 0; itemNo < LevelScene::countLevels(); ++ itemNo)
	{
		const CL_Pointf offset  = CL_Pointf(0.0f, 40.0f * itemNo);
		const CL_Colorf color   = (itemNo == m_selected) ? CL_Colorf::red : CL_Colorf::white;
		const CL_String caption = cl_format("Level %1", itemNo + 1);
		
		m_font.draw_text(renderer->getGC(), margins + offset, caption, color);
	}
}

// input event handlers:

void SelectLevelScene::onKeyDown(const CL_InputEvent &key, const CL_InputState &state)
{
	auto cfg = m_manager->getConfig();
	auto cnt = LevelScene::countLevels();

	if (key.id == cfg->keyPause())
	{ m_manager->popScene(); }

	if (key.id == cfg->mouseLeft())
	{ m_manager->repScene(LevelScene::createLevel(m_manager, m_selected)); }

	if (key.id == cfg->keyUp())
	{ m_selected = max(m_selected - 1, 0); }

	if (key.id == cfg->keyDown())
	{ m_selected = min(m_selected + 1, cnt - 1); }
}


//************************************************************************************************************************
