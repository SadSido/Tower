// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: the scene, that allows player to restart the level

#include "LevelFailedScene.h"
#include "LevelScene.h"
#include "../sys/GameManager.h"
#include "../sys/Renderer.h"
#include "../util/Generic.h"

//************************************************************************************************************************

// c-tor and d-tor:

LevelFailedScene::LevelFailedScene(GameManager * manager, int levelNo)
: GameScene(manager), m_levelNo(levelNo)
{
	auto renderer = m_manager->getRenderer();

	// create font:
	m_font = CL_Font_System(renderer->getGC(), "Microsoft Sans Serif", 32);

	// attach input handlers:
	m_slots.connect(renderer->getIC().get_keyboard().sig_key_down(), this, &LevelFailedScene::onKeyDown);
	m_slots.connect(renderer->getIC().get_mouse().sig_key_down(), this, &LevelFailedScene::onKeyDown);
}

// scene lifecycle:

void LevelFailedScene::update(float secs)
{
}

void LevelFailedScene::render()
{
	Renderer::Ref renderer = m_manager->getRenderer();

	const CL_Pointf margins(40.0f, 40.0f);
	const CL_String caption = "Level failed, (*| ) restart ( |*) quit";

	m_font.draw_text(renderer->getGC(), margins, caption, CL_Colorf::white);
}

// input event handlers:

void LevelFailedScene::onKeyDown(const CL_InputEvent &key, const CL_InputState &state)
{
	auto cfg = m_manager->getConfig();

	// left mouse - retry the level:

	if (key.id == cfg->mouseLeft())
	{ m_manager->repScene(LevelScene::createLevel(m_manager, m_levelNo)); }

	// right mouse - quit to main menu:

	if (key.id == cfg->mouseRight())
	{ m_manager->popScene(); }
}


//************************************************************************************************************************
