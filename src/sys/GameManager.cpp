// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: main class for running the game

#include "GameManager.h"
#include "../gmd/SelectLevelScene.h"

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

void GameManager::repScene(GameScene::Ref scene)
{
	m_stack.pop_back();
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
	m_sounder.reset(new Sounder());

	// push initial scene:
	pushScene(GameScene::Ref(new SelectLevelScene(this)));

	// main game loop:
	unsigned int lastTick = CL_System::get_time();
	while (m_stack.size())
	{
		GameScene::Ref topScene = getTopScene();

		unsigned int currTick = CL_System::get_time();
		unsigned int interval = (currTick - lastTick);

		// flexible inteval update:
		if (interval)
		{ 
			float secs = interval / 1000.0f;
			topScene->update(secs); 
		}

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
