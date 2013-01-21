// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: the scene, showing the dialog between characters

#include "DialogScene.h"
#include "../sys/GameManager.h"
#include "../sys/Renderer.h"
#include "../util/BasePath.h"

//************************************************************************************************************************

DialogScene::DialogScene(GameManager * manager)
: GameScene(manager)
{
	Configuration::Ref config = m_manager->getConfig();
	Renderer::Ref renderer = m_manager->getRenderer();
}

void DialogScene::update(float secs)
{
}

void DialogScene::render()
{
}

//************************************************************************************************************************
