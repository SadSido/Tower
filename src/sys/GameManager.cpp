// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: main class for running the game

#include "GameManager.h"
#include "../gmd/IntroScene.h"
#include "../gmd/TileTest.h"

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/sound.h>

//************************************************************************************************************************

GameManager::GameManager()
{
}

// scene management:

void GameManager::pushScene(GameScene::Ref scene)
{
	m_stack.push_back(scene);
}

void GameManager::popScene()
{
	m_stack.pop_back();
}

// main game loop:

int GameManager::runMainLoop()
{
	// initialize ClanLib:
	CL_SetupCore setup_core;
	CL_SetupDisplay setup_display;
	CL_SetupGL setup_opengl;

	// initialize services:
	m_config.reset(new Configuration());
	m_renderer.reset(new Renderer());

	// push initial scene:
	pushScene(GameScene::Ref(new IntroScene(this)));
	pushScene(GameScene::Ref(new TileTest(this)));

	unsigned int lastTick  = CL_System::get_time();

	// main game loop:
	while (m_stack.size())
	{
		GameScene::Ref topScene = m_stack.back();

		unsigned int currTick = CL_System::get_time();
		unsigned int interval = (currTick - lastTick);

		// flexible inteval update:
		if (interval)
		{ topScene->update(interval); }

		// render the scene:
		m_renderer->preRender();
		topScene->render();
		m_renderer->postRender();

		// remember the ticks:
		lastTick += interval;
		CL_KeepAlive::process();
	}

	return 0;
}

//************************************************************************************************************************
