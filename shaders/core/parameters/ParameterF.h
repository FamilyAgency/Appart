#pragma once
#include "AbstractParameter.h"

class ParameterF : public AbstractParameter
{
public:
	ParameterF(string name, float value) : AbstractParameter(name)
	{
		_value = value;
		_type = FILTER_PARAMETER_FLOAT;
	}
	virtual ~ParameterF() {}

	float   getValue() { return _value; }
	void    setValue(float value) { _value = value; }

private:

	float   _value;
};
