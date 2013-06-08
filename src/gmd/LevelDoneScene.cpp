// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: the scene, that allows player to proceed to the next level

#include "LevelDoneScene.h"
#include "IntroScene.h"
#include "../sys/GameManager.h"
#include "../sys/Renderer.h"
#include "../util/Generic.h"

//************************************************************************************************************************

// c-tor and d-tor:

LevelDoneScene::LevelDoneScene(GameManager * manager, int levelNo)
: GameScene(manager), m_levelNo(levelNo)
{
	auto renderer = m_manager->getRenderer();

	// create font:
	m_font = CL_Font_System(renderer->getGC(), "Microsoft Sans Serif", 32);

	// attach input handlers:
	m_slots.connect(renderer->getIC().get_keyboard().sig_key_down(), this, &LevelDoneScene::onKeyDown);
	m_slots.connect(renderer->getIC().get_mouse().sig_key_down(), this, &LevelDoneScene::onKeyDown);
}

// scene lifecycle:

void LevelDoneScene::update(float secs)
{
}

void LevelDoneScene::render()
{
	Renderer::Ref renderer = m_manager->getRenderer();

	const CL_Pointf margins(40.0f, 40.0f);
	const CL_String caption = "Level completed, (*| ) proceed ( |*) quit";

	m_font.draw_text(renderer->getGC(), margins, caption, CL_Colorf::white);
}

// input event handlers:

void LevelDoneScene::onKeyDown(const CL_InputEvent &key, const CL_InputState &state)
{
	auto cfg = m_manager->getConfig();

	// left mouse - proceed further:
	if (key.id == cfg->mouseLeft())
	{
		const int nextLevel = m_levelNo + 1;
		m_manager->repScene(IntroScene::createIntro(m_manager, nextLevel));
	}

	// right mouse - quit to main menu:
	if (key.id == cfg->mouseRight())
	{ m_manager->popScene(); }
}


//************************************************************************************************************************
