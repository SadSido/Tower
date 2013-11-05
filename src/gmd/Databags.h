// AUTHOR: Wiatcheslav "SadSido" Sidortsov
// ORIGIN: map for variant types, useful for quick in-game parameters editing

#ifndef _Databags_h_
#define _Databags_h_

#include <ClanLib/core.h>
#include <map>
#include <assert.h>

//************************************************************************************************************************

#define DECLARE_DATABAG_TYPE(TYPE) template <> struct Traits<TYPE> { enum { type = __LINE__ }; };

//************************************************************************************************************************

class Databag
{
	// data traits:

	template <typename TYPE>
	struct Traits {};

	// keep one macro per line =)
	
	DECLARE_DATABAG_TYPE(int);
	DECLARE_DATABAG_TYPE(float);
	DECLARE_DATABAG_TYPE(bool);

	DECLARE_DATABAG_TYPE(CL_String);
	DECLARE_DATABAG_TYPE(CL_Pointf);

	// data holders:

	struct Value
	{ typedef std::shared_ptr<Value> Ref; const int m_type; explicit Value(int type) : m_type(type) {} };

	template <typename TYPE> struct TypedValue : public Value
	{ const TYPE m_value; explicit TypedValue(TYPE value) : Value(Traits<TYPE>::type), m_value(value) {} };

public:

	typedef std::shared_ptr<Databag> Ref;
	typedef std::map<CL_String, Value::Ref> Map;

	explicit Databag();
	
	// setters and getters:

	template <typename TYPE> void set(const CL_String &key, const TYPE &value)
	{
		auto valptr = Value::Ref(new TypedValue<TYPE>(value));
		m_map[key] = valptr;
	}

	template <typename TYPE> TYPE get(const CL_String &key) const
	{
		auto it = m_map.find(key);
		if (it == m_map.end()) { return TYPE(); }  
		
		assert(it->second->m_type == Traits<TYPE>::type);
		return static_cast<TypedValue<TYPE>*>(it->second.get())->m_value;
	}

private:
	Map m_map;
};

//************************************************************************************************************************

struct Databags : public std::map<CL_String, Databag::Ref>
{
};

//************************************************************************************************************************

#endif