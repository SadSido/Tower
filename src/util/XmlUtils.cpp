// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: xml parsing wrappers

#include "XmlUtils.h"
#include <assert.h>

//************************************************************************************************************************

namespace 
{

template <typename RESULT>
RESULT readAttr(CL_DomElement node, const CL_String &one, const CL_String &two)
{
	return RESULT(node.get_attribute_float(one), node.get_attribute_float(two));
}

}

//************************************************************************************************************************

CL_Sizef readSize(CL_DomElement node)
{ return readAttr<CL_Sizef>(node, "width", "height"); }

CL_Pointf readPoint(CL_DomElement node)
{ return readAttr<CL_Pointf>(node, "x", "y"); }

CL_Pointf readPoint(CL_DomElement node, CL_String attrName)
{
	auto values = CL_StringHelp::split_text(node.get_attribute(attrName), ",");
	assert(values.size() == 2);

	CL_Pointf result = CL_Pointf(CL_StringHelp::text_to_float(values[0]), CL_StringHelp::text_to_float(values[1]));
	return result;
}

CL_Rectf readRect(CL_DomElement node)
{
	const float lft = node.get_attribute_float("l");
	const float top = node.get_attribute_float("t");

	if (node.has_attribute("w") && node.has_attribute("h"))
	{ return CL_Rectf(lft, top, CL_Sizef(node.get_attribute_float("w"), node.get_attribute_float("h"))); }

	if (node.has_attribute("r") && node.has_attribute("b"))
	{ return CL_Rectf(lft, top, node.get_attribute_float("r"), node.get_attribute_float("b")); }

	assert(false);
	return CL_Rectf();
}

//************************************************************************************************************************
