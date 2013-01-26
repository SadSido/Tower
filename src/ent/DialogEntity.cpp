// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: entity, which triggers a dialog scene

#include "DialogEntity.h"
#include "../sys/GameManager.h"
#include "../gmd/DialogScene.h"

//************************************************************************************************************************

DialogAction::DialogAction(DialogScript::Ref script)
: m_script(script)
{}

void DialogAction::execute(const UpdateCtx &ctx)
{
	// assert(m_script);
	GameScene::Ref dlgScene (new DialogScene(ctx.manager, m_script));
	ctx.manager->pushScene(dlgScene);
}

//************************************************************************************************************************
	
DialogEntity::DialogEntity(CL_String name, const CL_DomNodeList &props)
: Entity("DialogEntity", name)
{
	for (int prNo = 0; prNo < props.get_length(); ++ prNo)
	{
		// process current property:
		CL_DomElement prop = props.item(prNo).to_element();

		if (prop.get_attribute("name") == "Distance") 
		{ m_distance = prop.get_attribute_float("value"); }
	}

}

bool DialogEntity::update(const UpdateCtx &ctx, float secs)
{
	// checks whether we have a dialog available:
	if (!m_dlgSet) { m_dlgSet = ctx.dialogs[m_name]; }

	auto dialog = m_dlgSet->getDialog(ctx.globals);
	const float distance = m_rect.get_center().distance(ctx.player.getRect().get_center());

	if (!m_assigned && dialog && (distance < m_distance))
	{ assignAction(ctx.player, dialog); }

	else if (m_assigned && (!dialog || (distance >= m_distance)))
	{ resetAction(ctx.player); }

	// perform the rest of the update:
	// ...
	return true;
}

bool DialogEntity::render(const RenderCtx &ctx)
{
	CL_Draw::box(ctx.gc, ctx.tilemap->toScreen(m_rect), CL_Colorf(255,255,255));
	return true;
}


void DialogEntity::assignAction(Player& player, DialogScript::Ref script)
{
	player.setAction(PlayerAction::Ref(new DialogAction(script)));
	m_assigned = true;
}

void DialogEntity::resetAction(Player& player)
{
	player.setAction(0);
	m_assigned = false;
}


//************************************************************************************************************************
