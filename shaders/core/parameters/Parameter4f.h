#pragma once
#include "AbstractParameter.h"

class Parameter4f : public AbstractParameter
{
public:
	Parameter4f(string name, ofVec4f value) : AbstractParameter(name)
	{
		_value = value;
		_type = FILTER_PARAMETER_4F;
	}
	virtual ~Parameter4f() {}

	ofVec4f     getValue() { return _value; }
	void        setValue(ofVec4f value) { _value = value; }

private:

	ofVec4f   _value;
};
