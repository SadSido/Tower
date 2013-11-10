// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: the scene for switching areas

#include "AreaScene.h"
#include "LevelScene.h"
#include "../sys/GameManager.h"
#include "../sys/Renderer.h"

//************************************************************************************************************************

static const float s_speedFactor = 4.0f;

//************************************************************************************************************************

// c-tor and d-tor:

AreaScene::AreaScene(GameManager * manager, const CL_String &areaName, const CL_String &entryName)
: GameScene(manager), m_topScene(manager->getTopScene()), m_direct(true), m_percent(0.0f), m_areaName(areaName), m_entryName(entryName)
{
}

// scene lifecycle:

void AreaScene::update(float secs)
{
	if (m_direct)
	{
		// fade-out:
		m_percent = min(1.0f, m_percent + secs * s_speedFactor);
		if (m_percent == 1.0f)
		{
			NotifyAreaData data = { m_areaName, m_entryName };
			m_topScene->notify(n_EnterArea, (void*)(&data));
			m_direct = false;
		}
	}
	else
	{
		// fade-in:
		m_percent = max(0.0f, m_percent - secs * s_speedFactor);
		if (m_percent == 0.0f)
		{
			m_manager->popScene();
			m_direct = true;
		}
	}
}

void AreaScene::render()
{
	// render underlying scene:
	m_topScene->render();

	// render fading shadow:
	auto renderer = m_manager->getRenderer();
	CL_Draw::fill(renderer->getGC(), CL_Rectf(renderer->getGCSize()), CL_Colorf(0.0f, 0.0f, 0.0f, m_percent));
}

//************************************************************************************************************************
