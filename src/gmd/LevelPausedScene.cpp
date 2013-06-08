// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: the scene, that allows player to quit / pause the level

#include "LevelPausedScene.h"
#include "LevelScene.h"
#include "../sys/GameManager.h"
#include "../sys/Renderer.h"
#include "../util/Generic.h"

//************************************************************************************************************************

static const float s_speedFactor = 4.0f;

//************************************************************************************************************************

// c-tor and d-tor:

LevelPausedScene::LevelPausedScene(GameManager * manager)
: GameScene(manager), m_topScene(manager->getTopScene()), m_percent(0.0f)
{
	auto renderer = m_manager->getRenderer();

	// create font:
	m_font = CL_Font_System(renderer->getGC(), "Microsoft Sans Serif", 32);

	// attach input handlers:
	m_slots.connect(renderer->getIC().get_keyboard().sig_key_down(), this, &LevelPausedScene::onKeyDown);
	m_slots.connect(renderer->getIC().get_mouse().sig_key_down(), this, &LevelPausedScene::onKeyDown);
}

// scene lifecycle:

void LevelPausedScene::update(float secs)
{
	m_percent = min(1.0f, m_percent + secs * s_speedFactor);
}

void LevelPausedScene::render()
{
	Renderer::Ref renderer = m_manager->getRenderer();

	const CL_Pointf margins(40.0f, 40.0f);
	const CL_String caption = "Game paused, (*| ) continue ( |*) quit";

	// render underlying scene:
	m_topScene->render();

	// render overlay:
	CL_Draw::fill(renderer->getGC(), CL_Rectf(renderer->getGCSize()), CL_Colorf(0.0f, 0.0f, 0.0f, m_percent));
	m_font.draw_text(renderer->getGC(), margins, caption, CL_Colorf::white);
}

// input event handlers:

void LevelPausedScene::onKeyDown(const CL_InputEvent &key, const CL_InputState &state)
{
	auto cfg = m_manager->getConfig();

	// left mouse - continue the game:

	if (key.id == cfg->mouseLeft())
	{ m_manager->popScene(); }

	// right mouse - quit to main menu:

	if (key.id == cfg->mouseRight())
	{ 
		m_manager->popScene();
		m_manager->popScene();
	}
}


//************************************************************************************************************************
