// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: base class for the game monsters

#ifndef _MonsterEntity_h_
#define _MonsterEntity_h_

#include "../gmd/Entities.h"
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <memory>

//************************************************************************************************************************

struct LevelCtx;

//************************************************************************************************************************

class MonsterEntity : public Entity
{
public:
	#pragma region event handlers

	// monster's events customize its behavior. Event can handle its
	// own properties, initialized in constructor via xml attributes:
	
	struct Event
	{
		typedef std::shared_ptr<Event> Ref;
		
		explicit Event();
		virtual ~Event();

		virtual bool process(MonsterEntity * owner, const LevelCtx &ctx);
	};
	
	#pragma endregion

private:
	// virtual entity interface:
	virtual bool update (const LevelCtx &ctx, float secs);
	virtual bool render (const LevelCtx &ctx);
	virtual void upload (const LevelCtx &ctx);

protected:
	bool m_alive;

	CL_Pointf m_basePos;
	CL_Pointf m_nextPos;

	// customizable behavior:
	Event::Ref m_onEmerge;
	Event::Ref m_onDestin;
	Event::Ref m_onDetect;

public:

	// to create a monster, one must specify the set 
	// of its event handlers via template constructor:

	template<typename EMERGE, typename DESTIN, typename DETECT>
	explicit MonsterEntity(const CL_DomNodeList &plist)
	: m_alive(true)
	{
		// create event handlers:
		m_onEmerge = Event::Ref(new EMERGE(plist));
		m_onDestin = Event::Ref(new DESTIN(plist));
		m_onDetect = Event::Ref(new DETECT(plist));
	}
};

//************************************************************************************************************************

#endif
