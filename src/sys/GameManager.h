// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: main class for running the game

#ifndef _GameManager_h_
#define _GameManager_h_

#include <vector>
#include "GameScene.h"
#include "Configuration.h"
#include "Renderer.h"
#include "Sounder.h"

//************************************************************************************************************************

class GameManager
{
public:
	explicit GameManager();

	// main game loop:
	int runMainLoop();

	// scene management:
	void pushScene (GameScene::Ref scene);
	void repScene  (GameScene::Ref scene);
	void popScene  ();

	// services and managers:
	GameScene::Ref getTopScene() const
	{ return m_stack.back(); }

	Configuration::Ref getConfig() const
	{ return m_config; }

	Renderer::Ref getRenderer() const
	{ return m_renderer; }


private:
	std::vector<GameScene::Ref> m_stack;
	Configuration::Ref m_config;
	Renderer::Ref m_renderer;
	Sounder::Ref m_sounder;
};

//************************************************************************************************************************

#endif
