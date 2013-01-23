// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: class, incapsulating rendering operations

#ifndef _Renderer_h_
#define _Renderer_h_

#include <memory>
#include <ClanLib/core.h>
#include <ClanLib/display.h>

//************************************************************************************************************************

class Renderer
{
public:
	typedef std::shared_ptr<Renderer> Ref;
	explicit Renderer();

	inline CL_GraphicContext& getGC() const
	{ return m_window.get_gc(); }

	inline CL_InputContext& getIC() const
	{ return m_window.get_ic(); }
	 
	inline CL_DisplayWindow& getWindow()
	{ return m_window; }

	inline CL_Size getGCSize() const
	{ return m_window.get_gc().get_size(); }

	void preRender();
	void postRender();

private:
	CL_DisplayWindow m_window;
};

//************************************************************************************************************************

#endif
