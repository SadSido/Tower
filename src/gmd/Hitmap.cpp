// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: classes for handling combat damage

#include "Hitmap.h"
#include "../util/XmlUtils.h"
#include <assert.h>

//************************************************************************************************************************

Hitmap::Hitmap()
{}

Hitmap::Hitmap(const CL_String &id, CL_ResourceManager * resources)
{
	CL_Resource resource = resources->get_resource(id);
	assert(resource.get_element().get_tag_name() == "hitmap");

	auto rects = resource.get_element().get_elements_by_tag_name("rect");	
	for (int no = 0; no < rects.get_length(); ++ no)
	{
		CL_DomElement rect = rects.item(no).to_element();
		m_rects.push_back(readRect(rect));
	}

	// ensure we have rects:
	assert(m_rects.size());
}

CL_Rectf Hitmap::getRect(int index) const 
{
	const size_t pos = (size_t)index;
	return (pos < m_rects.size()) ? m_rects.at(pos) : CL_Rectf();
}

//************************************************************************************************************************
