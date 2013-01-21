// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: entity, which triggers a dialog scene

#include "DialogEntity.h"

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

	if (auto dialog = m_dlgSet->getDialog(ctx.globals))
	{
		const float distance = m_rect.get_center().distance(ctx.player.getRect().get_center());
		if ((distance < m_distance) && ctx.keys.get_keycode(CL_KEY_W))
		{
			ctx.player.setVel(CL_Pointf(0,0));
			// start the dialog:
		}
	}

	// perform the rest of the update:
	// ...
	return true;
}

bool DialogEntity::render(const RenderCtx &ctx)
{
	CL_Draw::box(ctx.gc, ctx.tilemap->toScreen(m_rect), CL_Colorf(255,255,255));
	return true;
}

//************************************************************************************************************************
