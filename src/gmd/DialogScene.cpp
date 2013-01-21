// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: the scene, showing the dialog between characters

#include "DialogScene.h"
#include "../sys/GameManager.h"
#include "../sys/Renderer.h"
#include "../util/BasePath.h"

//************************************************************************************************************************

// c-tor and d-tor:

DialogScene::DialogScene(GameManager * manager, DialogScript::Ref script)
: GameScene(manager), m_topScene(manager->getTopScene()), m_script(script)
{
	Configuration::Ref config = m_manager->getConfig();
	Renderer::Ref renderer = m_manager->getRenderer();
}

// scene lifecycle:

void DialogScene::update(float secs)
{
	CL_InputDevice  &keys   = m_manager->getRenderer()->getIC().get_keyboard();
	CL_InputDevice  &mouse  = m_manager->getRenderer()->getIC().get_mouse();

	if (keys.get_keycode(CL_KEY_ESCAPE))
	{
		m_manager->popScene();
	}
}

void DialogScene::render()
{
	Renderer::Ref renderer = m_manager->getRenderer();
	CL_Sizef window = renderer->getGC().get_size();

	// render underlying scene:
	m_topScene->render();

	// fade out a little:
	CL_Draw::fill(renderer->getGC(), CL_Rectf(window), CL_Colorf(0.0f, 0.0f, 0.0f, 0.5f));
}

//************************************************************************************************************************
