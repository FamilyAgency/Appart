#pragma once
enum OFX_FILTER_PARAMETER_TYPE
{
	FILTER_PARAMETER_FLOAT,
	FILTER_PARAMETER_INT,
	FILTER_PARAMETER_TEXTURE,
	FILTER_PARAMETER_2F,
	FILTER_PARAMETER_3F,
	FILTER_PARAMETER_4F,
	FILTER_PARAMETER_MATRIX4F,
	FILTER_PARAMETER_FV,
	FILTER_PARAMETER_2FV,
	FILTER_PARAMETER_3FV
};

#include "ofMain.h"

class AbstractParameter
{
public:
	AbstractParameter() { _name = ""; }
	AbstractParameter(const string& name) { _name = name; };

	virtual ~AbstractParameter() {}

	void            setName(string name) { _name = name; }
	string          getName() { return _name; }
	OFX_FILTER_PARAMETER_TYPE getType() { return _type; }
	template<class T>
	void            setValue(T value) {}
	template<class T>
	T *              getValue() { return NULL; }

protected:
	string          _name;
	OFX_FILTER_PARAMETER_TYPE   _type;
};
