// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: classes, required to run dialog scripts

#ifndef _Dialogs_h_
#define _Dialogs_h_

#include <ClanLib/core.h>
#include <map>

//************************************************************************************************************************

enum PhraseType
{
	pht_Player,
	pht_NPC,
	pht_None,
};

//************************************************************************************************************************

class DlgScript
{
	typedef std::pair<bool, CL_String> Condition;
	typedef std::pair<PhraseType, CL_String> Phrase;

public:
	explicit DlgScript();

	// editing script content while parsing:

	void addPrec(bool direct, const CL_String &global);
	void addPost(bool direct, const CL_String &global);
	void addText(PhraseType type, const CL_String &text);


private:
	std::vector<Condition> m_precs;
	std::vector<Condition> m_posts;
	std::vector<Phrase> m_texts;
};


class Dialogs
{
public:
	explicit Dialogs();

private:
	// maps the owners to their set of dialogs:
	std::map<CL_String, std::vector<DlgScript>> m_map;
};

//************************************************************************************************************************

#endif
