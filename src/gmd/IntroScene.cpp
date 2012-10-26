// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: the very first scene in the game

#include "IntroScene.h"
#include "../sys/GameManager.h"
#include "../sys/Renderer.h"

//************************************************************************************************************************

IntroScene::IntroScene(GameManager * manager)
: GameScene(manager)
{
	Configuration::Ref config = m_manager->getConfig();
	Renderer::Ref renderer = m_manager->getRenderer();

	// load up necessary resources:
	CL_String resPath = config->getPath("intro.xml");
	CL_ResourceManager resMan = CL_ResourceManager(resPath);

	m_logo = CL_Sprite(renderer->getGC(), "spr_logo", &resMan);
}

void IntroScene::update(unsigned int msecs)
{
	CL_InputContext &input  = m_manager->getRenderer()->getIC();
	CL_InputDevice  &keybrd = input.get_keyboard();

	// update the sprites:
	m_logo.update(msecs);

	if (keybrd.get_keycode(CL_KEY_ESCAPE))
	{ 
		m_manager->popScene(); 
	}
}

void IntroScene::render()
{
	Renderer::Ref renderer = m_manager->getRenderer();

	CL_Size size = renderer->getGC().get_size();
	m_logo.draw(renderer->getGC(), size.width / 2, size.height / 2);
}

//************************************************************************************************************************
