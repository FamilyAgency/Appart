#pragma once
#include "ParameterV.h"

class Parameter2fv : public ParameterV
{
public:

	Parameter2fv(string name, float * value, int numValues) : ParameterV(name, value, numValues)
	{
		_type = FILTER_PARAMETER_2FV;
	}
	Parameter2fv(string name, vector<ofVec2f> value) : ParameterV(name, NULL, value.size() * 2)
	{
		_type = FILTER_PARAMETER_2FV;
		float * v = new float[value.size() * 2];
		for (int i = 0; i < value.size(); i++)
		{
			v[i * 2] = value[i].x;
			v[i * 2 + 1] = value[i].y;
		}
		this->setValue(v);
	}

	virtual ~Parameter2fv() {}

};
