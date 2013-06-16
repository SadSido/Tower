// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: game scene interface

#include "GameScene.h"

//************************************************************************************************************************

GameScene::GameScene(GameManager * manager)
: m_manager(manager)
{}

GameScene::~GameScene()
{}

// no implementation for basic notification:

void GameScene::notify(Notify code, void * data)
{}

//************************************************************************************************************************
