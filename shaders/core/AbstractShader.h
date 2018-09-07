#pragma once
#define GLSL_STRING(version, shader) #shader
//#define GLSL_STRING(version, shader)  "#version " #version "\n" #shader

#include "parameters/ParametricObject.h"

typedef shared_ptr<class AbstractShader> AbstractShaderPtr;

class AbstractShader : public ParametricObject
{
public:
	AbstractShader();
	virtual ~AbstractShader();

	virtual void    begin();
	virtual void    end();
	ofShader&       getShader();

protected:
	virtual void    setupShader();
	virtual string  getFragSrc();
	virtual string  getVertSrc();

	ofShader        shader;

};