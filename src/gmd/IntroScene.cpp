// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: the scene, playing intro before the level

#include "IntroScene.h"
#include "LevelScene.h"
#include "../sys/GameManager.h"
#include "../sys/Renderer.h"
#include "../util/Generic.h"
#include <assert.h>

//************************************************************************************************************************

static const float s_speedfactor = 40.0f;
static CL_String s_intronames [] = { "Level01", "Level02", "Level03", "Level04", "Final" };

//************************************************************************************************************************

GameScene::Ref IntroScene::createIntro(GameManager * manager, int levelNo)
{
	assert(levelNo < COUNTOF(s_intronames));
	return IntroScene::Ref(new IntroScene(manager, levelNo));
}

//************************************************************************************************************************

// c-tor and d-tor:

IntroScene::IntroScene(GameManager * manager, int levelNo)
: GameScene(manager), m_levelNo(levelNo)
{
	auto renderer = m_manager->getRenderer();

	// create font:
	m_font = CL_Font_System(renderer->getGC(), "Microsoft Sans Serif", 32);

	// get animation offset:
	m_offset = (float)renderer->getGCSize().height;

	// attach input handlers:
	m_slots.connect(renderer->getIC().get_keyboard().sig_key_down(), this, &IntroScene::onKeyDown);
	m_slots.connect(renderer->getIC().get_mouse().sig_key_down(), this, &IntroScene::onKeyDown);
}

// scene lifecycle:

void IntroScene::update(float secs)
{
	m_offset = max(0.0f, m_offset - secs * s_speedfactor);
	
	// auto start the level:
	if (m_offset == 0.0f)
	{ proceedToLevel();	}
}

void IntroScene::render()
{
	auto renderer = m_manager->getRenderer();
	m_font.draw_text(renderer->getGC(), 40.0f, m_offset, s_intronames[m_levelNo]);
}

// input event handlers:

void IntroScene::onKeyDown(const CL_InputEvent &key, const CL_InputState &state)
{
	auto cfg = m_manager->getConfig();
	proceedToLevel();
}

// scene helpers:

void IntroScene::proceedToLevel()
{
	if (m_levelNo < LevelScene::countLevels())
	{
		// we have a corresponding level:
		m_manager->repScene(LevelScene::createLevel(m_manager, m_levelNo)); 
	}
	else
	{ 
		// that was a final intro, so quit:
		m_manager->popScene(); 
	}
}

//************************************************************************************************************************
