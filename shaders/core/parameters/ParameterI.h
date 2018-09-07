#pragma once
#include "AbstractParameter.h"

class ParameterI : public AbstractParameter
{
public:
	ParameterI(string name, int value) : AbstractParameter(name)
	{
		_value = value;
		_type = FILTER_PARAMETER_INT;
	}
	virtual ~ParameterI() {}

	int     getValue() { return _value; }
	void    setValue(int value) { _value = value; }

private:

	int     _value;
};
