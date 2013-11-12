// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: map for variant types, useful for quick in-game parameters editing

#include "Databags.h"
#include "../util/Parsing.h"
#include "../util/BasePath.h"
#include <assert.h>

//************************************************************************************************************************

namespace 
{
	template <typename TYPE>
	TYPE splitTwo(const CL_String &data)
	{
		auto values = CL_StringHelp::split_text(data, ",");	assert(values.size() == 2);
		return TYPE(CL_StringHelp::text_to_float(values[0]), CL_StringHelp::text_to_float(values[1]));
	}

	void insertValue(Databag::Ref dbag, const CL_String &type, const CL_String &name, const CL_String &data)
	{
		if (type == "int")
		{ int value = CL_StringHelp::text_to_int(data, 10); dbag->set<int>(name, value); return; }

		if (type == "bool")
		{ bool value = (data == "true") || (data == "yes"); dbag->set<bool>(name, value); return; }

		if (type == "float")
		{ float value = CL_StringHelp::text_to_float(data); dbag->set<float>(name, value); return; }

		if (type == "string")
		{ dbag->set<CL_String>(name, data); return; }

		if (type == "point")
		{ CL_Pointf value = splitTwo<CL_Pointf>(data); dbag->set<CL_Pointf>(name, value); return; }

		if (type == "size")
		{ CL_Sizef value = splitTwo<CL_Sizef>(data); dbag->set<CL_Sizef>(name, value); return; }

		if (type == "range")
		{ Range value = splitTwo<Range>(data); dbag->set<Range>(name, value); return; }
	}

	Databag::Ref parseDatabag(CL_String::const_iterator &it)
	{
		auto dbag = Databag::Ref(new Databag());

		// allow empty braces:
		CL_String start = parseToken(it);
		if (start == "{}") return dbag;

		assert(start == "{");		
		for (CL_String type = parseToken(it); type != "}"; type = parseToken(it))
		{
			auto name = parseToken(it);
			auto data = parseQuotes(it);
			insertValue(dbag, type, name, data);
		}

		// return filled databag:
		return dbag;
	}
}

//************************************************************************************************************************

void Databags::loadBagFile(const CL_String &path)
{
	CL_String source = CL_File::read_text(path);
	loadBagFile(source.begin());
}

void Databags::loadBagFile(CL_String::const_iterator it)
{
	while (*it)
	{
		CL_String token = parseToken(it);

		// allow single-line comments:
		if (token == "//")
		{ parseLine(it); }
			
		else if (token == "databag")
		{ loadDatabag(it); }

		else
		{ assert(token.empty()); }
	}
}

void Databags::loadDatabag(CL_String::const_iterator &it)
{
	// parse out name and databag:
	auto name = parseToken(it);
	auto dbag = parseDatabag(it);

	// add databag to the registry:
	operator[](name) = dbag;
}

//************************************************************************************************************************
