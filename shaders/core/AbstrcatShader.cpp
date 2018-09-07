#include "AbstractShader.h"


AbstractShader::AbstractShader()
{
	
}

AbstractShader::~AbstractShader()
{

}

void AbstractShader::setupShader()
{
	if (shader.isLoaded())
	{
		shader.unload();
	}

	//shader.setupShaderFromSource(GL_VERTEX_SHADER, getVertSrc());
	shader.setupShaderFromSource(GL_FRAGMENT_SHADER, getFragSrc());
	shader.linkProgram();
}

void AbstractShader::begin()
{
	shader.begin();
	updateParameters(&shader);	
}

void AbstractShader::end()
{
	shader.end();
}

string AbstractShader::getFragSrc()
{
	return GLSL_STRING(120,
		uniform sampler2D inputImageTexture;

		void main()
		{
			vec2 uv = gl_TexCoord[0].xy;
			gl_FragColor = texture2D(inputImageTexture, uv);
		}
	);
}

string AbstractShader::getVertSrc()
{
	return GLSL_STRING(150,
		uniform mat4 modelViewProjectionMatrix;
		in vec4 position;

		void main()
		{
			gl_Position = modelViewProjectionMatrix * position;
		}
	);
}

ofShader& AbstractShader::getShader() 
{
	return shader; 
}


