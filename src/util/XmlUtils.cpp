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

//************************************************************************************************************************
