// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: game scene interface

#ifndef _GameScene_h_
#define _GameScene_h_

#include "Notify.h"
#include <memory>

//************************************************************************************************************************

class GameManager;

class GameScene
{
public:
	typedef std::shared_ptr<GameScene> Ref;
	explicit GameScene(GameManager * manager);

	virtual void update(float secs, int msecs) = 0;
	virtual void render() = 0;
	virtual void notify(Notify code, void * data);

protected:
	GameManager * m_manager;
};

//************************************************************************************************************************

#endif
