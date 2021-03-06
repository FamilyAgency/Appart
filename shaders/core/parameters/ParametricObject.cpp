#include "ParametricObject.h"

ParametricObject::ParametricObject()
{

}

ParametricObject::~ParametricObject()
{

}

void ParametricObject::addParameter(AbstractParameter *parameter)
{
	map<string, AbstractParameter*>::iterator it = parameters.find(parameter->getName());
	if (it != parameters.end())
	{   // delete the old one
		delete it->second;
		parameters.erase(it);
	}
	parameters[parameter->getName()] = parameter;
}

void ParametricObject::clearParameters()
{
	while (parameters.size() > 0)
	{
		AbstractParameter* p = parameters[0];
		parameters.erase(parameters.begin());
		delete p;
	}
}

void ParametricObject::updateParameters(ofShader* shader)
{
	map<string, AbstractParameter *>::iterator it;
	for (it = parameters.begin(); it != parameters.end(); it++)
	{
		updateParameter(shader, it->second);
	}
}

void ParametricObject::updateParameter(ofShader* shader, AbstractParameter* parameter)
{
	switch (parameter->getType())
	{
		case FILTER_PARAMETER_FLOAT:
			shader->setUniform1f(parameter->getName(), ((ParameterF *)parameter)->getValue());
			break;
		case FILTER_PARAMETER_INT:
			shader->setUniform1i(parameter->getName(), ((ParameterI *)parameter)->getValue());
			break;
		case FILTER_PARAMETER_TEXTURE:
			shader->setUniformTexture(parameter->getName(), ((ParameterT *)parameter)->getValue(), ((ParameterT *)parameter)->getTextureIndex());
			break;
		case FILTER_PARAMETER_2F:
			shader->setUniform2f(parameter->getName(), ((Parameter2f *)parameter)->getValue().x, ((Parameter2f *)parameter)->getValue().y);
			break;
		case FILTER_PARAMETER_3F:
			shader->setUniform3f(parameter->getName(), ((Parameter3f *)parameter)->getValue().x, ((Parameter3f *)parameter)->getValue().y, ((Parameter3f *)parameter)->getValue().z);
			break;
		case FILTER_PARAMETER_4F:
			shader->setUniform4f(parameter->getName(), ((Parameter4f *)parameter)->getValue().x, ((Parameter4f *)parameter)->getValue().y, ((Parameter4f *)parameter)->getValue().z, ((Parameter4f *)parameter)->getValue().w);
			break;
		case FILTER_PARAMETER_MATRIX4F:
			shader->setUniformMatrix4f(parameter->getName(), ((ParameterMatrix4f *)parameter)->getValue());
			break;
		case FILTER_PARAMETER_FV:
			shader->setUniform1fv(parameter->getName(), ((ParameterV *)parameter)->getValue(), ((ParameterV *)parameter)->getNumValues());
			break;
		case FILTER_PARAMETER_2FV:
			shader->setUniform2fv(parameter->getName(), ((Parameter2fv *)parameter)->getValue(), ((Parameter2fv *)parameter)->getNumValues());
			break;
		case FILTER_PARAMETER_3FV:
			shader->setUniform3fv(parameter->getName(), ((Parameter3fv *)parameter)->getValue(), ((Parameter3fv *)parameter)->getNumValues());
			break;
	}
}

void ParametricObject::describeParameters()
{
	map<string, AbstractParameter *>::iterator it;
	for (it = parameters.begin(); it != parameters.end(); it++)
	{
		printf("   %s (%s)\n", it->second->getName().c_str(), getNameForParameterType(it->second->getType()).c_str());
	}
}

string ParametricObject::getNameForParameterType(OFX_FILTER_PARAMETER_TYPE type)
{
	switch (type)
	{
		case FILTER_PARAMETER_FLOAT:		return "float";
		case FILTER_PARAMETER_INT:			return "int";
		case FILTER_PARAMETER_TEXTURE:		return "texture";
		case FILTER_PARAMETER_2F:			return "vec2f"; 
		case FILTER_PARAMETER_3F:			return "vec3f"; 
		case FILTER_PARAMETER_4F:			return "vec4f";
		case FILTER_PARAMETER_MATRIX4F:		return "matrix4f";
		case FILTER_PARAMETER_2FV:			return "2fv"; 
		case FILTER_PARAMETER_3FV:			return "3fv";		
	}
	return "";
}
