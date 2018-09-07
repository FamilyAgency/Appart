#pragma once

#include "ParameterV.h"
#include "ParameterF.h"
#include "ParameterI.h"
#include "ParameterT.h"
#include "Parameter2f.h"
#include "Parameter3f.h"
#include "Parameter4f.h"
#include "ParameterMatrix4f.h"
#include "Parameter2fv.h"
#include "Parameter3fv.h"

class ParametricObject
{
public:
	ParametricObject();
	virtual ~ParametricObject();

	template<class T>
	void updateParameter(string name, T value) { parameters[name]->setValue(value); }
	void updateParameter(string name, float value) { ((ParameterF *)parameters[name])->setValue(value); }
	void updateParameter(string name, int value) { ((ParameterI*)parameters[name])->setValue(value); }
	void updateParameter(string name, ofVec2f value) { ((Parameter2f *)parameters[name])->setValue(value); }
	void updateParameter(string name, ofVec3f value) { ((Parameter3f *)parameters[name])->setValue(value); }
	void updateParameter(string name, ofVec4f value) { ((Parameter4f *)parameters[name])->setValue(value); }
	void updateParameter(string name, ofMatrix4x4 value) { ((ParameterMatrix4f *)parameters[name])->setValue(value); }
	void updateParameter(string name, ofTexture value) { ((ParameterT *)parameters[name])->setValue(value); }
	void updateParameter(string name, float * value) { ((ParameterV *)parameters[name])->setValue(value); }
	AbstractParameter * getParameter(string name) { return parameters[name]; }

	virtual void    describeParameters();

protected:	
	void            updateParameters(ofShader* shader);
	void            addParameter(AbstractParameter* parameter);
	void            clearParameters();

private:
	map<string, AbstractParameter*> parameters;
	void            updateParameter(ofShader* shader, AbstractParameter* parameter);
	string          getNameForParameterType(OFX_FILTER_PARAMETER_TYPE type);
};
